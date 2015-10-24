/*------------------------------------------------------
 * TITLE:  shadowspot.cpp
 * AUTHOR: Adrian Skilling
 * ENVIR:  NetBSD
 * DESC:   Useful REYES definitions
 * (C)     Copyright $Date: 1998/12/10 20:21:12 $
 * $Log: shadowspot.cpp,v $
 * Revision 1.1  1998/12/10 20:21:12  ady
 * Initial revision
 *
 * Revision 1.1  1997/11/29 17:56:01  ady
 * Initial revision
 *
 * Revision 1.2  1997/11/21 20:14:41  ady
 * meaningless change
 *
 * Revision 1.1  1997/11/16 14:15:10  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// C includes
#include <math.h>

// Reyes includes
#include "shadowspot.h"
#include "useful.h"
#include "ri.h"

//=======================================
// Constructor
//---------------------------------------
Shadowspot::Shadowspot(Point3 _from,Point3 _to,Colour _colour,float _intensity,
                       float _coneangle,float _conedeltaangle,float _beamdistribution,char *map=NULL,
		       float _radius)
{
  from=RiGlobal.tWorldToCamera*RiCurrent.transform*_from;
  to=RiGlobal.tWorldToCamera*RiCurrent.transform*_to;
  colour=_colour;
  intensity=_intensity;
  coneangle=_coneangle*0.5;
  conedeltaangle=_conedeltaangle*0.5;
  beamdistribution=_beamdistribution;
  radius=_radius;
  if (map) loadDepthMap(map);
  minDepthDifference=0.5;
}

//=======================================
// Constructor
//---------------------------------------
Shadowspot::Shadowspot(ParamList parameters)
{
  int i;

  from=Point3(0,0,0);
  to=Point3(0,0,1);
  intensity=1;
  coneangle=0.5*30;
  conedeltaangle=0.5*5;
  beamdistribution=2;
  colour=Colour(1,1,1);
  depth=NULL;
  radius=0.1;

  if (parameters.Exists("from"))             from=parameters.Vector("from");
  if (parameters.Exists("to"))               to=parameters.Vector("to");
  if (parameters.Exists("intensity"))        intensity=parameters.Float("intensity");
  if (parameters.Exists("color"))            colour=parameters.Vector("color");
  if (parameters.Exists("coneangle"))        coneangle=0.5*parameters.Float("coneangle");
  if (parameters.Exists("conedeltaangle"))   conedeltaangle=0.5*parameters.Float("conedeltaangle");
  if (parameters.Exists("beamdistribution")) beamdistribution=parameters.Float("beamdistribution");
  if (parameters.Exists("map"))              {strcpy(map,parameters.String("map")); loadDepthMap(map);}
  if (parameters.Exists("radius"))           {radius=0.5*parameters.Float("radius");}

  from=RiGlobal.tWorldToCamera*RiCurrent.transform*from;
  to=RiGlobal.tWorldToCamera*RiCurrent.transform*to;
  minDepthDifference=0.5;
}

//=======================================
// SoftVisible
//---------------------------------------
float Shadowspot::SoftVisible(Point3 wP,Point3 eP)
{
  int x,y,rx,ry,xi,yi;
  float d;
  Point3 p;
  float sum=0;
  int sampdist=7;
  int samples=0;
  float px,py,r,lz;

  p=tWorldToCamera*wP;
  lz=p.z;
  p.x=p.x/p.z;
  p.y=p.y/p.z;
  p=tCameraToRaster*p;

  x=(int)p.x;
  y=(int)p.y;

  rx=(int)(0.5*radius*(lz-1)/lz*tCameraToRaster[0][0]);
  ry=(int)(0.5*radius*(lz-1)/lz*tCameraToRaster[1][1]);

  // cout << rx << "," << ry << "\n";

  sum=0;
  for(xi=-rx;xi<=rx;xi++)
    for(yi=-ry;yi<=ry;yi++)
    {
      if ((x+xi>=0)&&(x+xi<width)&&(y+yi>=0)&&(y+yi<height))
      {
	d=depth[(y+yi)*width+(x+xi)];
	// Is this depth closer ?, if not don't even consider it
	if (d+minDepthDifference<lz)
	{
	  px=xi*lz/(lz-d);
	  py=yi*lz/(lz-d);
	  r=sqrt((px*px)/(rx*rx)+(py*py)/(ry*ry));
	  if (r<1)
	  {
	    // this blocks the light sources, so just include 0 in samples
	    samples++;
	  }
	  else
	  {
	    // doesn't block light source, so don't include it
	  }
	}
	else
	{
	  sum++;
	  samples++;
	}
      }
    }
  return sum/samples;
}

//=======================================
// Visible
//---------------------------------------
float Shadowspot::Visible(Point3 wP,Point3 eP)
{
  int x,y;
  float d;
  Point3 p;
  float sum=0;
  int sampdist=7;

  p=tWorldToCamera*wP;
  p.x=p.x/p.z;
  p.y=p.y/p.z;
  p=tCameraToRaster*p;

  x=(int)p.x;
  y=(int)p.y;

  if ((x<0)||(x>=width)||(y<0)||(y>=height))
    return 1;

  d=depth[y*width+x];
  if (d+minDepthDifference<p.z)
    return 0;
  else
    return 1;
}

//=======================================
// LocalIntensity
//---------------------------------------
float Shadowspot::LocalIntensity(Point3 p)
{
  Vector3 L,D;
  float dist2,d1,d2,i;
  float v=SoftVisible(wP,p);
  // float v=1;

  d1=cos(DEGTORAD*coneangle);
  d2=cos(DEGTORAD*(1+coneangle+conedeltaangle));

  L=to-from;
  L.Normalise();
  D=p-from;
  dist2=D.x*D.x+D.y*D.y+D.z*D.z;
  D.Normalise();
  i=L*D;    /* Cos of angle between directions */

  if (i>=d1)
  {
    return v*intensity/dist2;
  }
  else
  {
    if (i>d2)
    {
      return v*intensity*(i-d2)/(dist2*(d1-d2));
    }
    else
    {
      return 0;
    }
  }
}

