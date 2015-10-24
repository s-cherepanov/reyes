#ifndef disk_h
#define disk_h
/*------------------------------------------------------
 * TITLE:  disk.h
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Disk primitive class
 * (C)     Copyright $Date: 1998/12/10 20:14:53 $
 * $Log: disk.h,v $
 * Revision 1.4  1998/12/10 20:14:53  ady
 * Added tmin,tmax to disk
 *
 * Revision 1.3  1997/11/29 18:09:05  ady
 * Uses new lights and new dicing method
 *
 * Revision 1.2  1997/11/23 15:41:47  ady
 * Added arcmin and arcmax
 *
 * Revision 1.1  1997/11/16 14:21:49  ady
 * Initial revision
 *
 * Revision 1.1  1997/08/28 00:16:13  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// C++ includes
#include "iostream.h"
#include "iomanip.h"

// Misc includes
#include "list.h"

// Reyes includes
#include "primitive.h"
#include "point3.h"
#include "boundbox3.h"
#include "microgrid.h"
#include "camera.h"
#include "matrix4.h"

class Disk: public Primitive
{
  public:
    Disk(float z=0,float radius=1,float arcmin=0,float arcmax=360,float tmin=0,float tmax=1); // Constructor

    // Stream input/output
    friend ostream &operator<<(ostream &io,const Disk &s);
    void Dump();

    bool TransformToEyeSpace(Matrix4 t_position,Matrix4 t_vector);

    void DoDice(MicroGrid &microgrid,int us,int vs);

    bool Splitable();
    void Split(list<Primitive*> &primlist);

    bool EyeBound(BoundBox3 &bb);

  private:
    float z;
    float radius;
    float arcmin,arcmax;
    float tmin,tmax;
};

#endif
