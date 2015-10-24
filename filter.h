/*------------------------------------------------------
 * TITLE:  filter.h
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Filtering functions header
 * (C)     Copyright $Date: 1998/12/10 20:15:03 $
 * $Log: filter.h,v $
 * Revision 1.1  1998/12/10 20:15:03  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

float RiBoxFilter(float x,float y,float xwidth,float ywidth);
float RiGaussianFilter(float x,float y,float xwidth,float ywidth);
