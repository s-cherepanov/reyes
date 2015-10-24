/*------------------------------------------------------
 * TITLE:  noise.cpp
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Noise implementation
 * (C)     Copyright $Date: 1997/11/21 20:14:41 $
 * $Log: _noise.cpp,v $
 * Revision 1.2  1997/11/21 20:14:41  ady
 * Now writes out an image
 *
 * Revision 1.1  1997/11/16 14:15:10  ady
 * Initial revision
 *
 * Revision 1.1  1997/02/23 22:55:40  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>

#include "noise.h"
#include "image.h"

int main(int argc,char *argv[])
{
  Image image(100,100,RGB);
  int x,y;
  float v;
  float scale=0.04;

  initialise_noise();
  for(x=0;x<100;x++)
  {
    for(y=0;y<100;y++)
    {
      v=noise1D(x*scale,y*scale,0);
      image.WriteColour(x,y,Colour(v,v,v));
    }
  }
  image.WriteAsPBM("noise.pbm");
  return EXIT_SUCCESS;
}
