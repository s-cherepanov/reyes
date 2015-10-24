#ifndef vector3_h
#define vector3_h
/*------------------------------------------------------
 * TITLE:  vector3.h
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Vector class
 * (C)     Copyright $Date: 1998/12/10 20:19:08 $
 * $Log: vector3.h,v $
 * Revision 1.3  1998/12/10 20:19:08  ady
 * Removes reyes include
 *
 * Revision 1.2  1997/11/21 20:26:40  ady
 * Added - and += operators
 *
 * Revision 1.1  1997/11/16 14:21:49  ady
 * Initial revision
 *
 * Revision 1.2  1997/08/28 00:16:13  ady
 * Removed include "reyes.h"
 *
 * Revision 1.1  1997/02/23 23:00:16  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// C++ includes
#include "iostream.h"

class Vector3
{
  public:
    Vector3(float x=0,float y=0,float z=0);

    // Stream input/output
    friend ostream &operator<<(ostream &io,const Vector3 &v);

    // Arithmetic operations
    friend Vector3 operator+(Vector3 a,Vector3 b);
    friend Vector3 operator-(Vector3 a,Vector3 b);
    friend Vector3 operator^(Vector3 a,Vector3 b);
    friend float operator*(Vector3 a,Vector3 b);

    Vector3 operator+=(Vector3 a){x+=a.x; y+=a.y; z+=a.z; return *this;}

    friend Vector3 operator*(float s,Vector3 v);
    friend Vector3 operator*(Vector3 v,float s);
    friend Vector3 operator/(Vector3 v,float s);

    friend Vector3 operator-(Vector3 v);

    friend bool operator!=(Vector3 a,Vector3 b);
    friend bool operator==(Vector3 a,Vector3 b);

    void Normalise();
    float Length();

    float x,y,z;
};

#endif
