/*------------------------------------------------------
 * TITLE:  torus.cpp
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Torus primitive class
 * (C)     Copyright $Date: 1998/12/10 20:12:54 $
 * $Log: torus.cpp,v $
 * Revision 1.2  1998/12/10 20:12:54  ady
 * Changed dicing
 *
 * Revision 1.1  1997/11/29 18:07:27  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// C includes
#include <math.h>

// Reyes includes
#include "ri.h"
#include "smath.h"
#include "torus.h"
#include "patch.h"
#include "point3.h"
#include "matrix4.h"
#include "useful.h"

//=======================================
// Constructors
//---------------------------------------
Torus::Torus(float major_radius,float minor_radius,float phimin,float phimax,float arcmin,float arcmax)
{
  this->major_radius=major_radius;
  this->minor_radius=minor_radius;
  this->phimin=phimin;
  this->phimax=phimax;
  this->arcmin=arcmin;
  this->arcmax=arcmax;
}

//=======================================
//========STREAM INPUT/OUTPUT============
//=======================================

ostream &operator<<(ostream &io,const Torus &s)
{
  io << "Torus " << s.major_radius << " " << s.minor_radius << " " << s.phimin << " " << s.phimax << " [" << s.arcmin << ":" << s.arcmax << "]\n";
  return io;
}

void Torus::Dump()
{
  cout << *this;
}

//=======================================
// DoDice
//---------------------------------------
void Torus::DoDice(MicroGrid &microgrid,int us,int vs)
{
  int u,v;
  float uang,vang,tx,tz;

  microgrid.SetSize(us,vs);
  microgrid.SetTextureCoords(arcmin/360,phimin/360,arcmax/360,phimax/360);
  for(u=0;u<=us;u++)
  {
    uang=DEGTORAD*(arcmin+((arcmax-arcmin)*u)/us);
    for(v=0;v<=vs;v++)
    {
      vang=DEGTORAD*(phimin+((phimax-phimin)*v)/vs);
      tx=major_radius+minor_radius*sin(vang);
      tz=minor_radius*cos(vang);
      microgrid.point[u][v].x=tx*cos(uang);
      microgrid.point[u][v].y=tx*sin(uang);
      microgrid.point[u][v].z=tz;
      microgrid.point[u][v]=RiCurrent.transform*microgrid.point[u][v];
    }
  }
}

//=======================================
// Splitable
//---------------------------------------
bool Torus::Splitable()
{
  return TRUE;
}

//=======================================
// Split
//---------------------------------------
void Torus::Split(list<Primitive*> &primlist)
{
  float phicent=(phimin+phimax)/2;
  float arccent=(arcmin+arcmax)/2;

  primlist.push_front(new Torus(major_radius,minor_radius,phimin,phicent,arcmin,arccent));
  primlist.push_front(new Torus(major_radius,minor_radius,phimin,phicent,arccent,arcmax));
  primlist.push_front(new Torus(major_radius,minor_radius,phicent,phimax,arcmin,arccent));
  primlist.push_front(new Torus(major_radius,minor_radius,phicent,phimax,arccent,arcmax));
}

//=======================================
// TransformToEyeSpace
//---------------------------------------
bool Torus::TransformToEyeSpace(Matrix4 t_position,Matrix4 t_vector)
{
  return TRUE;
}

//=======================================
// EyeBound
//---------------------------------------
bool Torus::EyeBound(BoundBox3 &bb)
{
  return FALSE;
}
