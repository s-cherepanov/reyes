/*------------------------------------------------------
 * TITLE:  cone.cpp
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Cone primitive class
 * (C)     Copyright $Date: 1998/12/10 20:05:18 $
 * $Log: cone.cpp,v $
 * Revision 1.3  1998/12/10 20:05:18  ady
 * Added zmin and zmax arguments
 *
 * Revision 1.2  1997/11/29 18:02:41  ady
 * New dicing and estimate gridsize method
 *
 * Revision 1.1  1997/11/23 15:24:06  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// C includes
#include <math.h>

// Reyes includes
#include "ri.h"
#include "smath.h"
#include "cone.h"
#include "patch.h"
#include "point3.h"
#include "matrix4.h"
#include "useful.h"

//=======================================
// Constructors
//---------------------------------------
Cone::Cone(float height,float radius,float thetamin,float thetamax,float zmin,float zmax)
{
  this->height=height;
  this->radius=radius;
  this->zmin=zmin;
  this->zmax=zmax;
  this->thetamin=thetamin;
  this->thetamax=thetamax;
}

//=======================================
//========STREAM INPUT/OUTPUT============
//=======================================

ostream &operator<<(ostream &io,const Cone &d)
{
  io.setf(ios::showpoint+ios::right+ios::fixed);
  io << "Cone " << d.height << " " << d.radius << " " << d.thetamin << "-" << d.thetamax << " " << d.zmin << "-" << d.zmax << "\n";
  return io;
}

void Cone::Dump()
{
  cout << *this;
}

//=======================================
// Dice
//---------------------------------------
void Cone::DoDice(MicroGrid &microgrid,int us,int vs)
{
  int u,v;
  float r,uang,vv,hyp,z;
  float tmin,tmax;
  Matrix4 transform;

  transform=RiGlobal.tWorldToCamera*RiCurrent.transform;
  hyp=sqrt(radius*radius+height*height);
  tmin=hyp*zmin/height;
  tmax=hyp*zmax/height;
  microgrid.SetSize(us,vs);
  microgrid.SetTextureCoords(thetamin/360,tmin,thetamax/360,tmax);
  for(u=0;u<=us;u++)
  {
    uang=DEGTORAD*(thetamin+((thetamax-thetamin)*u)/us);
    for(v=0;v<=vs;v++)
    {
      vv=(float)v/vs;
      z=zmin+vv*(zmax-zmin);
      r=radius*(1-z/height);
      microgrid.point[u][v].x=r*sin(uang);
      microgrid.point[u][v].y=r*cos(uang);
      microgrid.point[u][v].z=z;
      microgrid.point[u][v]=RiGlobal.tWorldToCamera*RiCurrent.transform*microgrid.point[u][v];
    }
  }
}

//=======================================
// Splitable
//---------------------------------------
bool Cone::Splitable()
{
  return TRUE;
}

//=======================================
// Split
//---------------------------------------
void Cone::Split(list<Primitive*> &primlist)
{
  float zmid=0.5*(zmin+zmax);
  float thetacent=0.5*(thetamin+thetamax);

  primlist.push_back(new Cone(height,radius,thetamin,thetacent,zmin,zmid));
  primlist.push_back(new Cone(height,radius,thetacent,thetamax,zmin,zmid));
  primlist.push_back(new Cone(height,radius,thetamin,thetacent,zmid,zmax));
  primlist.push_back(new Cone(height,radius,thetacent,thetamax,zmid,zmax));
}

//=======================================
// TransformToEyeSpace
//---------------------------------------
bool Cone::TransformToEyeSpace(Matrix4 t_position,Matrix4 t_vector)
{
  return TRUE;
}

//=======================================
// EyeBound
//---------------------------------------
bool Cone::EyeBound(BoundBox3 &bb)
{
  return FALSE;
}
