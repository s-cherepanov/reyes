#ifndef useful_h
#define useful_h
/*------------------------------------------------------
 * TITLE:  useful.h
 * AUTHOR: Adrian Skilling
 * ENVIR:  RiscBSD
 * DESC:   Useful stuff
 * (C)     Copyright $Date: 1998/12/10 20:19:01 $
 * $Log: useful.h,v $
 * Revision 1.5  1998/12/10 20:19:01  ady
 * Added logn
 *
 * Revision 1.4  1997/11/29 18:12:35  ady
 * Now includes GLOBAL and LOCAL definitions
 * No FALSE or TRUE defined (comes from bool.h)
 *
 * Revision 1.3  1997/11/21 20:26:40  ady
 * Now does interpolation and MAX and SGN functions
 *
 * Revision 1.2  1997/11/18 21:40:57  ady
 * Added Bezier and Interpolate functions
 *
 * Revision 1.1  1997/11/16 14:21:49  ady
 * Initial revision
 *
 * Revision 1.1  1997/08/28 00:16:13  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

#define DEGTORAD 0.017453293
#define RADTODEG 57.29578

#define CLAMP(v,mn,mx)   (((v)<(mn))?(mn):(((v)>(mx))?(mx):(v)))
#define MIN(a,b)         ((a)<(b)?(a):(b))    
#define MAX(a,b)         ((a)>(b)?(a):(b))
#define ABS(n)           ((n)>=0?(n):-(n))
#define SGN(n)           ((n)>0?1:((n<0)?-1:0))

#define GLOBAL extern
#define LOCAL  static

//=======================================
// logn
//---------------------------------------
float logn(float base,float num);

//=======================================
// Bezier1D
//---------------------------------------
template <class T>T Bezier1D(T p1,T p2,T p3,T p4,float t)
{
  return ((1-t)*(1-t)*(1-t))*p1+
         (3*t*(1-t)*(1-t))*p2+
         (3*t*t*(1-t))*p3+
         (t*t*t)*p4;
}

//=======================================
// Bezier2D
//---------------------------------------
template <class T>T Bezier2D(T p00,T p10,T p20,T p30,
                             T p01,T p11,T p21,T p31,
                             T p02,T p12,T p22,T p32,
                             T p03,T p13,T p23,T p33,
                             float s,float t)
{
  return Bezier1D(Bezier1D(p00,p10,p20,p30,s),
                  Bezier1D(p01,p11,p21,p31,s),
                  Bezier1D(p02,p12,p22,p32,s),
                  Bezier1D(p03,p13,p23,p33,s),t);
}

//=======================================
// Interpolate1D
//---------------------------------------
template <class T>T Interpolate1D(T p0,T p1,T p2,T p3,float t)
{
  T a=p1+0.333*(p2-p0);
  T b=p2-0.333*(p3-p1);
  return Bezier1D(p1,a,b,p2,t);
}

//=======================================
// Interpolate2D
//---------------------------------------
template <class T>T Interpolate2D(T p00,T p10,T p20,T p30,
                                  T p01,T p11,T p21,T p31,
                                  T p02,T p12,T p22,T p32,
                                  T p03,T p13,T p23,T p33,
                                  float s,float t)
{
  return Interpolate1D(Interpolate1D(p00,p10,p20,p30,s),
                       Interpolate1D(p01,p11,p21,p31,s),
                       Interpolate1D(p02,p12,p22,p32,s),
                       Interpolate1D(p03,p13,p23,p33,s),t);
}

#endif
