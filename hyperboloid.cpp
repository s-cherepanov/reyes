/*------------------------------------------------------
 * TITLE:  hyperboloid.cpp
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Hyperboloid primitive class
 * (C)     Copyright $Date: 1998/12/10 20:07:25 $
 * $Log: hyperboloid.cpp,v $
 * Revision 1.2  1998/12/10 20:07:25  ady
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
#include "hyperboloid.h"
#include "patch.h"
#include "point3.h"
#include "matrix4.h"
#include "useful.h"

//=======================================
// Constructors
//---------------------------------------
Hyperboloid::Hyperboloid(Point3 point1,Point3 point2,float thetamin,float thetamax,float tmin,float tmax)
{
  this->point1=point1;
  this->point2=point2;
  this->thetamin=thetamin;
  this->thetamax=thetamax;
  this->tmin=tmin;
  this->tmax=tmax;
}

//=======================================
//========STREAM INPUT/OUTPUT============
//=======================================

ostream &operator<<(ostream &io,const Hyperboloid &s)
{
  io << "Hyperboloid " << s.point1 << " " << s.point2 << " " << s.thetamin << "-" << s.thetamax << "\n";
  return io;
}

void Hyperboloid::Dump()
{
  cout << *this;
}

//=======================================
// DoDice
//---------------------------------------
void Hyperboloid::DoDice(MicroGrid &microgrid,int us,int vs)
{
  Point3 p1,p2,p;
  int u,v;
  float uu,vv;
  float radius;
  Matrix4 transform,ntransform;

  transform=RiGlobal.tWorldToCamera*RiCurrent.transform;
  ntransform=transform;
  ntransform[0][3]=0;
  ntransform[1][3]=0;
  ntransform[2][3]=0;
  radius=MAX(point1.x*point1.x+point1.y*point1.y,point2.x*point2.x+point2.y*point2.y);
  radius=sqrt(radius);
  microgrid.SetSize(us,vs);
  microgrid.SetTextureCoords(thetamin/360,tmin,thetamax/360,tmax);
  for(u=0;u<=us;u++)
  {
    uu=(float)u/us;
    uu=DEGTORAD*(thetamin+uu*(thetamax-thetamin));
    p1.x=point1.x;
    p1.y=point1.y;
    p1.z=point1.z;
    p2.x=point2.x;
    p2.y=point2.y;
    p2.z=point2.z;
    for(v=0;v<=vs;v++)
    {
      vv=(float)v/vs;
      p=p1*(1-vv)+p2*vv;
      microgrid.point[u][v].x=p.x*cos(-uu)+p.y*sin(-uu);
      microgrid.point[u][v].y=p.x*sin(-uu)-p.y*cos(-uu);
      microgrid.point[u][v].z=p.z;
      microgrid.point[u][v]=transform*microgrid.point[u][v];
    }
  }
}

//=======================================
// Splitable
//---------------------------------------
bool Hyperboloid::Splitable()
{
  return TRUE;
}

//=======================================
// Split
//---------------------------------------
void Hyperboloid::Split(list<Primitive*> &primlist)
{
  Point3 pcent=(point1+point2)/2;
  float thetacent=(thetamin+thetamax)/2;
  float tcent=0.5*(tmin+tmax);

  primlist.push_front(new Hyperboloid(point1,pcent,thetamin,thetacent,tmin,tcent));
  primlist.push_front(new Hyperboloid(point1,pcent,thetacent,thetamax,tmin,tcent));
  primlist.push_front(new Hyperboloid(pcent,point2,thetamin,thetacent,tcent,tmax));
  primlist.push_front(new Hyperboloid(pcent,point2,thetacent,thetamax,tcent,tmax));
}

//=======================================
// TransformToEyeSpace
//---------------------------------------
bool Hyperboloid::TransformToEyeSpace(Matrix4 t_position,Matrix4 t_vector)
{
  return TRUE;
}

//=======================================
// EyeBound
//---------------------------------------
bool Hyperboloid::EyeBound(BoundBox3 &bb)
{
  return FALSE;
}
