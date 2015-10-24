#ifndef reyes_h
#define reyes_h
/*------------------------------------------------------
 * TITLE:  reyes.h
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Useful REYES definitions
 * (C)     Copyright $Date: 1997/11/29 18:10:42 $
 * $Log: reyes.h,v $
 * Revision 1.2  1997/11/29 18:10:42  ady
 * Now has verbosity switch
 *
 * Revision 1.1  1997/11/16 14:21:49  ady
 * Initial revision
 *
 * Revision 1.3  1997/08/28 00:16:13  ady
 * Changed interface to facilitate shadow map rendering
 *
 * Revision 1.2  1997/03/10 21:58:23  ady
 * Added inline functions min and max
 *
 * Revision 1.1  1997/02/23 23:00:16  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// C includes
#include <stdio.h>

template <class T>inline T min(T a,T b){return (a<b)?a:b;}
template <class T>inline T max(T a,T b){return (a>b)?a:b;}

template <class T>inline T min(T a,T b,T c){return (a<b)?(a<c?a:c):b;}
template <class T>inline T max(T a,T b,T c){return (a>b)?(a>c?a:c):b;}

typedef struct
{
  int subdivision;
  float micropolygonsize;
  int maxmicrogridsize;
  bool jitter;
  bool doublesided;
  bool shadows;
  char *world;
  char *image;
  int verbosity;
}reyesoptions_str;

typedef struct
{
  bool perspective;
  float FieldOfView;
}projectionoptions_str;

typedef struct
{
  int width;
  int height;
  float pixelratio;
}formatoptions_str;

#ifdef FIXEDPOINT
  typedef int position_t;
  typedef int vector_t;
  typedef int colour_t;
  typedef int scalar_t;
# define FLOAT2FIXED(val,places) (int)(val*(1<<places))
# define FIXED2FLOAT(val,places) ((float)val/(1<<places))
#else
  typedef float position_t;
  typedef float vector_t;
  typedef float colour_t;
  typedef float scalar_t;
# define FLOAT2FIXED(val,places) (val)
# define FIXED2FLOAT(val,places) (val)
#endif

/* Global variables */

extern reyesoptions_str      reyesoptions;
extern projectionoptions_str projectionoptions;
extern formatoptions_str     formatoptions;

#endif
