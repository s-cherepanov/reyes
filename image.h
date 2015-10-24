#ifndef image_h
#define image_h
/*------------------------------------------------------
 * TITLE:  image.h
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Image class
 * (C)     Copyright $Date: 1998/12/10 20:15:59 $
 * $Log: image.h,v $
 * Revision 1.2  1998/12/10 20:15:59  ady
 * Added xSamplingRate and ySamplingRate
 *
 * Revision 1.1  1997/11/16 14:21:49  ady
 * Initial revision
 *
 * Revision 1.1  1997/08/28 00:16:13  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// Reyes includes
#include "colour.h"

typedef enum
{
  GREYSCALE=0,
  RGB=1
}image_type;

class Image
{
  public:
    Image(int width,int height,image_type type=RGB);
    Image(char *filename);

    bool WriteAsPBM(char *filename,int xSamplingRate=1,int ySamplingRate=1);

    void WriteColour(int x,int y,Colour colour);
    void WriteGrey(int x,int y,float grey);

    Colour ReadColour(int x,int y);
    float ReadGrey(int x,int y);

    int Width(){return width;}
    int Height(){return height;}

  private:
    image_type type;
    int width,height;
    unsigned char *data;
};

#endif
