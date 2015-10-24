/*------------------------------------------------------
 * TITLE:  framebuffer.cpp
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Grid of micropolygons class
 * (C)     Copyright $Date: 1999/04/01 19:59:59 $
 * $Log: framebuffer.cpp,v $
 * Revision 1.3  1999/04/01 19:59:59  ady
 * *** empty log message ***
 *
 * Revision 1.2  1998/12/10 20:06:53  ady
 * Now does something !
 *
 * Revision 1.1  1997/12/08 23:09:22  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// C includes
#include <math.h>

// C++ includes
#include "bool.h"

// Reyes includes
#include "framebuffer.h"
#include "ri.h"

#define EMPTY   1    /* Empty pixel */

FrameBuffer::FrameBuffer(int width,int height,RtDisplayMode mode)
{
  Pixel *pixel;
  int i,x,y;
  char *block;

  this->width=width;
  this->height=height;
  this->mode=mode;
  switch(mode)
  {
    case RtRGB:
    case RtRGBZ:
    case RtRGBAZ:
      samples=7;
      c=0;
      a=3;
      z=6;
      break;
    case RtZ:
      samples=1;
      c=-1;
      a=-1;
      z=0;
      break;
  }
  pixelSize=sizeof(float)*samples+sizeof(Pixel*);
  block=(char*)calloc(width*height,sizeof(Pixel*));
  pixels=(Pixel***)calloc(width,sizeof(Pixel*));
  for(x=0;x<width;x++)
  {
    pixels[x]=(Pixel**)(block+x*height*sizeof(Pixel*));
  }
  data=(char*)calloc(pixelSize*width*height,1);
  if (data==NULL)
    {cout << "No room for " << width << " x " << height << " pixels\n"; exit(EXIT_FAILURE);}
  used=0;
  free=width*height;
}

Pixel* FrameBuffer::newPixel()
{
  Pixel *pixel;
  int i;

  if (free==0)
  {
    data=(char*)realloc(data,2*used*pixelSize);
    if (!data) {cerr << "Cannot reserve space for more pixels\n"; exit(EXIT_FAILURE);}
    free=used;
  }
  // Get pointer to pixel and change free and used accordingly
  pixel=(Pixel*)(data+(pixelSize*used));
  free--;
  used++;

  pixel->next=NULL;
  // Initialise pixel as black, transparent and very very far away
  for(i=0;i<samples-1;i++) // Z is always last sample
  {
    pixel->sample[i]=0;
  }
  pixel->sample[z]=MAXFLOAT;

  return pixel;
}

void FrameBuffer::WriteColour(int x,int y,Colour colour)
{
  if (pixels[x][y]==NULL) pixels[x][y]=newPixel();
  if (pixels[x][y])
  {
    pixels[x][y]->sample[0]=colour.r;
    pixels[x][y]->sample[1]=colour.g;
    pixels[x][y]->sample[2]=colour.b;
  }
}

void FrameBuffer::WriteOpacity(int x,int y,Opacity opacity)
{
  if ((x>=0)&&(x<width)&&(y>=0)&&(y<height))
  {
    if (pixels[x][y]==NULL) pixels[x][y]=newPixel();
    if (pixels[x][y])
    {
      pixels[x][y]->sample[a+0]=opacity.r;
      pixels[x][y]->sample[a+1]=opacity.g;
      pixels[x][y]->sample[a+2]=opacity.b;
    }
  }
}

bool FrameBuffer::WriteDepth(int x,int y,float newz)
{
  if ((x<0)||(x>=width)||(y<0)||(y>=height)) return FALSE;
  if (pixels[x][y]==NULL) pixels[x][y]=newPixel();
  if (newz<pixels[x][y]->sample[z])
  {
    pixels[x][y]->sample[z]=newz;
    return TRUE;
  }
  return FALSE;
}

bool FrameBuffer::WritePixel(int x,int y,float newz,Colour colour,Opacity opacity)
{
  Pixel **pixelptr;
  Pixel *pixel;
  Pixel *newpixel;

  if ((x<0)||(x>=width)||(y<0)||(y>=height)) return FALSE;
  if ((opacity.r+opacity.g+opacity.b)>0)
  {
    if (pixels[x][y]==NULL) pixels[x][y]=newPixel();

    if (a==-1) /* Not using opacity - simply test for smallest Z */
    {
      if (newz<pixels[x][y]->sample[z])
      {
        if (c!=-1)
	{
	  pixels[x][y]->sample[c+0]=colour.r;
	  pixels[x][y]->sample[c+1]=colour.g;
	  pixels[x][y]->sample[c+2]=colour.b;
	}
	pixels[x][y]->sample[z]=newz;
      }
    }
    else
    {
      pixelptr=&pixels[x][y];
      pixel=pixels[x][y];
      while((pixel)&&(pixel->sample[z]>newz))
      {
	pixelptr=&(pixel->next);
	pixel=pixel->next;
      }
      newpixel=newPixel();
      *pixelptr=newpixel;   // Pointer to this pixel
      newpixel->next=pixel; // Pointer to next pixel
      if (c!=-1)
      {
        newpixel->sample[c+0]=colour.r;
        newpixel->sample[c+1]=colour.g;
        newpixel->sample[c+2]=colour.b;
      }
      if (a!=-1)
      {
        newpixel->sample[a+0]=opacity.r;
        newpixel->sample[a+1]=opacity.g;
        newpixel->sample[a+2]=opacity.b;
      }
      newpixel->sample[z]=newz;
    }
  }
  return TRUE;
}

