#ifndef shadowspot_h
#define shadowspot_h
/*------------------------------------------------------
 * TITLE:  shadowspot.h
 * AUTHOR: Adrian Skilling
 * ENVIR:  NetBSD
 * DESC:   Light source class
 * (C)     Copyright $Date: 1998/12/10 20:21:33 $
 * $Log: shadowspot.h,v $
 * Revision 1.1  1998/12/10 20:21:33  ady
 * Initial revision
 *
 * Revision 1.1  1997/11/29 17:56:18  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// Reyes includes
#include "light.h"

class Shadowspot: public Light
{
  public:
    Shadowspot(ParamList parameters);
    Shadowspot(Point3 _from=Point3(0,0,0),Point3 _to=Point3(0,0,1),Colour _color=Colour(1,1,1),float _intensity=1,float _coneangle=30,float _conedeltaangle=5,float _beamdistribution=2.0,char *map=NULL,float _radius=0);

    Colour Ambient();
    Colour Diffuse(Point3 P,Vector3 N);
    Colour Specular(Point3 P,Vector3 N,float shinyness);

    void Dump();
    void loadDepthMap(char *filename);

    friend ostream &operator<<(ostream &io,const Shadowspot &l);

  private:
    float LocalIntensity(Point3 p);
    float Visible(Point3 wP,Point3 eP);
    float SoftVisible(Point3 wP,Point3 eP);

    Point3 from,to;
    Colour colour;
    float intensity;
    float coneangle;
    float conedeltaangle;
    float beamdistribution;
    float minDepthDifference;
    float radius; // radius of light source
    // Depth map and transformation matrices
    char map[256];
    int width,height;
    float *depth;
    Matrix4 tWorldToCamera;  // Camera space= vantage point at origin, view along Z axis
    Matrix4 tCameraToRaster;
};

#endif
