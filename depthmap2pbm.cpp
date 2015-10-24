/*------------------------------------------------------
 * TITLE:  depthmap2bpm.cpp
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Multi-resolution texture map
 * (C)     Copyright $Date: 1999/04/01 19:59:59 $
 * $Log: depthmap2pbm.cpp,v $
 * Revision 1.1  1999/04/01 19:59:59  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

#include <math.h>

#include "ri.h"

int main(int argc,char *argv[])
{
  FILE *fp_depthmap,*fp_image;
  int width,height;
  float *data_depthmap;
  unsigned char *data_image;
  int x,y;
  float v;
  float min=0,max=MAXFLOAT;
  Matrix4 t1,t2;

  if (argc-1<2)
    {
      fprintf(stderr,"usage: texture2bpm <depthmap file> <bpm file> [min depth] [max depth]\n");
      exit(EXIT_FAILURE);
    }

  if (argc-1>2) min=atof(argv[3]);
  if (argc-1>3) max=atof(argv[4]);

  fp_depthmap=fopen(argv[1],"rb");
  if (!fp_depthmap)
    RiError("Cannot open depth map `%s'",argv[1]);
  fp_image=fopen(argv[2],"wb");
  if (!fp_depthmap)
    RiError("Cannot open pbm file `%s'",argv[2]);

  fscanf(fp_depthmap,"%d\n%d\n",&width,&height);

  t1.readBinary(fp_depthmap);
  t2.readBinary(fp_depthmap);

  data_depthmap=(float*)calloc(sizeof(float),width);
  if (!data_depthmap)
    RiError("Cannot reserve %d bytes",sizeof(float)*width);

  data_image=(unsigned char*)malloc(width);
  if (!data_image)
    RiError("Cannot reserve %d bytes",width);

  fprintf(fp_image,"P5\n");
  fprintf(fp_image,"%d\n%d\n",width,height);
  fprintf(fp_image,"255\n");
  for(y=0;y<width;y++)
  {
    fread((void*)data_depthmap,sizeof(float),width,fp_depthmap);
    for(x=0;x<width;x++)
    {
      v=255.0*((data_depthmap[x]-min)/(max-min));
      data_image[x]=(int)CLAMP(v,0,255);
    }
    fwrite((void*)data_image,1,width,fp_image);
  }
  free(data_depthmap);
  free(data_image);
  fclose(fp_depthmap);
  fclose(fp_image);
}
