#ifndef quaternion_h
#define quaternion_h
/*------------------------------------------------------
 * TITLE:  quaternion.h
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Useful functions
 * (C)     Copyright $Date: 1997/11/21 20:26:40 $
 * $Log: quaternion.h $
 * Revision 1.2  1997/11/21 20:26:40  ady
 * Changed spelling
 *
 * Revision 1.1  1997/11/16 14:21:49  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// C++ includes
#include "iostream.h"
#include "iomanip.h"

// Reyes includes
#include "matrix4.h"

class Quaternion
{
  public:
    Quaternion(float _x=0,float _y=0,float _z=0,float _w=0)
      :x(_x),y(_y),z(_z),w(_w){}

    void Rotation(float theta,Vector3 r); // Rotation of theta about vector r
    Matrix4 RotationMatrix();

    // Stream input/output
    friend ostream &operator<<(ostream &io,const Quaternion &q);

  private:
    float x,y,z,w;
};

#endif
