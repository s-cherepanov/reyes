#ifndef camera_h
#define camera_h
/*------------------------------------------------------
 * TITLE:  camera.h
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Camera definition
 * (C)     Copyright $Date: 1997/11/16 14:21:49 $
 * $Log: camera.h,v $
 * Revision 1.1  1997/11/16 14:21:49  ady
 * Initial revision
 *
 * Revision 1.3  1997/08/28 00:16:13  ady
 * Changed values back to floats
 *
 * Revision 1.2  1997/03/10 21:55:57  ady
 * Added epsilon,hither and yon distances
 *
 * Revision 1.1  1997/02/23 23:00:16  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// Reyes includes
#include "matrix4.h"
#include "point3.h"

class Camera
{
  public:
    Camera(Point3 position=Point3(0,0,0),Point3 lookat=Point3(0,0,100),float FieldOfView=40);

    Matrix4 WorldToEyeMatrix();
    Matrix4 WorldNormalToEyeNormalMatrix();
    void ViewportScale(int width,float &scale);
    void Dump();

    float Epsilon(){return epsilon;}
    float Hither(){return hither;}
    float Yon(){return yon;}

  private:
    Point3 position;
    float FieldOfView;
    Vector3 u,v,d;
    float epsilon; // To close to do perspective
    float hither;  // Close enough
    float yon;     // Too far away
};

#endif
