#ifndef texturemap_h
#define texturemap_h
/*------------------------------------------------------
 * TITLE:  texturemap.h
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Multi-resolution texture map
 * (C)     Copyright $Date: 1999/04/01 19:59:59 $
 * $Log: texturemap.h,v $
 * Revision 1.2  1999/04/01 19:59:59  ady
 * *** empty log message ***
 *
 * Revision 1.1  1998/12/10 20:18:38  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// Wrap types
#define RI_BLACK       0
#define RI_PERIODIC    1
#define RI_CLAMP       2

class TextureMap
{
  public:
    TextureMap(int maxresolution=0,int samples=1,int swrap=RI_PERIODIC,int twrap=RI_PERIODIC,bool multiresolution=TRUE);
    TextureMap(char *imagefile,int swrap,int twrap,
                float (*filterfunc)(float,float,float,float),float swidth,float twidth);
    TextureMap(char *texturefile);
    ~TextureMap();

    void ReadFilteredSample(int resolution,float x,float y,float *sample);
    void ReadUnfilteredSample(int resolution,int x,int y,float *sample);

    void WriteSample(int resolution,int x,int y,float *sample);

    void GenerateFilteredMaps();

    void SaveTextureMap(char *texturefile);
    void WriteAsPBM(char *fn_pbm,int resolution);

    int Width(){return (1<<maxresolution);}
    int Height(){return (1<<maxresolution);}

    int MaxResolution(){return maxresolution;}

  private:
    void Allocate();
    void Free();

    int maxresolution;    /* Square with side of 2**resolution */
    int samples;          /* Samples in a pixel */
    int swrap,twrap;      /* Wrapping type */
    bool multiresolution; /* Store multi-resolution images - not needed for depth map */
    float **data;         /* Indexed by [resolution][y*height*samples+x*samples+s] */
};

#endif
