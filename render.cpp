/*------------------------------------------------------
 * TITLE:  render.cpp
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Renderman interface implementation
 * (C)     Copyright $Date: 1998/12/10 20:11:20 $
 * $Log: render.cpp,v $
 * Revision 1.1  1998/12/10 20:11:20  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// C includes

// C++ includes

// Reyes includes
#include "ri.h"

/******************************************************/
/* Rendering                                          */
/******************************************************/
void GlobalState::RenderPrimitive(Primitive *prim)
{
  BoundBox3 eyebound;
  bool undiceable=TRUE;
  int u,v;
  float xscale=tCameraToRaster[0][0];
  float yscale=tCameraToRaster[1][1];
  MicroGrid microgrid;       /* Grid of micro-polygons */
  MicroPolygon micropolygon; /* Single micro-polygon   */
  list<Primitive*> primitives;

  // Add primitive to head of primitive to be rendered list
  primitives.push_front(prim);

  /* Render all primitives on list */
  while(!primitives.empty())
  {
    prim=*primitives.begin();
    primitives.pop_front();
    if (RiGlobal.options.verbosity>0)
    {
      cout << "Rendering: "; prim->Dump();
    }

    /* Do dicing and rendering */
    if (prim->Diceable(xscale,yscale))
    {
      prim->Dice(microgrid,xscale,yscale);
      microgrid.ComputeNormals();
      if (RiCurrent.geometryAttributes.displacement)
      {
	microgrid.Displace();
	microgrid.ComputeNormals();
      }
      microgrid.Shade(lights);
      for(u=0;u<microgrid.Width();u++)
      {
        for(v=0;v<microgrid.Height();v++)
        {
	  if (microgrid.ExtractMicroPolygon(micropolygon,u,v))
	  {
	    micropolygon.TransformToScreenSpace(tCameraToRaster);
	    micropolygon.Rasterize(*framebuffer);
	  }
	}
      }
      microgrid.Free();
    }
    else
    {
      if (RiGlobal.options.verbosity>0)
      {
	cout << "Splitting...\n";
      }
      prim->Split(primitives);
    }
  }
}
