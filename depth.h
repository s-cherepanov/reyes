#ifndef depth_h
#define depth_h
/*------------------------------------------------------
 * TITLE:  depth.h
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Depth class
 * (C)     Copyright $Date: 1998/12/10 20:14:42 $
 * $Log: depth.h,v $
 * Revision 1.2  1998/12/10 20:14:42  ady
 * Now includes string.h
 *
 * Revision 1.1  1997/11/16 14:21:49  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// C++ includes
#include <String.h>

// Reyes includes
#include "rgbcolour.h"
#include "colour.h"

class Depth
{
  public:
    Depth(int width,int height);
    ~Depth();

    void Clear();

    bool WriteDepth(int x,int y,float z);
    float ReadDepth(int x,int y);

    // IO stream operations
    friend ostream &operator<<(ostream &io,const Depth &depth);

    // File output
    bool WriteAsPBM(char *filename);

  private:
    int width;
    int height;
    float **depth;
};

#endif
