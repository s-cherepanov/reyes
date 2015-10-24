/*------------------------------------------------------
 * TITLE:  noise.h
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Noise definition
 * (C)     Copyright $Date: 1997/11/21 20:26:40 $
 * $Log: noise.h,v $
 * Revision 1.2  1997/11/21 20:26:40  ady
 * Change noise3D() so a vector is returned
 *
 * Revision 1.1  1997/11/16 14:21:49  ady
 * Initial revision
 *
 * Revision 1.1  1997/02/23 23:00:16  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// Reyes includes
#include "vector3.h"

void initialise_noise(void);
float noise1D(float x,float y,float z);
Vector3 noise3D(float x,float y,float z);
