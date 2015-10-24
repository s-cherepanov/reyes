/*------------------------------------------------------
 * TITLE:  micropoly.c++
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Micropolygon utilities
 * (C)     Copyright $Date: 1999/04/01 19:59:59 $
 * $Log: micropoly.cpp,v $
 * Revision 1.4  1999/04/01 19:59:59  ady
 * *** empty log message ***
 *
 * Revision 1.3  1998/12/10 20:08:48  ady
 * scaling now done elsewhere
 *
 * Revision 1.2  1997/11/29 18:02:41  ady
 * Now includes bool.h
 *
 * Revision 1.1  1997/11/16 14:15:10  ady
 * Initial revision
 *
 * Revision 1.3  1997/08/28 00:05:57  ady
 * Changed to reflect change of interface to ZBuffer
 *
 * Revision 1.2  1997/03/10 21:55:33  ady
 * Now uses jittering
 *
 * Revision 1.1  1997/02/23 22:55:40  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// C includes
#include <stdlib.h>

// Reyes includes
#include "ri.h"
#include "point3.h"
#include "micropoly.h"
#include "framebuffer.h"
#include "smath.h"
#include "jitter.h"
#include "useful.h"
#include "bool.h"

// C++ includes
#include "iostream.h"

//=======================================
// Constructors
//---------------------------------------
MicroPolygon::MicroPolygon(Point3 &p1,Point3 &p2,Point3 &p3,Point3 &p4,Colour &colour,Opacity &opacity)
{
  p[0]=p1;
  p[1]=p2;
  p[2]=p3;
  p[3]=p4;
  this->colour=colour;
  this->opacity=opacity;
}

MicroPolygon::MicroPolygon()
{
  p[0]=Point3(0,0,0);
  p[1]=Point3(0,0,0);
  p[2]=Point3(0,0,0);
  p[3]=Point3(0,0,0);
  colour=Colour(0,0,0);
  opacity=Opacity(1,1,1);
}

//=======================================
//========STREAM INPUT/OUTPUT============
//=======================================

ostream &operator<<(ostream &io,const MicroPolygon &m)
{
  io << "Vertices:" << m.p[0] << "," << m.p[1] << "," << m.p[2] << "," << m.p[3] << " Colour:" << m.colour;
  return io;
}

//=======================================
//========STOCHASTIC SAMPLING============
//=======================================


//=======================================
// Sample
//   Samples micro-polygon at given screen-space
//   co-ords (sx,sy). Return TRUE for a hit,
//   FALSE otherwise. If hit z is set and the
//   colour fields are set.
//---------------------------------------
bool MicroPolygon::Sample(float sx,float sy,float &z,Colour &colour,Opacity &opacity)
{
  float ax,ay,bx,by,r;
  int inside,i,ir;

  inside=0;
  for(i=0;i<4;i++)
  {
    // Calc edge vector
    ax=p[(i+1)%4].x-p[i].x;
    ay=p[(i+1)%4].y-p[i].y;

    // Calc vector from vertex to sample point
    bx=sx-p[i].x;
    by=sy-p[i].y;

    // Calc. side (a to left of b if +v.e and vice versa)
    r=ax*by-ay*bx;
    inside+=(r>=0)?1:0;
  }
  if (inside==4)
  {
    z=this->p[0].z;
    colour=this->colour;
    opacity=this->opacity;
    return TRUE;
  }
  else
    return FALSE;
}

//=======================================
// Rasterize
//---------------------------------------
void MicroPolygon::Rasterize(FrameBuffer &framebuffer)
{
  int xmin=(int)p[0].x,ymin=(int)p[0].y;
  int xmax=(int)(p[0].x+1),ymax=(int)(p[0].y+1);
  int i,x,y;
  float ax,ay,bx,by,z;
  Colour colour;
  Opacity opacity;
  Point3 t;
  float jx,jy;

  // Calculate screen-space bounds of micropolygon
  for(i=1;i<4;i++)
  {
    if ((int)p[i].x<xmin) xmin=(int)p[i].x;
    if ((int)(p[i].x+1)>xmax) xmax=(int)(p[i].x+1);
    if ((int)p[i].y<ymin) ymin=(int)p[i].y;
    if ((int)(p[i].y+1)>ymax) ymax=(int)(p[i].y+1);
  }

  // Do sampling
  if (RiGlobal.options.jitter)
  {
    for(x=xmin;x<=xmax;x++)
    {
      for(y=ymin;y<=ymax;y++)
      {
        Jitter1(x,y,0,&jx,&jy);
        jx+=x;
        jy+=y;
        if (this->Sample(jx,jy,z,colour,opacity))
	{
          framebuffer.WritePixel(x,y,z,colour,opacity);
	}
      }
    }
  }
  else
  {
    for(x=xmin;x<=xmax;x++)
    {
      for(y=ymin;y<=ymax;y++)
      {
        if (this->Sample(x,y,z,colour,opacity))
	{
          framebuffer.WritePixel(x,y,z,colour,opacity);
	}
      }
    }
  }
}

//=======================================
// TransformToScreenSpace
//---------------------------------------
void MicroPolygon::TransformToScreenSpace(Matrix4 &transform)
{
  int i;

  for(i=0;i<4;i++)
  {
    if (p[i].z<0.1)
    {
      printf("Error Z value %f <=0.1\n",p[i].z);
      exit(EXIT_FAILURE);
    }
    p[i].x=p[i].x/p[i].z;
    p[i].y=p[i].y/p[i].z;
    p[i]=transform*p[i];
  }
}