//=======================================
// Diffuse
//---------------------------------------
Colour Shadowspot::Ambient()
{
  return Colour(0,0,0);
}

//=======================================
// Diffuse
//---------------------------------------
Colour Shadowspot::Diffuse(Point3 P,Vector3 N)
{
  Vector3 L=to-from;
  float i;

  L.Normalise();
  i=N*L;
  i=MAX(-i,0);
  return (LocalIntensity(P)*i)*colour;
}

//=======================================
// Specular
//---------------------------------------
Colour Shadowspot::Specular(Point3 P,Vector3 N,float shinyness)
{
  Vector3 L=to-from;
  Vector3 V=-P; // View vector
  float s,c;

  L.Normalise();
  V.Normalise();
  c=-N*L;
  s=(2*N*c+L)*V;
  s=MAX(s,0);
  return colour*(pow(s,shinyness)*LocalIntensity(P));
}

//=======================================
// Dump
//---------------------------------------
void Shadowspot::Dump()
{
  cout << *this;
}

//=======================================
// GenerateTransformations
//---------------------------------------
void Shadowspot::loadDepthMap(char *filename)
{
  FILE *fp=fopen(filename,"rb");
  int x,y;

  fprintf(stderr,"Opening `%s'\n",filename);
  if (!fp) RiError("Cannot open depth map `%s'",filename);
  fscanf(fp,"%d\n%d\n",&width,&height);
  cout << width << "," << height << "\n";
  tWorldToCamera.readBinary(fp);
  tCameraToRaster.readBinary(fp);
  depth=(float*)malloc(width*height*sizeof(float));
  if (!depth)
    RiError("Cannot reserve %d floats",width*height);
  fread(depth,sizeof(float),width*height,fp);
  fclose(fp);
}

//=======================================
//========STREAM INPUT/OUTPUT============
//=======================================

ostream &operator<<(ostream &io,const Shadowspot &l)
{
  cout << "from: " << l.from << " to:" << l.to << " Colour: " << l.colour << " Inten.: " << l.intensity;
}
