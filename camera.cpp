/*------------------------------------------------------
 * TITLE:  camera.cpp
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Camera implementation
 * (C)     Copyright $Date: 1998/12/10 20:05:06 $
 * $Log: camera.cpp,v $
 * Revision 1.2  1998/12/10 20:05:06  ady
 * small fix
 *
 * Revision 1.1  1997/11/16 14:15:10  ady
 * Initial revision
 *
 * Revision 1.3  1997/08/28 00:05:57  ady
 * Added WorldNormalToEyeNormalMatrix() and Dump()
 *
 * Revision 1.2  1997/03/10 21:53:29  ady
 * Now have proper camera from camera position to viewpoint
 *
 * Revision 1.1  1997/02/23 22:55:40  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// C includes
#include <math.h>

// Reyes includes
#include "smath.h"
#include "point3.h"
#include "vector3.h"
#include "matrix4.h"
#include "camera.h"
#include "useful.h"

//=======================================
// Constructor
//---------------------------------------
Camera::Camera(Point3 position,Point3 lookat,float FieldOfView)
{
  this->position=position;
  this->FieldOfView=FieldOfView;
  d=lookat-position;
  d.Normalise();
  v=Vector3(0,1,0); // Up
  u=v^d;
  v=d^u;
  u.Normalise();
  v.Normalise();
  epsilon=0.1;
  hither=1;
  yon=1000000;
}

//=======================================
// WorldToEyeMatrix
//---------------------------------------
Matrix4 Camera::WorldToEyeMatrix()
{
  Matrix4 m;

  m[0][0]=u.x; m[0][1]=u.y; m[0][2]=u.z; m[0][3]=-(u.x*position.x+u.y*position.y+u.z*position.z);
  m[1][0]=v.x; m[1][1]=v.y; m[1][2]=v.z; m[1][3]=-(v.x*position.x+v.y*position.y+v.z*position.z);
  m[2][0]=d.x; m[2][1]=d.y; m[2][2]=d.z; m[2][3]=-(d.x*position.x+d.y*position.y+d.z*position.z);
  m[3][0]=0;   m[3][1]=0;   m[3][2]=0;   m[3][3]=1;
  return m;
}

//=======================================
// Dump
//---------------------------------------
void Camera::Dump()
{
  cout << "Position:" << position << " FieldOfView:" << FieldOfView << "\n";
  cout << "U:" << u << "\n";
  cout << "V:" << v << "\n";
  cout << "D:" << d << "\n";
}

//=======================================
// WorldNormalToEyeNormalMatrix
//---------------------------------------
Matrix4 Camera::WorldNormalToEyeNormalMatrix()
{
  Matrix4 m;

  m[0][0]=u.x; m[0][1]=u.y; m[0][2]=u.z; m[0][3]=0;
  m[1][0]=v.x; m[1][1]=v.y; m[1][2]=v.z; m[1][3]=0;
  m[2][0]=d.x; m[2][1]=d.y; m[2][2]=d.z; m[2][3]=0;
  m[3][0]=0;   m[3][1]=0;   m[3][2]=0;   m[3][3]=1;
  return m;
}

//=======================================
// WorldToEyeMatrix
//---------------------------------------
void Camera::ViewportScale(int width,float &scale)
{
  scale=width/(2.0*tan(FieldOfView*DEGTORAD/2));
}
