/*------------------------------------------------------
 * TITLE:  cylinder.cpp
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Cylinder primitive class
 * (C)     Copyright $Date: 1999/04/01 21:38:02 $
 * $Log: cylinder.cpp,v $
 * Revision 1.6  1999/04/01 21:38:02  ady
 * Now calculates normal like in sphere
 *
 * Revision 1.5  1998/12/10 20:05:51  ady
 * Added dicing
 *
 * Revision 1.4  1997/11/29 18:02:41  ady
 * New dicing and estimate gridsize method
 *
 * Revision 1.3  1997/11/23 15:24:06  ady
 * Added AttachSurface and AttachDisplacement when dicing
 *
 * Revision 1.2  1997/11/21 20:14:41  ady
 * Modified for consistancy with new displacement shaders
 *
 * Revision 1.1  1997/11/16 14:15:10  ady
 * Initial revision
 *
 * Revision 1.1  1997/08/28 00:05:57  ady
 * Initial revision
 *
 * Revision 1.2  1997/03/10 22:00:39  ady
 *  New dice and split structure
 *
 * Revision 1.1  1997/02/23 22:58:14  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// C includes
#include <math.h>

// Reyes includes
#include "ri.h"
#include "smath.h"
#include "cylinder.h"
#include "patch.h"
#include "point3.h"
#include "matrix4.h"
#include "useful.h"

//=======================================
// Constructors
//---------------------------------------
Cylinder::Cylinder(float radius,float depth,float height,float arcmin,float arcmax,float tmin,float tmax)
{
  this->radius=radius;
  this->depth=depth;
  this->height=height;
  this->arcmin=arcmin;
  this->arcmax=arcmax;
  this->tmin=tmin;
  this->tmax=tmax;
}

//=======================================
//========STREAM INPUT/OUTPUT============
//=======================================

ostream &operator<<(ostream &io,const Cylinder &c)
{
  io << "Cylinder " << c.radius << " " << c.depth << " " << c.height << "\n";
  return io;
}

void Cylinder::Dump()
{
  cout << "Cylinder " << radius << " " << depth << " " << height << " " << arcmin << " " << arcmax << "\n";
}

//=======================================
// DoDice
//---------------------------------------
void Cylinder::DoDice(MicroGrid &microgrid,int us,int vs)
{
  int u,v;
  float uang,vz;
  Vector3 n;
  Point3 p;
  Matrix4 transform,ntransform;

  transform=RiGlobal.tWorldToCamera*RiCurrent.transform;
  ntransform=transform;
  ntransform[0][3]=0;
  ntransform[1][3]=0;
  ntransform[2][3]=0;

  microgrid.SetSize(us,vs);
  microgrid.SetTextureCoords(arcmin/360,tmin,arcmax/360,tmax);
  for(u=0;u<=us;u++)
  {
    uang=DEGTORAD*(arcmin+((arcmax-arcmin)*u)/us);
    for(v=0;v<=vs;v++)
    {
      vz=depth+((height-depth)*v)/vs;
      p.x=radius*sin(uang);
      p.y=radius*cos(uang);
      p.z=vz;
      microgrid.point[u][v]=transform*p;
      p.z=0;
      microgrid.normal[u][v]=ntransform*p;
    }
  }
}

//=======================================
// Splitable
//---------------------------------------
bool Cylinder::Splitable()
{
  return TRUE;
}

//=======================================
// Split
//---------------------------------------
void Cylinder::Split(list<Primitive*> &primlist)
{
  float zcent=0.5*(height+depth);
  float arccent=0.5*(arcmax+arcmin);
  float tcent=0.5*(tmin+tmax);

  primlist.push_front(new Cylinder(radius,depth,zcent,arcmin,arccent,tmin,tcent));
  primlist.push_front(new Cylinder(radius,depth,zcent,arccent,arcmax,tmin,tcent));
  primlist.push_front(new Cylinder(radius,zcent,height,arcmin,arccent,tcent,tmax));
  primlist.push_front(new Cylinder(radius,zcent,height,arccent,arcmax,tcent,tmax));
}

//=======================================
// TransformToEyeSpace
//---------------------------------------
bool Cylinder::TransformToEyeSpace(Matrix4 t_position,Matrix4 t_vector)
{
  return TRUE;
}

//=======================================
// EyeBound
//---------------------------------------
bool Cylinder::EyeBound(BoundBox3 &bb)
{
  return FALSE;
}
