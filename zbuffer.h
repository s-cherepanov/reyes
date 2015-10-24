#ifndef zbuffer_h
#define zbuffer_h
/*------------------------------------------------------
 * TITLE:  zbuffer.h
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Z buffer class
 * (C)     Copyright $Date: 1998/12/10 20:19:16 $
 * $Log: zbuffer.h,v $
 * Revision 1.2  1998/12/10 20:19:16  ady
 * Changed sampling to Ri style
 *
 * Revision 1.1  1997/11/16 14:21:49  ady
 * Initial revision
 *
 *-----------------------------------------------------*/


// Reyes includes
#include "image.h"
#include "depth.h"
#include "colour.h"

class ZBuffer
{
  public:
    ZBuffer(int width,int height,int xSamplingRate=1,int ySamplingRate=1):
      depth(width*xSamplingRate,height*ySamplingRate),
      image(width*xSamplingRate,height*ySamplingRate,RGB)
      {this->xSamplingRate=xSamplingRate;
       this->ySamplingRate=ySamplingRate;}

    WriteImageAsPBM(char* filename)
      {image.WriteAsPBM(filename,xSamplingRate,ySamplingRate);}

    void WriteColour(int x,int y,Colour colour)
      {image.WriteColour(x,y,colour);}

    bool WriteDepth(int x,int y,float z)
      {return depth.WriteDepth(x,y,z);}


  private:
    Image image;
    Depth depth;
    int xSamplingRate,ySamplingRate;
};

#endif
