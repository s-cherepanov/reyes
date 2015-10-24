#ifndef primitive_h
#define primitive_h
/*------------------------------------------------------
 * TITLE:  primitive.h
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Primitive class (container of sphere,polygon,patch,etc...)
 * (C)     Copyright $Date: 1998/12/10 20:17:15 $
 * $Log: primitive.h,v $
 * Revision 1.4  1998/12/10 20:17:15  ady
 * Remove texturing
 *
 * Revision 1.3  1997/11/29 18:10:42  ady
 * New dicing method and now contains texture co-ordinates
 *
 * Revision 1.2  1997/11/21 20:26:40  ady
 * Made consistent with new displacement shaders
 *
 * Revision 1.1  1997/11/16 14:21:49  ady
 * Initial revision
 *
 * Revision 1.3  1997/08/28 00:16:13  ady
 * Added AttachDisplacementShader()
 *
 * Revision 1.2  1997/03/10 21:58:23  ady
 * New structure for dicing and splitting
 *
 * Revision 1.1  1997/02/23 23:00:16  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// C++ includes
#include "iostream.h"
#include "iomanip.h"

// Misc includes
#include "list.h"

// Reyes includes
#include "camera.h"
#include "microgrid.h"
#include "surface.h"
#include "displacement_shaders.h"
#include "boundbox3.h"

class Primitive
{
  public:
    virtual void Dump(){cout << "Unknown primitive!";}
    virtual bool TransformToEyeSpace(Matrix4 t_position,Matrix4 t_vector){cout << "TransformToEyeSpace():Unknown primitive!";}

    virtual void DoDice(MicroGrid &microgrid,int us,int vs){cout << "DoDice()::Unknown primitive!\n";}

    virtual bool Splitable(){cout << "Unknown primitive!"; return FALSE;}
    virtual void Split(list<Primitive*> &primlist){cout << "Split():Unknown primitive!";}

    virtual bool EyeBound(BoundBox3 &bb){cout << "EyeBound():Unknown primitive!\n"; return FALSE;}

    void Dice(MicroGrid &microgrid,float xscale,float yscale);
    void EstimateGridSize(float xscale,float yscale,int &us,int &vs);
    bool Diceable(float xscale,float yscale);
};

#endif
