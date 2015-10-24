/*------------------------------------------------------
 * TITLE:  boundbox3.c++
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Bounding box utilities
 * (C)     Copyright $Date: 1998/12/10 20:04:55 $
 * $Log: boundbox3.cpp,v $
 * Revision 1.3  1998/12/10 20:04:55  ady
 * small fix
 *
 * Revision 1.2  1997/11/23 15:24:06  ady
 * Can now transform bounding box
 *
 * Revision 1.1  1997/11/16 14:15:10  ady
 * Initial revision
 *
 * Revision 1.2  1997/08/28 00:05:57  ady
 * Fixed problem with calculating screen bounds of bounding box
 *
 * Revision 1.1  1997/02/23 22:55:40  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// Reyes includes
#include "boundbox3.h"
#include "useful.h"

//=======================================
// Constructor
//---------------------------------------
BoundBox3::BoundBox3(Point3 min,Point3 max)
{
  this->min=min;
  this->max=max;
}

//=======================================
// EyeToScreenBound
//---------------------------------------
/*
BoundBox3 BoundBox3::EyeToScreenBound(float vpscale,int centx,int centy)
{
  BoundBox3 screen;
  Point3 v[8];
  int i;

  v[0].x=min.x; v[0].y=min.y; v[0].z=min.z;
  v[1].x=min.x; v[1].y=min.y; v[1].z=max.z;
  v[2].x=min.x; v[2].y=max.y; v[2].z=min.z;
  v[3].x=min.x; v[3].y=max.y; v[3].z=max.z;
  v[4].x=max.x; v[4].y=min.y; v[4].z=min.z;
  v[5].x=max.x; v[5].y=min.y; v[5].z=max.z;
  v[6].x=max.x; v[6].y=max.y; v[6].z=min.z;
  v[7].x=max.x; v[7].y=max.y; v[7].z=max.z;

  for(i=0;i<8;i++)
  {
    v[i].x=(v[i].x*vpscale/v[i].z)+centx;
    v[i].y=(v[i].y*vpscale/v[i].z)+centy;
  }

  screen.min=v[0];
  screen.max=v[1];
  for(i=1;i<8;i++)
  {
    screen.min.x=MIN(screen.min.x,v[i].x);
    screen.min.y=MIN(screen.min.y,v[i].y);
    screen.min.z=MIN(screen.min.z,v[i].z);
    screen.max.x=MAX(screen.max.x,v[i].x);
    screen.max.y=MAX(screen.max.y,v[i].y);
    screen.max.z=MAX(screen.max.z,v[i].z);
  }
  return screen;
}
*/

//=======================================
// SortMinMax
//---------------------------------------
void BoundBox3::SortMinMax()
{
  float newmin,newmax;

  newmin=MIN(min.x,max.x);
  newmax=MAX(min.x,max.x);
  min.x=newmin;
  max.x=newmax;
  newmin=MIN(min.y,max.y);
  newmax=MAX(min.y,max.y);
  min.y=newmin;
  max.y=newmax;
  newmin=MIN(min.z,max.z);
  newmax=MAX(min.z,max.z);
  min.z=newmin;
  max.z=newmax;
}

//=======================================
// operator*   Matrix4 * BoundBox3
//---------------------------------------
BoundBox3 operator*(Matrix4 transform,BoundBox3 box)
{
  Point3 p[8];
  int i;
  BoundBox3 nbox;

  p[0]=Point3(box.min.x,box.min.y,box.min.z);
  p[1]=Point3(box.min.x,box.min.y,box.max.z);
  p[2]=Point3(box.min.x,box.max.y,box.min.z);
  p[3]=Point3(box.min.x,box.max.y,box.max.z);
  p[4]=Point3(box.max.x,box.min.y,box.min.z);
  p[5]=Point3(box.max.x,box.min.y,box.max.z);
  p[6]=Point3(box.max.x,box.max.y,box.min.z);
  p[7]=Point3(box.max.x,box.max.y,box.max.z);
  for(i=0;i<8;i++)
  {
    p[i]=transform*p[i];
  }
  nbox.min.x=MIN(MIN(MIN(p[0].x,p[1].x),MIN(p[2].x,p[3].x)),MIN(MIN(p[4].x,p[5].x),MIN(p[6].x,p[7].x)));
  nbox.min.y=MIN(MIN(MIN(p[0].y,p[1].y),MIN(p[2].y,p[3].y)),MIN(MIN(p[4].y,p[5].y),MIN(p[6].y,p[7].y)));
  nbox.min.z=MIN(MIN(MIN(p[0].z,p[1].z),MIN(p[2].z,p[3].z)),MIN(MIN(p[4].z,p[5].z),MIN(p[6].z,p[7].z)));
  nbox.max.x=MAX(MAX(MAX(p[0].x,p[1].x),MAX(p[2].x,p[3].x)),MAX(MAX(p[4].x,p[5].x),MAX(p[6].x,p[7].x)));
  nbox.max.y=MAX(MAX(MAX(p[0].y,p[1].y),MAX(p[2].y,p[3].y)),MAX(MAX(p[4].y,p[5].y),MAX(p[6].y,p[7].y)));
  nbox.max.z=MAX(MAX(MAX(p[0].z,p[1].z),MAX(p[2].z,p[3].z)),MAX(MAX(p[4].z,p[5].z),MAX(p[6].z,p[7].z)));
  return nbox;
}

//=======================================
//========STREAM INPUT/OUTPUT============
//=======================================

ostream &operator<<(ostream &io,const BoundBox3 &bbox)
{
  io << "[" << bbox.min << " to " << bbox.max << "]";
  return io;
}
