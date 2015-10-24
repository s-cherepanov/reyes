/*------------------------------------------------------
 * TITLE:  depth.cpp
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Z buffer class
 * (C)     Copyright $Date: 1997/11/29 18:02:41 $
 * $Log: depth.cpp,v $
 * Revision 1.2  1997/11/29 18:02:41  ady
 * Include bool.h
 *
 * Revision 1.1  1997/11/16 14:15:10  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// C includes
#include <stdio.h>
#include <math.h>

// C++ includes
#include "limits.h"

// Reyes includes
#include "useful.h"
#include "depth.h"
#include "rgbcolour.h"
#include "colour.h"
#include "bool.h"

//=======================================
// Constructor
//---------------------------------------
Depth::Depth(int width,int height)
{
  int x,y;

  this->width=width;
  this->height=height;

  depth=new float*[width];
  for(x=0;x<width;x++)
  {
    depth[x]=new float[height];
  }
  for(x=0;x<width;x++)
  {
    for(y=0;y<height;y++)
    {
      depth[x][y]=MAXFLOAT;
    }
  }
}

//=======================================
// Destructor
//---------------------------------------
Depth::~Depth()
{
  int x;

  for(x=0;x<width;x++)
  {
    delete[] depth[x];
  }
  delete[] depth;
}

//=======================================
// Clear
//---------------------------------------
void Depth::Clear()
{
  int x,y;

  for(x=0;x<width;x++)
  {
    for(y=0;y<height;y++)
    {
      depth[x][y]=MAXFLOAT;
    }
  }
}

//=======================================
// WriteDepth
//---------------------------------------
bool Depth::WriteDepth(int x,int y,float z)
{
  if ((x>=0)&&(x<width)&&(y>=0)&&(y<height))
  {
    if (z<depth[x][y])
    {
      depth[x][y]=z;
      return TRUE;
    }
  }
  return FALSE;
}

//=======================================
// ReadDepth
//---------------------------------------
float Depth::ReadDepth(int x,int y)
{
  if ((x>=0)&&(x<width)&&(y>=0)&&(y<height))
  {
    return depth[x][y];
  }
  else
  {
    return 0;
  }
}

//=======================================
//========STREAM INPUT/OUTPUT============
//=======================================

ostream &operator<<(ostream &io,const Depth &depth)
{
  int x,y;

  for(y=depth.height-1;y>=0;y--)
  {
    for(x=0;x<depth.width;x++)
    {
      io << depth.depth[x][y] << " ";
    }
    io << "\n";
  }
}

//=======================================
// WriteDepth_PBM
//---------------------------------------
bool Depth::WriteAsPBM(char *filename)
{
  FILE *fp=fopen(filename,"wb");
  int x,y,normz;
  float minz,maxz;
  unsigned char col;

  fprintf(fp,"P6\n%d %d\n255\n",width,height);
  maxz=0;
  minz=MAXFLOAT;
  for(y=0;y<height;y++)
  {
    for(x=0;x<width;x++)
    {
      if ((depth[x][y]>maxz)&&(depth[x][y]<MAXFLOAT)) maxz=depth[x][y];
      if (depth[x][y]<minz) minz=depth[x][y];
    }
  }
  if (minz==maxz) {maxz=minz+1;}
  for(y=height-1;y>=0;y--)
  {
    for(x=0;x<width;x++)
    {
      if (depth[x][y]<=maxz)
        col=(unsigned char)(255-(255*(depth[x][y]-minz)/(maxz-minz)));
      else
        col=0;
      fwrite(&col,sizeof(char),1,fp);
      fwrite(&col,sizeof(char),1,fp);
      fwrite(&col,sizeof(char),1,fp);
    }
  }
  fclose(fp);
  return TRUE;
}
