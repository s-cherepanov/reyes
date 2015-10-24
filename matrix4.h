#ifndef matrix4_h
#define matrix4_h
/*------------------------------------------------------
 * TITLE:  matrix4.h
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Matrix definition
 * (C)     Copyright $Date: 1999/04/01 19:59:59 $
 * $Log: matrix4.h,v $
 * Revision 1.3  1999/04/01 19:59:59  ady
 * *** empty log message ***
 *
 * Revision 1.2  1998/12/10 20:16:14  ady
 * Added inverse
 *
 * Revision 1.1  1997/11/16 14:21:49  ady
 * Initial revision
 *
 * Revision 1.2  1997/08/28 00:16:13  ady
 * Added Scale(), Translate() and Rotate()
 *
 * Revision 1.1  1997/02/23 23:00:16  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// C includes
#include <stdio.h>

// C++ includes
#include "iostream.h"

// Reyes includes
#include "vector3.h"

class Matrix4
{
  public:
    Matrix4();

    void Identity();
    void Scale(Vector3 s);
    void Translate(Vector3 t);
    void Rotate(Vector3 r);
    Matrix4 Inverse();

    // Stream input/output
    friend ostream &operator<<(ostream &io,Matrix4 &m);
    void writeBinary(FILE *fp);
    void readBinary(FILE *fp);

    float* operator[](int row){return element[row];}

    friend Vector3 operator*(Matrix4 m,Vector3 v);
    friend Matrix4 operator*(Matrix4 a,Matrix4 b);

  private:
    float element[4][4];
};

#endif