float gamma(float value,float gain,float gamma)
{
  return pow(value*gain,gamma);
}

Colour FrameBuffer::GetCompositeColour(int x,int y)
{
  Colour colour;
  Pixel *pixel=pixels[x][y];
 
  while(pixel)
  {
    colour.r=pixel->sample[c+0]+colour.r*(1-pixel->sample[a+0]);
    colour.g=pixel->sample[c+1]+colour.g*(1-pixel->sample[a+1]);
    colour.b=pixel->sample[c+2]+colour.b*(1-pixel->sample[a+2]);
    pixel=pixel->next;
  }
  return colour;
}

float FrameBuffer::GetDepth(int x,int y)
{
  float depth=RI_INFINITY;
  Pixel *pixel=pixels[x][y];
 
  while(pixel)
  {
    depth=pixel->sample[z];
    pixel=pixel->next;
  }
  return depth;
}

bool FrameBuffer::WriteImageAsPBM(char *filename)
{
  FILE *fp=fopen(filename,"wb");
  int x,y;
  int ox,oy;
  float r,g,b;
  unsigned char col[3];
  unsigned char depth;
  float power=1/RiGlobal.display.gamma;
  int imagewidth=width/RiGlobal.display.xSamplingRate;
  int imageheight=height/RiGlobal.display.ySamplingRate;
  float xcent=0.5*(RiGlobal.display.filterXWidth-1);
  float ycent=0.5*(RiGlobal.display.filterYWidth-1);
  float f,sum,z;
  float filter[RiGlobal.display.filterXWidth][RiGlobal.display.filterYWidth];
  Colour colour;

  if (!fp) return FALSE;

  if (c!=-1)
    fprintf(fp,"P6\n");
  else
    fprintf(fp,"P5\n");

  fprintf(fp,"%d %d\n",imagewidth,imageheight);
  fprintf(fp,"255\n");

  if (c!=-1)
  {
    // Calculate filter values
    for(ox=0;ox<RiGlobal.display.filterXWidth;ox++)
    {
      for(oy=0;oy<RiGlobal.display.filterYWidth;oy++)
      {
	filter[ox][oy]=RiGlobal.display.filterFunction((ox-xcent),(oy-ycent),0.5*RiGlobal.display.xSamplingRate,0.5*RiGlobal.display.ySamplingRate);
      }
    }
    for(y=0;y<imageheight;y++)
    {
      for(x=0;x<imagewidth;x++)
      {
	r=0;
	g=0;
	b=0;
	sum=0;
	for(oy=0;oy<RiGlobal.display.filterYWidth;oy++)
        {
	  for(ox=0;ox<RiGlobal.display.filterXWidth;ox++)
          {
	    f=filter[ox][oy];
	    colour=GetCompositeColour(x*RiGlobal.display.xSamplingRate+ox,height-1-(y*RiGlobal.display.ySamplingRate+oy));
	    r+=f*colour.r;
	    g+=f*colour.g;
	    b+=f*colour.b;
	    sum+=f;
	  }
	}
	r=gamma(r/sum,RiGlobal.display.gain,power);
	g=gamma(g/sum,RiGlobal.display.gain,power);
	b=gamma(b/sum,RiGlobal.display.gain,power);
	col[0]=CLAMP((int)(r*255),0,255);
	col[1]=CLAMP((int)(g*255),0,255);
	col[2]=CLAMP((int)(b*255),0,255);
	fwrite((void*)&col,1,3,fp);
      }
    }
  }
  else
  {
    for(y=0;y<imageheight;y++)
    {
      for(x=0;x<imagewidth;x++)
      {
	z=GetDepth(x,height-1-y);
	depth=(int)CLAMP(RiGlobal.display.depthQuantizer.one*(z-RiGlobal.display.depthQuantizer.minimum)/
                         (RiGlobal.display.depthQuantizer.maximum-RiGlobal.display.depthQuantizer.minimum),
                         0,RiGlobal.display.depthQuantizer.one);
	fwrite((void*)&depth,1,1,fp);
      }
    }
  }
  return TRUE;
}

bool FrameBuffer::WriteDepthMap(char *filename)
{
  float sample;
  int x,y;
  FILE *fp=fopen(filename,"wb");

  if (!fp)
    RiError("Cannot open depth map file `%s'",filename);

  fprintf(fp,"%d\n%d\n",width,height);

  RiGlobal.tWorldToCamera.writeBinary(fp);
  RiGlobal.tCameraToRaster.writeBinary(fp);

  for(x=0;x<width;x++)
  {
    for(y=0;y<height;y++)
    {
      if (pixels[x][y])
	sample=pixels[x][y]->sample[z];
      else
	sample=RI_INFINITY;
      fwrite((void*)&sample,sizeof(float),1,fp);
    }
  }
  fclose(fp);
}
