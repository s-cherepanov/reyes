/*------------------------------------------------------
 * TITLE:  useful.cpp
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Useful functions
 * (C)     Copyright $Date: 1998/12/10 20:13:03 $
 * $Log: useful.cpp,v $
 * Revision 1.2  1998/12/10 20:13:03  ady
 * New function
 *
 * Revision 1.1  1997/11/16 14:18:48  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// C includes
#include <math.h>

//=======================================
// logn
//---------------------------------------
float logn(float base,float num)
{
  return log(num)/log(base);
}
