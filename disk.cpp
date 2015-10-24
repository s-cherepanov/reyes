/*------------------------------------------------------
 * TITLE:  disk.cpp
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Disk primitive class
 * (C)     Copyright $Date: 1998/12/10 20:06:13 $
 * $Log: disk.cpp,v $
 * Revision 1.5  1998/12/10 20:06:13  ady
 * Changed dicing
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
#include "disk.h"
#include "patch.h"
#include "point3.h"
#include "matrix4.h"
#include "useful.h"

//=======================================
// Constructors
//---------------------------------------
Disk::Disk(float z,float radius,float arcmin,float arcmax,float tmin,float tmax)
{
  this->z=z;
  this->radius=radius;
  this->arcmin=arcmin;
  this->arcmax=arcmax;
  this->tmin=tmin;
  this->tmax=tmax;
}

//=======================================
//========STREAM INPUT/OUTPUT============
//=======================================

ostream &operator<<(ostream &io,const Disk &d)
{
  io.setf(ios::showpoint+ios::right+ios::fixed);
  io << "Disk " << d.z << " " << d.radius << " " << d.arcmin << "-" << d.arcmax << "\n";
  return io;
}

void Disk::Dump()
{
  cout << *this;
}

//=======================================
// Dice
//---------------------------------------
void Disk::DoDice(MicroGrid &microgrid,int us,int vs)
{
  int u,v;
  float uang,r;
  float t;
  Matrix4 transform;

  transform=RiGlobal.tWorldToCamera*RiCurrent.transform;
  microgrid.SetSize(us,vs);
  microgrid.SetTextureCoords(arcmin/360,tmin,arcmax/360,tmax);
  for(u=0;u<=us;u++)
  {
    uang=DEGTORAD*(arcmin+((arcmax-arcmin)*u)/us);
    for(v=0;v<=vs;v++)
    {
      t=tmin+(tmax-tmin)*v/vs;
      r=radius*t;
      microgrid.point[u][v].x=r*sin(uang);
      microgrid.point[u][v].y=r*cos(uang);
      microgrid.point[u][v].z=z;
      microgrid.point[u][v]=transform*microgrid.point[u][v];
    }
  }
}

//=======================================
// Splitable
//---------------------------------------
bool Disk::Splitable()
{
  return TRUE;
}

//=======================================
// Split
//---------------------------------------
void Disk::Split(list<Primitive*> &primlist)
{
  float arccent=0.5*(arcmax+arcmin);
  float tcent=0.5*(tmin+tmax);

  primlist.push_front(new Disk(z,radius,arcmin,arccent,tmin,tcent));
  primlist.push_front(new Disk(z,radius,arccent,arcmax,tmin,tcent));
  primlist.push_front(new Disk(z,radius,arcmin,arccent,tcent,tmax));
  primlist.push_front(new Disk(z,radius,arccent,arcmax,tcent,tmax));
}

//=======================================
// TransformToEyeSpace
//---------------------------------------
bool Disk::TransformToEyeSpace(Matrix4 t_position,Matrix4 t_vector)
{
  return TRUE;
}

//=======================================
// EyeBound
//---------------------------------------
bool Disk::EyeBound(BoundBox3 &bb)
{
  return FALSE;
}
