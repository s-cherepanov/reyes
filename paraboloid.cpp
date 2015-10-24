/*------------------------------------------------------
 * TITLE:  paraboloid.cpp
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Paraboloid primitive class
 * (C)     Copyright $Date: 1998/12/10 20:09:03 $
 * $Log: paraboloid.cpp,v $
 * Revision 1.2  1998/12/10 20:09:03  ady
 * Added dicing
 *
 * Revision 1.1  1997/11/29 18:02:41  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// C includes
#include <math.h>

// Reyes includes
#include "ri.h"
#include "smath.h"
#include "paraboloid.h"
#include "patch.h"
#include "point3.h"
#include "matrix4.h"
#include "useful.h"

//=======================================
// Constructors
//---------------------------------------
Paraboloid::Paraboloid(float rmax,float zmin,float zmax,float thetamin,float thetamax,float tmin,float tmax)
{
  this->rmax=rmax;
  this->zmin=zmin;
  this->zmax=zmax;
  this->thetamin=thetamin;
  this->thetamax=thetamax;
  this->tmin=tmin;
  this->tmax=tmax;
}

//=======================================
//========STREAM INPUT/OUTPUT============
//=======================================

ostream &operator<<(ostream &io,const Paraboloid &s)
{
  io << "Paraboloid " << s.rmax << " " << s.zmin << " " << s.zmax << " " << s.thetamin << "-" << s.thetamax << "\n";
  return io;
}

void Paraboloid::Dump()
{
  cout << *this;
}

//=======================================
// DoDice
//---------------------------------------
void Paraboloid::DoDice(MicroGrid &microgrid,int us,int vs)
{
  int u,v;
  float r,z,uu,vv;
  Matrix4 transform;

  transform=RiGlobal.tWorldToCamera*RiCurrent.transform;
  microgrid.SetSize(us,vs);
  microgrid.SetTextureCoords(thetamin/360,tmin,thetamax/360,tmax);
  for(u=0;u<=us;u++)
  {
    uu=(float)u/us;
    uu=DEGTORAD*(thetamin+uu*(thetamax-thetamin));
    for(v=0;v<=vs;v++)
    {
      vv=(float)v/vs;
      z=zmin+vv*(zmax-zmin);
      r=rmax*sqrt(z/zmax);
      microgrid.point[u][v].x=r*cos(uu);
      microgrid.point[u][v].y=r*sin(uu);
      microgrid.point[u][v].z=z;
      microgrid.point[u][v]=transform*microgrid.point[u][v];
    }
  }
}

//=======================================
// Splitable
//---------------------------------------
bool Paraboloid::Splitable()
{
  return TRUE;
}

//=======================================
// Split
//---------------------------------------
void Paraboloid::Split(list<Primitive*> &primlist)
{
  float zcent=(zmin+zmax)/2;
  float thetacent=(thetamin+thetamax)/2;
  float rcent=rmax*sqrt(zcent/zmax);
  float tcent=0.5*(tmin+tmax);

  primlist.push_front(new Paraboloid(rcent,zmin,zcent,thetamin,thetacent,tmin,tcent));
  primlist.push_front(new Paraboloid(rcent,zmin,zcent,thetacent,thetamax,tmin,tcent));
  primlist.push_front(new Paraboloid(rmax,zcent,zmax,thetamin,thetacent,tcent,tmax));
  primlist.push_front(new Paraboloid(rmax,zcent,zmax,thetacent,thetamax,tcent,tmax));
}

//=======================================
// TransformToEyeSpace
//---------------------------------------
bool Paraboloid::TransformToEyeSpace(Matrix4 t_position,Matrix4 t_vector)
{
  return TRUE;
}

//=======================================
// EyeBound
//---------------------------------------
bool Paraboloid::EyeBound(BoundBox3 &bb)
{
  return FALSE;
}
