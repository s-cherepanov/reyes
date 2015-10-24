#ifndef microgrid_h
#define microgrid_h
/*------------------------------------------------------
 * TITLE:  microgrid.h
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Grid of micropolygons class
 * (C)     Copyright $Date: 1998/12/10 20:16:20 $
 * $Log: microgrid.h,v $
 * Revision 1.4  1998/12/10 20:16:20  ady
 * Added texture coords
 *
 * Revision 1.3  1997/11/29 18:09:05  ady
 * Uses new lights
 *
 * Revision 1.2  1997/11/21 20:26:40  ady
 * Made consistent with new displacement shaders
 *
 * Revision 1.1  1997/11/16 14:21:49  ady
 * Initial revision
 *
 * Revision 1.3  1997/08/28 00:16:13  ady
 * Added SetTextureCoords() and AttachDisplacementShader()
 *
 * Revision 1.2  1997/03/10 21:57:57  ady
 * Now uses surface shaders
 *
 * Revision 1.1  1997/02/23 23:00:16  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// C includes
#include <stdio.h>

// C++ includes
#include "iostream.h"
#include "iomanip.h"

// Misc includes
#include "list.h"

// Reyes includes
#include "point3.h"
#include "micropoly.h"
#include "colour.h"
#include "light.h"
#include "surface.h"
#include "displacement_shaders.h"

class MicroGrid
{
  public:
    MicroGrid(int width=1,int height=1);  // Constructor (width,height in micropolygons)
    ~MicroGrid(); // Destructor

    void Allocate(int width,int height);
    void Free();

    // Stream input/output
    friend ostream &operator<<(ostream &io,const MicroGrid &m);
    void WritePostscript(FILE *fp);

    bool ExtractMicroPolygon(MicroPolygon &m,int u,int v); // Extract micro-polygon from grid

    int Width(){return width;}
    int Height(){return height;}

    void SetSize(int width,int height);  // Change size and re-initialise microgrid
    void SetTextureCoords(float _umin,float _vmin,float _umax,float _vmax)
      {umin=_umin; vmin=_vmin; umax=_umax; vmax=_vmax;}

    void MicroGrid::ComputeNormals();
    void MicroGrid::Shade(list<Light*> &lights);
    void MicroGrid::Displace();
    void MicroGrid::Statistics(float &zmin,float &zmax,float &maxusize,float &maxvsize);

    Point3 **point;                  // 2D array of points
    Colour **colour;                 // 2D array of micro-polygon colours
    Opacity **opacity;               // 2D array of micro-polygon opacity
    Vector3 **normal;                // 2D array of normals

  private:
    int width,height;
    float umin,vmin;
    float umax,vmax;
};

#endif
