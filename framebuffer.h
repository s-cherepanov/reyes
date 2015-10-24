#ifndef framebuffer_h
#define framebuffer_h
/*------------------------------------------------------
 * TITLE:  framebuffer.h
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Grid of micropolygons class
 * (C)     Copyright $Date: 1999/04/01 19:59:59 $
 * $Log: framebuffer.h,v $
 * Revision 1.3  1999/04/01 19:59:59  ady
 * *** empty log message ***
 *
 * Revision 1.2  1998/12/10 20:15:17  ady
 * Added Rt types
 *
 * Revision 1.1  1997/12/08 23:09:37  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// C includes
#include <stdlib.h>

// Reyes includes
#include "colour.h"
#include "opacity.h"

typedef enum
{
  RtRGB,
  RtRGBZ,
  RtRGBAZ,
  RtAZ,
  RtZ
}RtDisplayMode;

class Pixel
{
  public:
    Pixel *next;
    float sample[1];
};

class FrameBuffer
{
  public:
    FrameBuffer(int width,int height,RtDisplayMode mode);
    void WriteColour(int x,int y,Colour colour);
    void WriteOpacity(int x,int y,Opacity opacity);
    bool WriteDepth(int x,int y,float z);
    bool WritePixel(int x,int y,float z,Colour colour,Opacity opacity);
    Colour GetCompositeColour(int x,int y);
    float GetDepth(int x,int y);

    RtDisplayMode DisplayMode(){return mode;}

    bool WriteImageAsPBM(char *filename);
    bool WriteDepthMap(char *filename);

  private:
    Pixel* newPixel();

    RtDisplayMode mode;    /* Display mode */
    Pixel ***pixels;
    int width,height;
    char *data;
    int pixelSize;
    int c,a,z;
    int samples; /* Number of samples, includes RGB,A and Z, but not pointer */
    int used;
    int free;
};

#endif
