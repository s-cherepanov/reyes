/*------------------------------------------------------
 * TITLE:  patch.cpp
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Bicubic patch primitive class
 * (C)     Copyright $Date: 1998/12/10 20:09:25 $
 * $Log: patch.cpp,v $
 * Revision 1.4  1998/12/10 20:09:25  ady
 * Now uses Ri structures
 *
 * Revision 1.3  1997/11/29 18:02:41  ady
 * Uses new MIN and MAX functions
 *
 * Revision 1.2  1997/11/18 21:41:26  ady
 * Removed Bezier functions (now in useful.h)
 *
 * Revision 1.1  1997/11/16 14:18:48  ady
 * Initial revision
 *
 * Revision 1.1  1997/08/28 00:05:57  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// C includes
#include <math.h>

// C++ includes

// Reyes includes
#include "ri.h"
#include "point3.h"
#include "patch.h"
#include "useful.h"

//=======================================
// SplitBezier1D
//---------------------------------------
//  Split a bezier segment of the form
//    p1 - p2 - p3 - p4
//  into two joined segments with a midpoint at t=0.5
//  The new points are returned as an 7 points
//  (the end of the first segment and start of the 2nd are
//   coincident)
void SplitBezier1D(Point3 p1,Point3 p2,Point3 p3,Point3 p4,
                   Point3 &n1,Point3 &n2,Point3 &n3,Point3 &n4,Point3 &n5,Point3 &n6,Point3 &n7)
{
  n1=p1;
  n2=0.5*p1+0.5*p2;
  n3=0.25*p1+0.5*p2+0.25*p3;
  n4=0.125*p1+0.375*p2+0.375*p3+0.125*p4;
  n5=0.25*p2+0.5*p3+0.25*p4;
  n6=0.5*p3+0.5*p4;
  n7=p4;
}

//=======================================
// EstimateQuadGridSize
//---------------------------------------
void EstimateQuadGridSize(Point3 p1,Point3 p2,Point3 p3,Point3 p4,float vpscale,int &us,int &vs)
{
  float x0,y0,x1,y1,x2,y2,x3,y3,d0,d1,zmin,zmax;

  x0=vpscale*p1.x/p1.z; y0=vpscale*p1.y/p1.z;
  x1=vpscale*p2.x/p2.z; y1=vpscale*p2.y/p2.z;
  x2=vpscale*p3.x/p3.z; y2=vpscale*p3.y/p3.z;
  x3=vpscale*p4.x/p4.z; y3=vpscale*p4.y/p4.z;

  zmin=MIN(MIN(p1.z,p2.z),MIN(p3.z,p4.z));
  zmax=MAX(MAX(p1.z,p2.z),MAX(p3.z,p4.z));

  d0=sqrt((x1-x0)*(x1-x0)+(y1-y0)*(y1-y0));
  d1=sqrt((x3-x0)*(x3-x0)+(y3-y0)*(y3-y0));

  us=MAX((int)(d0/RiCurrent.shadingAttributes.shadingRate),1);
  vs=MAX((int)(d1/RiCurrent.shadingAttributes.shadingRate),1);
}

//=======================================
// Constructor
//---------------------------------------
Patch::Patch(Point3 control[4][4])
{
  int i,j;

  for(i=0;i<4;i++)
    for(j=0;j<4;j++)
      this->control[i][j]=control[i][j];
  smin=0;
  tmin=0;
  smax=1;
  tmax=1;
}

Patch::Patch(Point3 c00,Point3 c10,Point3 c20,Point3 c30,
             Point3 c01,Point3 c11,Point3 c21,Point3 c31,
             Point3 c02,Point3 c12,Point3 c22,Point3 c32,
             Point3 c03,Point3 c13,Point3 c23,Point3 c33)
{
  this->control[0][0]=c00;
  this->control[0][1]=c01;
  this->control[0][2]=c02;
  this->control[0][3]=c03;
  this->control[1][0]=c10;
  this->control[1][1]=c11;
  this->control[1][2]=c12;
  this->control[1][3]=c13;
  this->control[2][0]=c20;
  this->control[2][1]=c21;
  this->control[2][2]=c22;
  this->control[2][3]=c23;
  this->control[3][0]=c30;
  this->control[3][1]=c31;
  this->control[3][2]=c32;
  this->control[3][3]=c33;
  smin=0;
  tmin=0;
  smax=1;
  tmax=1;
}

//=======================================
//========STREAM INPUT/OUTPUT============
//=======================================

ostream &operator<<(ostream &io,const Patch &p)
{
  int s,t;

  io.setf(ios::showpoint+ios::right+ios::fixed);
  for(t=0;t<4;t++)
  {
    for(s=0;s<4;s++)
    {
      io << p.control[s][t];
    }
    io << "\n";
  }
  return io;
}

void Patch::Dump()
{
  cout << *this;
}

//=======================================
// EstimateGridSize
//---------------------------------------
void Patch::EstimateGridSize(float vpscale,int &us,int &vs)
{
  int us11,vs11,us12,vs12,us13,vs13,
      us21,vs21,us22,vs22,us23,vs23,
      us31,vs31,us32,vs32,us33,vs33;

  EstimateQuadGridSize(control[0][0],control[0][1],control[1][1],control[1][0],vpscale,us11,vs11);
  EstimateQuadGridSize(control[1][0],control[1][1],control[2][1],control[2][0],vpscale,us21,vs21);
  EstimateQuadGridSize(control[2][0],control[2][1],control[3][1],control[3][0],vpscale,us31,vs31);

  EstimateQuadGridSize(control[0][1],control[0][2],control[1][2],control[1][1],vpscale,us12,vs12);
  EstimateQuadGridSize(control[1][1],control[1][2],control[2][2],control[2][1],vpscale,us22,vs22);
  EstimateQuadGridSize(control[2][1],control[2][2],control[3][2],control[3][1],vpscale,us32,vs32);

  EstimateQuadGridSize(control[0][2],control[0][3],control[1][3],control[1][2],vpscale,us13,vs13);
  EstimateQuadGridSize(control[1][2],control[1][3],control[2][3],control[2][2],vpscale,us23,vs23);
  EstimateQuadGridSize(control[2][2],control[2][3],control[3][3],control[3][2],vpscale,us33,vs33);

  us=MAX(MAX(MAX(MAX(us11,us12),MAX(us13,us21)),MAX(MAX(us22,us23),MAX(us31,us32))),us33);
  vs=MAX(MAX(MAX(MAX(vs11,vs12),MAX(vs13,vs21)),MAX(MAX(vs22,vs23),MAX(vs31,vs32))),vs33);

  us=3*us; vs=3*vs;
}

//=======================================
// Diceable
//---------------------------------------
bool Patch::Diceable(float vpscale)
{
  int us,vs;

  EstimateGridSize(vpscale,us,vs);
  if (us*vs<=reyesoptions.maxmicrogridsize)
    return TRUE;
  else
    return FALSE;
}

//=======================================
// Dice
//---------------------------------------
void Patch::Dice(MicroGrid &microgrid,float vpscale)
{
  int u,v;
  int us,vs;
  float s,t;

  EstimateGridSize(vpscale,us,vs);
  microgrid.SetSize(us,vs);
  for(u=0;u<=us;u++)
  {
    s=(float)u/us;
    for(v=0;v<=vs;v++)
    {
      t=(float)v/vs;
      microgrid.point[u][v]=Bezier2D(control[0][0],control[1][0],control[2][0],control[3][0],
                                     control[0][1],control[1][1],control[2][1],control[3][1],
                                     control[0][2],control[1][2],control[2][2],control[3][2],
                                     control[0][3],control[1][3],control[2][3],control[3][3],s,t);
    }
  }
  microgrid.SetTextureCoords(smin,tmin,smax,tmax);
}

//=======================================
// Splitable
//---------------------------------------
bool Patch::Splitable()
{
  return TRUE;
}

//=======================================
// Split
//---------------------------------------
void Patch::Split(list<Primitive*> &primlist)
{
  int s,t,ss,tt;
  Point3 o[4][7],p[7][7];
  Patch *patch;

  for(s=0;s<4;s++)
  {
    SplitBezier1D(control[s][0],control[s][1],control[s][2],control[s][3],
                  o[s][0],o[s][1],o[s][2],o[s][3],o[s][4],o[s][5],o[s][6]);
  }
  for(t=0;t<7;t++)
  {
    SplitBezier1D(o[0][t],o[1][t],o[2][t],o[3][t],
                  p[0][t],p[1][t],p[2][t],p[3][t],p[4][t],p[5][t],p[6][t]);
  }

  for(ss=0;ss<2;ss++)
  {
    for(tt=0;tt<2;tt++)
    {
      s=ss*3;
      t=tt*3;
      patch=new Patch(p[s+0][t+0],p[s+1][t+0],p[s+2][t+0],p[s+3][t+0],
                      p[s+0][t+1],p[s+1][t+1],p[s+2][t+1],p[s+3][t+1],
                      p[s+0][t+2],p[s+1][t+2],p[s+2][t+2],p[s+3][t+2],
                      p[s+0][t+3],p[s+1][t+3],p[s+2][t+3],p[s+3][t+3]);
      patch->SetTextureCoords(smin+(ss/2.0)*(smax-smin),tmin+(tt/2.0)*(tmax-tmin),
                              smin+((ss+1)/2.0)*(smax-smin),tmin+((tt+1)/2.0)*(tmax-tmin));
      primlist.push_front(patch);
    }
  }
}


//=======================================
// TransformToEyeSpace
//---------------------------------------
bool Patch::TransformToEyeSpace(Matrix4 t_position,Matrix4 t_vector)
{
  Patch *p;
  int i,j;

  for(i=0;i<4;i++)
    for(j=0;j<4;j++)
      control[i][j]=t_position*control[i][j];
  return TRUE;
}

//=======================================
// EyeBound
//---------------------------------------
bool Patch::EyeBound(BoundBox3 &bb)
{
  int s,t;
  bb.min=control[0][0];
  bb.max=control[0][0];

  for(s=0;s<4;s++)
  {
    for(t=0;t<4;t++)
    {
      bb.min.x=MIN(bb.min.x,control[s][t].x);
      bb.min.y=MIN(bb.min.y,control[s][t].y);
      bb.min.z=MIN(bb.min.z,control[s][t].z);
      bb.max.x=MAX(bb.max.x,control[s][t].x);
      bb.max.y=MAX(bb.max.y,control[s][t].y);
      bb.max.z=MAX(bb.max.z,control[s][t].z);
    }
  }
  return TRUE;
}
