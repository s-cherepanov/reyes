/*------------------------------------------------------
 * TITLE:  sphere.cpp
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Sphere primitive class
 * (C)     Copyright $Date: 1999/04/01 19:59:59 $
 * $Log: sphere.cpp,v $
 * Revision 1.6  1999/04/01 19:59:59  ady
 * *** empty log message ***
 *
 * Revision 1.5  1998/12/10 20:12:15  ady
 * Changed spliting
 *
 * Revision 1.4  1997/11/29 18:07:27  ady
 * New dicing and estimate gridsize method
 *
 * Revision 1.3  1997/11/23 15:26:03  ady
 * Change title in header
 *
 * Revision 1.2  1997/11/21 20:17:43  ady
 * Reorganisation so that Zmin,Zmax and arc values could usefully be used.
 * Now dices sphere segments directly rather than converting to patches
 *
 * Revision 1.1  1997/11/16 14:18:48  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// C includes
#include <math.h>

// Reyes includes
#include "ri.h"
#include "smath.h"
#include "sphere.h"
#include "patch.h"
#include "point3.h"
#include "matrix4.h"
#include "useful.h"

//=======================================
// Constructors
//---------------------------------------
Sphere::Sphere(float radius,float zmin,float zmax,float arcmin,float arcmax)
{
  this->radius=radius;
  this->zmin=zmin;
  this->zmax=zmax;
  this->arcmin=arcmin;
  this->arcmax=arcmax;
}

//=======================================
//========STREAM INPUT/OUTPUT============
//=======================================

ostream &operator<<(ostream &io,const Sphere &s)
{
  io << "Sphere " << s.radius << " " << s.zmin << " " << s.zmax << " " << s.arcmin << "-" << s.arcmax << "\n";
  return io;
}

void Sphere::Dump()
{
  cout << "Sphere " << radius << " " << zmin << " " << zmax << " " << arcmin << "-" << arcmax << "\n";
}

//=======================================
// Dice
//---------------------------------------
void Sphere::DoDice(MicroGrid &microgrid,int us,int vs)
{
  int u,v;
  float uang,vang;
  float vangmin,vangmax;
  Matrix4 transform; // 3D space transform
  Matrix4 ntransform; // normal transform (doesn't include translate)
  Point3 p;

  transform=RiGlobal.tWorldToCamera*RiCurrent.transform;
  ntransform=transform;
  ntransform[0][3]=0;
  ntransform[1][3]=0;
  ntransform[2][3]=0;

  microgrid.SetSize(us,vs);
  vangmin=asin(zmin/radius);
  vangmax=asin(zmax/radius);
  microgrid.SetTextureCoords(arcmin/360,vangmin/(2*PI),arcmax/360,vangmax/(2*PI));
  for(u=0;u<=us;u++)
  {
    uang=DEGTORAD*(arcmin+((arcmax-arcmin)*u)/us);
    for(v=0;v<=vs;v++)
    {
      vang=vangmin+((vangmax-vangmin)*v)/vs;
      p.x=radius*sin(uang)*cos(vang);
      p.y=radius*cos(uang)*cos(vang);
      p.z=radius*sin(vang);
      microgrid.point[u][v]=transform*p;
      microgrid.normal[u][v]=ntransform*p;
      microgrid.normal[u][v].Normalise();
    }
  }
}

//=======================================
// Splitable
//---------------------------------------
bool Sphere::Splitable()
{
  return TRUE;
}

//=======================================
// Split
//---------------------------------------
void Sphere::Split(list<Primitive*> &primlist)
{
  float zcent=(zmin+zmax)/2;
  float arccent=(arcmin+arcmax)/2;

  primlist.push_front(new Sphere(radius,zmin,zcent,arcmin,arccent));
  primlist.push_front(new Sphere(radius,zmin,zcent,arccent,arcmax));
  primlist.push_front(new Sphere(radius,zcent,zmax,arcmin,arccent));
  primlist.push_front(new Sphere(radius,zcent,zmax,arccent,arcmax));
}

//=======================================
// TransformToEyeSpace
//---------------------------------------
bool Sphere::TransformToEyeSpace(Matrix4 t_position,Matrix4 t_vector)
{
  return TRUE;
}

//=======================================
// EyeBound
//---------------------------------------
bool Sphere::EyeBound(BoundBox3 &bb)
{
  return FALSE;
}
