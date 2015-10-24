/*------------------------------------------------------
 * TITLE:  noise.cpp
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Noise implementation
 * (C)     Copyright $Date: 1997/11/21 20:17:43 $
 * $Log: noise.cpp,v $
 * Revision 1.3  1997/11/21 20:17:43  ady
 * Now uses bicubic rather than linear interpolation
 *
 * Revision 1.2  1997/11/18 21:42:38  ady
 * Made noise function user bicubic interpolation rather than linear
 *
 * Revision 1.1  1997/11/16 14:18:48  ady
 * Initial revision
 *
 * Revision 1.1  1997/02/23 22:55:40  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// C includes
#include <stdlib.h>

// Reyes includes
#include "useful.h"
#include "noise.h"

#define MAX_NOISE 32

float noise_t[MAX_NOISE][MAX_NOISE][MAX_NOISE][3];

float noise(float x,float y,float z,int d);

float frac(float r)
{
  return r-(int)r;
}

void initialise_noise(void)
{
  int x,y,z,xx,yy,zz,d;

  for(x=0;x<MAX_NOISE;x++)
    for(y=0;y<MAX_NOISE;y++)
      for(z=0;z<MAX_NOISE;z++)
        for(d=0;d<3;d++)
	  noise_t[x][y][z][d]=(float)rand()/RAND_MAX;
}

float noise1D(float x,float y,float z)
{
  return noise(x,y,z,0);
}

Vector3 noise3D(float x,float y,float z)
{
  return Vector3(noise(x,y,z,0),noise(x,y,z,1),noise(x,y,z,2));
}

float noise(float x,float y,float z,int d)
{
  int x0,x1,x2,x3;
  int y0,y1,y2,y3;
  int z0,z1,z2,z3;
  float ox,oy,oz;
  float v0,v1,v2,v3;

  x+=1500; y+=1500; z+=1500;

  ox=frac(x); oy=frac(y); oz=frac(z);

  x0=((int)x+0)%MAX_NOISE;
  x1=((int)x+1)%MAX_NOISE;
  x2=((int)x+2)%MAX_NOISE;
  x3=((int)x+3)%MAX_NOISE;
  y0=((int)y+0)%MAX_NOISE;
  y1=((int)y+1)%MAX_NOISE;
  y2=((int)y+2)%MAX_NOISE;
  y3=((int)y+3)%MAX_NOISE;
  z0=((int)z+0)%MAX_NOISE;
  z1=((int)z+1)%MAX_NOISE;
  z2=((int)z+2)%MAX_NOISE;
  z3=((int)z+3)%MAX_NOISE;

  v0=Interpolate2D(noise_t[x0][y0][z0][d],noise_t[x1][y0][z0][d],noise_t[x2][y0][z0][d],noise_t[x3][y0][z0][d],
                   noise_t[x0][y1][z0][d],noise_t[x1][y1][z0][d],noise_t[x2][y1][z0][d],noise_t[x3][y1][z0][d],
                   noise_t[x0][y2][z0][d],noise_t[x1][y2][z0][d],noise_t[x2][y2][z0][d],noise_t[x3][y2][z0][d],
                   noise_t[x0][y3][z0][d],noise_t[x1][y3][z0][d],noise_t[x2][y3][z0][d],noise_t[x3][y3][z0][d],ox,oy);
  v1=Interpolate2D(noise_t[x0][y0][z1][d],noise_t[x1][y0][z1][d],noise_t[x2][y0][z1][d],noise_t[x3][y0][z1][d],
                   noise_t[x0][y1][z1][d],noise_t[x1][y1][z1][d],noise_t[x2][y1][z1][d],noise_t[x3][y1][z1][d],
                   noise_t[x0][y2][z1][d],noise_t[x1][y2][z1][d],noise_t[x2][y2][z1][d],noise_t[x3][y2][z1][d],
                   noise_t[x0][y3][z1][d],noise_t[x1][y3][z1][d],noise_t[x2][y3][z1][d],noise_t[x3][y3][z1][d],ox,oy);
  v2=Interpolate2D(noise_t[x0][y0][z2][d],noise_t[x1][y0][z2][d],noise_t[x2][y0][z2][d],noise_t[x3][y0][z2][d],
                   noise_t[x0][y1][z2][d],noise_t[x1][y1][z2][d],noise_t[x2][y1][z2][d],noise_t[x3][y1][z2][d],
                   noise_t[x0][y2][z2][d],noise_t[x1][y2][z2][d],noise_t[x2][y2][z2][d],noise_t[x3][y2][z2][d],
                   noise_t[x0][y3][z2][d],noise_t[x1][y3][z2][d],noise_t[x2][y3][z2][d],noise_t[x3][y3][z2][d],ox,oy);
  v3=Interpolate2D(noise_t[x0][y0][z3][d],noise_t[x1][y0][z3][d],noise_t[x2][y0][z3][d],noise_t[x3][y0][z3][d],
                   noise_t[x0][y1][z3][d],noise_t[x1][y1][z3][d],noise_t[x2][y1][z3][d],noise_t[x3][y1][z3][d],
                   noise_t[x0][y2][z3][d],noise_t[x1][y2][z3][d],noise_t[x2][y2][z3][d],noise_t[x3][y2][z3][d],
                   noise_t[x0][y3][z3][d],noise_t[x1][y3][z3][d],noise_t[x2][y3][z3][d],noise_t[x3][y3][z3][d],ox,oy);
  return Interpolate1D(v0,v1,v2,v3,oz);
}

