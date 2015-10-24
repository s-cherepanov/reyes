#ifndef box2_h
#define box2_h
/*------------------------------------------------------
 * TITLE:  box2.h
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   
 * (C)     Copyright $Date: 1998/12/10 20:13:27 $
 * $Log: box2.h,v $
 * Revision 1.2  1998/12/10 20:13:27  ady
 * Changed defaults on Box2
 *
 * Revision 1.1  1997/11/29 18:09:05  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

template <class T>
class Box2
{
  public:
    Box2(T _xmin=0,T _ymin=0,T _xmax=1,T _ymax=1)
      {xmin=_xmin; ymin=_ymin; xmax=_xmax; ymax=_ymax;}

  public:
    T xmin,ymin;
    T xmax,ymax;
};

#endif
