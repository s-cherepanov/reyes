/*------------------------------------------------------
 * TITLE:  image.cpp
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Image class
 * (C)     Copyright $Date: 1998/12/10 20:07:36 $
 * $Log: image.cpp,v $
 * Revision 1.3  1998/12/10 20:07:36  ady
 * Now using xSampingRate and ySamplingRate
 *
 * Revision 1.2  1997/11/29 18:02:41  ady
 * New dicing and estimate gridsize method
 *
 * Revision 1.1  1997/11/16 14:15:10  ady
 * Initial revision
 *
 * Revision 1.1  1997/08/28 00:05:57  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// C includes
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

// C++ includes
#include <bool.h>

// Reyes includes
#include "image.h"
#include "useful.h"

//=======================================
// Constructor
//---------------------------------------
Image::Image(int width,int height,image_type type)
{
  this->type=type;
  this->width=width;
  this->height=height;
  switch(type)
  {
    case GREYSCALE:
      data=(unsigned char*)calloc(1,width*height);
      break;
    case RGB:
      data=(unsigned char*)calloc(3,width*height);
      break;
  }
}

//=======================================
// Constructor
//---------------------------------------
Image::Image(char *filename)
{
  FILE *fp=fopen(filename,"rb");
  char line[256];
  int params,y;

  if (!fp)
  {
    fprintf(stderr,"Cannot open image '%s'\n",(char*)filename);
    exit(EXIT_FAILURE);
  }
  fscanf(fp,"%s\n",line);
  if (!strcmp(line,"P6"))
  {
    type=RGB;
  }
  else
  {
    if (!strcmp(line,"P5"))
      type=GREYSCALE;
    else
    {
      fprintf(stderr,"Sorry, don't understand image type `%s'\n",line);
      exit(EXIT_FAILURE);
    }
  }

  params=sscanf(line,"%d %d",&width,&height);
  while(params!=2)
  {
    fgets(line,256,fp);
    params=sscanf(line,"%d %d",&width,&height);
  }
  fgets(line,256,fp); // Get max range
  switch(type)
  {
    case GREYSCALE:
      data=(unsigned char*)malloc(width*height);
      for(y=0;y<height;y++)
      {
	fread(data+(width*(height-y-1)),1,width,fp);
      }
      break;
    case RGB:
      data=(unsigned char*)malloc(width*height*3);
      for(y=0;y<height;y++)
      {
	fread(data+(3*width*(height-y-1)),1,width*3,fp);
      }
      break;
  }
  fclose(fp);
}

//=======================================
// WriteColour
//---------------------------------------
void Image::WriteColour(int x,int y,Colour colour)
{
  unsigned char *pos=data+(y*width+x)*3;

  if ((x>=0)&&(x<width)&&(y>=0)&&(y<height))
  {
    pos[0]=(unsigned char)(CLAMP(colour.r,0,1)*255);
    pos[1]=(unsigned char)(CLAMP(colour.g,0,1)*255);
    pos[2]=(unsigned char)(CLAMP(colour.b,0,1)*255);
  }
}

//=======================================
// WriteGrey
//---------------------------------------
void Image::WriteGrey(int x,int y,float grey)
{
  unsigned char *pos=data+(y*width+x);

  if ((x>=0)&&(x<width)&&(y>=0)&&(y<height))
  {
    pos[0]=(int)(CLAMP(grey,0,1)*255);
  }
}

//=======================================
// ReadRGB
//---------------------------------------
Colour Image::ReadColour(int x,int y)
{
  Colour c;
  unsigned char *pos=data+(y*width+x)*3;

  if ((x>=0)&&(x<width)&&(y>=0)&&(y<height))
  {
    c.r=(*pos++)/255.0;
    c.g=(*pos++)/255.0;
    c.b=(*pos++)/255.0;
    return c;
  }
  else
  {
    return Colour(0,0,0);
  }
}

//=======================================
// ReadGrey
//---------------------------------------
float Image::ReadGrey(int x,int y)
{
  if ((x>=0)&&(x<width)&&(y>=0)&&(y<height))
  {
    return (data[y*height+x])/255.0;
  }
  else
    return 0;
}

//=======================================
// WriteAsPBM
//---------------------------------------
bool Image::WriteAsPBM(char* filename,int xSamplingRate=1,int ySamplingRate=1)
{
  FILE *fp=fopen(filename,"wb");
  int x,y,xs,ys,r,g,b;
  unsigned char col[3];

  if (!fp) return FALSE;
  switch(type)
  {
    case GREYSCALE:
      fprintf(fp,"P5\n");
      break;
    case RGB:
      fprintf(fp,"P6\n");
      break;
  }

  fprintf(fp,"%d %d\n",width/xSamplingRate,height/ySamplingRate);
  fprintf(fp,"255\n");

  for(y=((int)(height/ySamplingRate))*ySamplingRate-1;y>=0;y-=ySamplingRate)
  {
    for(x=0;x<width;x+=xSamplingRate)
    {
      r=0; g=0; b=0;
      for(xs=0;xs<xSamplingRate;xs++)
      {
	for(ys=0;ys<ySamplingRate;ys++)
	{
	  switch(type)
	  {
	    case RGB:
	      r+=data[3*(width*(y+ys)+(x+xs))];
	      g+=data[3*(width*(y+ys)+(x+xs))+1];
	      b+=data[3*(width*(y+ys)+(x+xs))+2];
	      break;
	  }
	}
      }
      col[0]=r/(xSamplingRate*ySamplingRate);
      col[1]=g/(xSamplingRate*ySamplingRate);
      col[2]=b/(xSamplingRate*ySamplingRate);
      fwrite((void*)&col,1,3,fp);
    }
  }
  return TRUE;
}
