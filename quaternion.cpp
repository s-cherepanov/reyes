/*------------------------------------------------------
 * TITLE:  quaternion.cpp
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Useful functions
 * (C)     Copyright $Date: 1997/11/21 20:17:43 $
 * $Log: quaternion.cpp $
 * Revision 1.2  1997/11/21 20:17:43  ady
 * Changed rotation routine (fixed bug) and changed name
 *
 * Revision 1.1  1997/11/16 14:18:48  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

#include "quaternion.h"
#include "useful.h"

void Quaternion::Rotation(float theta,Vector3 r)
{
  float sn=sin(-DEGTORAD*theta/2),cs=cos(-DEGTORAD*theta/2);

  r.Normalise();
  x=r.x*sn; y=r.y*sn; z=r.z*sn; w=cs;
}

Matrix4 Quaternion::RotationMatrix()
{
  Matrix4 t;
  float norm=x*x+y*y+z*z+w*w;
  float s=(norm>0)?2/norm:0;
  float xs=x*s,ys=y*s,zs=z*s,
        wx=w*xs,wy=w*ys,wz=w*zs,
        xx=x*xs,xy=x*ys,xz=x*zs,
        yy=y*ys,yz=y*zs,zz=z*zs;
  t[0][0]=1-(yy+zz); t[0][1]=xy+wz;     t[0][2]=xz-wy;
  t[1][0]=xy-wz;     t[1][1]=1-(xx+zz); t[1][2]=yz+wx;
  t[2][0]=xz+wy;     t[2][1]=yz-wx;     t[2][2]=1-(xx+yy);
  return t;
}

//=======================================
//========STREAM INPUT/OUTPUT============
//=======================================

ostream &operator<<(ostream &io,const Quaternion &q)
{
  io << "[" << q.x << "," << q.y << "," << q.z << "," << q.w << "]";
  return io;
}
