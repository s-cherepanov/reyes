/*------------------------------------------------------
 * TITLE:  polygon.cpp
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Polygon primitive class
 * (C)     Copyright $Date: 1998/12/10 20:10:01 $
 * $Log: polygon.cpp,v $
 * Revision 1.4  1998/12/10 20:10:01  ady
 * Now uses Ri structures
 *
 * Revision 1.3  1997/11/29 18:02:41  ady
 * Uses new stdc list class
 *
 * Revision 1.2  1997/11/21 20:17:43  ady
 * Made consistent with new displacement shaders
 *
 * Revision 1.1  1997/11/16 14:18:48  ady
 * Initial revision
 *
 * Revision 1.3  1997/08/28 00:05:57  ady
 * Fixed polygon splitting and added EyeBound()
 *
 * Revision 1.2  1997/03/10 22:00:39  ady
 * New dice and split structure
 *
 * Revision 1.1  1997/02/23 22:55:40  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// C includes
#include <math.h>
#include <stdlib.h>

// Misc includes
#include "list.h"

// Reyes includes
#include "ri.h"
#include "useful.h"
#include "noise.h"
#include "reyes.h"
#include "primitive.h"
#include "polygon.h"
#include "point3.h"
#include "boundbox3.h"

//=======================================
// Constructors
//---------------------------------------
Polygon::Polygon(Point3 p1,Point3 p2,Point3 p3,Point3 p4)
{
  p[0]=p1;
  p[1]=p2;
  p[2]=p3;
  p[3]=p4;
  smin=0;
  tmin=0;
  smax=1;
  tmax=1;
}


//=======================================
//========STREAM INPUT/OUTPUT============
//=======================================

ostream &operator<<(ostream &io,const Polygon &p)
{
  io.setf(ios::showpoint+ios::right+ios::fixed);
  io << setprecision(2) << p.p[0];
  io << "-";
  io << setprecision(2) << p.p[1];
  io << "-";
  io << setprecision(2) << p.p[2];
  io << "-";
  io << setprecision(2) << p.p[3];
  io << "[" << p.smin << "," << p.tmin << "]";
  io << "-[" << p.smax << "," << p.tmax << "]";
  return io;
}

void Polygon::Dump()
{
  cout << setprecision(2) << p[0];
  cout << "-";
  cout << setprecision(2) << p[1];
  cout << "-";
  cout << setprecision(2) << p[2];
  cout << "-";
  cout << setprecision(2) << p[3];
  cout << "[" << smin << "," << tmin << "]";
  cout << "-[" << smax << "," << tmax << "]";
}

//=======================================
// Dice
//---------------------------------------
bool Polygon::Diceable(float vpscale)
{
  int u,v;
  int us,vs;
  float x0,y0,x1,y1,x2,y2,x3,y3,d0,d1,zmin,zmax;

  zmin=min(min(p[0].z,p[1].z),min(p[2].z,p[3].z));
  zmax=max(max(p[0].z,p[1].z),max(p[2].z,p[3].z));

  x0=vpscale*p[0].x/p[0].z; y0=vpscale*p[0].y/p[0].z;
  x1=vpscale*p[1].x/p[1].z; y1=vpscale*p[1].y/p[1].z;
  x2=vpscale*p[2].x/p[2].z; y2=vpscale*p[2].y/p[2].z;
  x3=vpscale*p[3].x/p[3].z; y3=vpscale*p[3].y/p[3].z;

  d0=sqrt((x1-x0)*(x1-x0)+(y1-y0)*(y1-y0));
  d1=sqrt((x3-x0)*(x3-x0)+(y3-y0)*(y3-y0));

  us=MAX((int)(d0/RiCurrent.shadingAttributes.shadingRate),1);
  vs=MAX((int)(d1/RiCurrent.shadingAttributes.shadingRate),1);

  if ((us*vs<=RiGlobal.options.maxMicroGridSize)&&(zmax<2*zmin))
    return TRUE;
  else
    return FALSE;
}

//=======================================
// Dice
//---------------------------------------
void Polygon::Dice(MicroGrid &microgrid,float vpscale)
{
  int u,v;
  int us,vs;
  Vector3 a,b;
  float x0,y0,x1,y1,x2,y2,x3,y3,d0,d1;

  x0=vpscale*p[0].x/p[0].z; y0=vpscale*p[0].y/p[0].z;
  x1=vpscale*p[1].x/p[1].z; y1=vpscale*p[1].y/p[1].z;
  x2=vpscale*p[2].x/p[2].z; y2=vpscale*p[2].y/p[2].z;
  x3=vpscale*p[3].x/p[3].z; y3=vpscale*p[3].y/p[3].z;

  d0=sqrt((x1-x0)*(x1-x0)+(y1-y0)*(y1-y0));
  d1=sqrt((x3-x0)*(x3-x0)+(y3-y0)*(y3-y0));

  us=MAX((int)(d0/reyesoptions.micropolygonsize),1);
  vs=MAX((int)(d1/reyesoptions.micropolygonsize),1);

  microgrid.SetSize(us,vs);
  microgrid.SetTextureCoords(smin,tmin,smax,tmax);
  a=p[1]-p[0];
  b=p[3]-p[0];
  for(u=0;u<=us;u++)
  {
    for(v=0;v<=vs;v++)
    {
      microgrid.point[u][v]=p[0]+((float)u/us)*a+((float)v/vs)*b;
    }
  }
}

//=======================================
// Splitable
//---------------------------------------
bool Polygon::Splitable()
{
  return TRUE;
}

//=======================================
// Split
//---------------------------------------
void Polygon::Split(list<Primitive*> &primlist)
{
  Point3 a,b,c,d,cent;
  Polygon *p1,*p2,*p3,*p4;
  float scent,tcent;

  a=(p[0]+p[1])/2;
  b=(p[2]+p[3])/2;
  c=(p[0]+p[3])/2;
  d=(p[1]+p[2])/2;
  cent=(p[0]+p[1]+p[2]+p[3])/4;
  scent=(smin+smax)/2;
  tcent=(tmin+tmax)/2;
  p1=new Polygon(p[0],a,cent,c); p1->SetTextureCoords(smin,tmin,scent,tcent);
  p2=new Polygon(a,p[1],d,cent); p2->SetTextureCoords(scent,tmin,smax,tcent);
  p3=new Polygon(c,cent,b,p[3]); p3->SetTextureCoords(smin,tcent,scent,tmax);
  p4=new Polygon(cent,d,p[2],b); p4->SetTextureCoords(scent,tcent,smax,tmax);
  primlist.push_front(p1);
  primlist.push_front(p2);
  primlist.push_front(p3);
  primlist.push_front(p4);
}

//=======================================
// TransformToEyeSpace
//---------------------------------------
bool Polygon::TransformToEyeSpace(Matrix4 t_position,Matrix4 t_vector)
{
  int i;

  for(i=0;i<4;i++)
    p[i]=t_position*p[i];
  return TRUE;
}

//=======================================
// EyeBound
//---------------------------------------
bool Polygon::EyeBound(BoundBox3 &bb)
{
  bb.min.x=min(min(p[0].x,p[1].x),min(p[2].x,p[3].x));
  bb.min.y=min(min(p[0].y,p[1].y),min(p[2].y,p[3].y));
  bb.min.z=min(min(p[0].z,p[1].z),min(p[2].z,p[3].z));
  bb.max.x=max(max(p[0].x,p[1].x),max(p[2].x,p[3].x));
  bb.max.y=max(max(p[0].y,p[1].y),max(p[2].y,p[3].y));
  bb.max.z=max(max(p[0].z,p[1].z),max(p[2].z,p[3].z));
  return TRUE;
}
