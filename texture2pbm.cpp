/*------------------------------------------------------
 * TITLE:  texture2bpm.cpp
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Multi-resolution texture map
 * (C)     Copyright $Date: 1999/04/01 19:59:59 $
 * $Log: texture2pbm.cpp,v $
 * Revision 1.1  1999/04/01 19:59:59  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

#include "ri.h"

#include "texturemap.h"

int main(int argc,char *argv[])
{
  TextureMap *texmap=NULL;
  char fn[256];
  int r;

  if (argc-1!=2)
    {
      fprintf(stderr,"usage: texture2bpm <texturemap> <bpm file>\n");
    }
  texmap=new TextureMap(argv[1]);

  for(r=0;r<=texmap->MaxResolution();r++)
  {
    sprintf(fn,"%s%d.pbm",argv[2],r);
    texmap->WriteAsPBM(fn,r);
  }
}
