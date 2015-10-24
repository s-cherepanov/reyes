/*------------------------------------------------------
 * TITLE:  texturemap.cpp
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Multi-resolution texture map
 * (C)     Copyright $Date: 1999/04/01 19:59:59 $
 * $Log: texturemap.cpp,v $
 * Revision 1.2  1999/04/01 19:59:59  ady
 * *** empty log message ***
 *
 * Revision 1.1  1998/12/10 20:12:41  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// C includes
#include <stdlib.h>
#include <stdio.h>

// Reyes includes
#include "ri.h"
#include "texturemap.h"
#include "image.h"
#include "useful.h"

TextureMap::TextureMap(int maxresolution,int samples,int swrap,int twrap,bool multiresolution)
{
  this->maxresolution=maxresolution;
  this->samples=samples;
  this->swrap=swrap;
  this->twrap=twrap;
  Allocate();
}

TextureMap::~TextureMap()
{
  Free();
}

TextureMap::TextureMap(char *imagefile,int swrap,int twrap,
                            float (*filterfunc)(float,float,float,float),float swidth,float twidth)
{
  Image image(imagefile);
  int resolution;
  int sres,tres,x,y;
  int imagewidth,imageheight;
  Colour col;
  float *pixel;

  imagewidth=image.Width();
  imageheight=image.Height();
  sres=(int)logn(2,imagewidth);
  tres=(int)logn(2,imageheight);

  this->maxresolution=MAX(sres,tres);
  this->samples=3;
  this->swrap=swrap;
  this->twrap=twrap;

  Allocate();

  pixel=data[maxresolution];
  for(y=0;y<imageheight;y++)
  {
    for(x=0;x<imagewidth;x++)
    {
      col=image.ReadColour(x,y);
      *pixel++=col.r;
      *pixel++=col.g;
      *pixel++=col.b;
    }
  }
  GenerateFilteredMaps();
}

TextureMap::TextureMap(char *texturefile)
{
  FILE *fp=fopen(texturefile,"rb");
  int r;

  if (!fp)
    {RiError("Cannot open texture file `%s' for input",texturefile);}

  fscanf(fp,"%d\n",&maxresolution);
  fscanf(fp,"%d\n",&samples);
  fscanf(fp,"%d\n",&swrap);
  fscanf(fp,"%d\n",&twrap);
  Allocate();
  for(r=0;r<=maxresolution;r++)
  {
    fread((void*)data[r],sizeof(float),(1<<r)*(1<<r)*samples,fp);
  }
  fclose(fp);
}

void TextureMap::Allocate()
{
  int r,x;
  int size=0;
  float *block;
  
  for(r=0;r<=maxresolution;r++)
  {
    size+=(1<<r)*(1<<r)*samples; /* Space for pixel entries */
  }

  block=(float*)calloc(size,sizeof(float));
  data=(float**)calloc(1+maxresolution,sizeof(float*));

  // Set up memory for pointers
  for(r=0;r<=maxresolution;r++)
  {
    data[r]=block;
    block+=(1<<r)*(1<<r)*samples;
  }
}

void TextureMap::Free()
{
  free(data[0]);
  free(data);
}

void TextureMap::ReadFilteredSample(int resolution,float x,float y,float *sample)
{
  int ix=(int)(5*x*(1<<resolution));
  int iy=(int)(5*y*(1<<resolution));
  float *pixel;
  int s;
  
  ix=((ix+(4<<resolution))%(1<<resolution));
  iy=((iy+(4<<resolution))%(1<<resolution));
  pixel=data[resolution]+(iy*(1<<resolution)*samples+ix*samples);
  for(s=0;s<samples;s++)
  {
    sample[s]=*pixel++;
  }
}

void TextureMap::ReadUnfilteredSample(int resolution,int x,int y,float *sample)
{
  float *pixel=data[resolution]+(y*(1<<resolution)*samples+x*samples);
  int s;

  for(s=0;s<samples;s++)
  {
    sample[s]=*pixel++;
  }
}

void TextureMap::WriteSample(int resolution,int x,int y,float *sample)
{
  float *pixel=&data[resolution][y*(1<<resolution)*samples+x*samples];
  int s;

  for(s=0;s<samples;s++)
  {
    *pixel++=sample[s];
  }
}

void TextureMap::GenerateFilteredMaps()
{
  int r,s,x,y;
  int lowres,highres;
  float sample[samples];
  float *sample1=(float*)calloc(samples,sizeof(float)),
        *sample2=(float*)calloc(samples,sizeof(float)),
        *sample3=(float*)calloc(samples,sizeof(float)),
        *sample4=(float*)calloc(samples,sizeof(float));

  for(r=maxresolution-1;r>=0;r--)
  {
    highres=2*(1<<r);
    lowres=(1<<r);
    for(x=0;x<lowres;x++)
    {
      for(y=0;y<lowres;y++)
      {
	ReadUnfilteredSample(r+1,x*2+0,y*2+0,sample1);
	ReadUnfilteredSample(r+1,x*2+1,y*2+0,sample2);
	ReadUnfilteredSample(r+1,x*2+0,y*2+1,sample3);
	ReadUnfilteredSample(r+1,x*2+1,y*2+1,sample4);
	for(s=0;s<samples;s++)
	{
	  sample[s]=0.25*(sample1[s]+sample2[s]+sample3[s]+sample4[s]);
	}
	WriteSample(r,x,y,sample);
      }
    }
  }
  free(sample1);
  free(sample2);
  free(sample3);
  free(sample4);
}

void TextureMap::SaveTextureMap(char *texturefile)
{
  FILE *fp=fopen(texturefile,"wb");
  int r;

  if (!fp)
    {RiError("Cannot open texture file `%s' for output",texturefile);}

  fprintf(fp,"%d\n",maxresolution);
  fprintf(fp,"%d\n",samples);
  fprintf(fp,"%d\n",swrap);
  fprintf(fp,"%d\n",twrap);
  for(r=0;r<=maxresolution;r++)
  {
    fwrite((void*)data[r],sizeof(float),(1<<r)*(1<<r)*samples,fp);
  }
  fclose(fp);
}

void TextureMap::WriteAsPBM(char *fn_pbm,int resolution)
{
  int x,y;
  int size=(1<<maxresolution);
  FILE *fp=fopen(fn_pbm,"wb");
  float f;

  fprintf(fp,"P5\n");
  fprintf(fp,"%d %d\n",1<<resolution,1<<resolution);
  fprintf(fp,"255\n");
  for(y=0;y<Height();y++)
  {
    for(x=0;x<Width();x++)
    {
      ReadUnfilteredSample(resolution,x,y,&f);
      f=CLAMP(f,0,255);
      fputc((int)f,fp);
    }
  }
  fclose(fp);
}
