/*------------------------------------------------------
 * TITLE:  microgrid.c++
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Grid of micropolygons class
 * (C)     Copyright $Date: 1999/04/01 19:59:59 $
 * $Log: microgrid.cpp,v $
 * Revision 1.5  1999/04/01 19:59:59  ady
 * *** empty log message ***
 *
 * Revision 1.4  1998/12/10 20:08:16  ady
 * Now uses Ri structures
 *
 * Revision 1.3  1997/11/29 18:02:41  ady
 * Uses new lights and calculate grid statistics
 *
 * Revision 1.2  1997/11/21 20:17:43  ady
 * Modified for new displacement shaders
 *
 * Revision 1.1  1997/11/16 14:15:10  ady
 * Initial revision
 *
 * Revision 1.3  1997/08/28 00:05:57  ady
 * Added displacement shader capability
 *
 * Revision 1.2  1997/03/10 21:53:29  ady
 * Now uses surface textures
 *
 * Revision 1.1  1997/02/23 22:55:40  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// C includes
#include <math.h>

// Reyes includes
#include "ri.h"
#include "microgrid.h"
#include "micropoly.h"
#include "colour.h"
#include "light.h"
#include "noise.h"
#include "useful.h"

//=======================================
// Constructor
//---------------------------------------
MicroGrid::MicroGrid(int width,int height)
{
  int x;

  this->Allocate(width,height);
  this->SetTextureCoords(0,0,1,1);
}

//=======================================
// Destructor
//---------------------------------------
MicroGrid::~MicroGrid()
{
  this->Free();
}

//=======================================
// SetSize
//---------------------------------------
void MicroGrid::SetSize(int width,int height)
{
  this->Free();
  this->Allocate(width,height);
}

//=======================================
// Allocate
//---------------------------------------
void MicroGrid::Allocate(int width,int height)
{
  int x;
  Point3 *point_block;
  Colour *colour_block;
  Opacity *opacity_block;
  Vector3 *normal_block;

  this->width=width;
  this->height=height;

  // Create 2D array of points
  point=new (Point3*)[width+1];
  point_block=new Point3[(width+1)*(height+1)];
  if (!point_block) RiError("Cannot reserve memory for point grid %d x %d",width+1,height+1);
  for(x=0;x<width+1;x++)
    point[x]=&point_block[x*(height+1)];

  // Create 2D array of colours
  colour=new (Colour*)[width];
  colour_block=new Colour[width*height];
  if (!point_block) RiError("Cannot reserve memory for colour grid %d x %d",width,height);
  for(x=0;x<width;x++)
    colour[x]=&colour_block[x*height];

  // Create 2D array of colours
  opacity=new (Opacity*)[width];
  opacity_block=new Opacity[width*height];
  if (!opacity_block) RiError("Cannot reserve memory for opacity grid %d x %d",width,height);
  for(x=0;x<width;x++)
    opacity[x]=&opacity_block[x*height];

  // Create 2D array of normals
  normal=new (Vector3*)[width];
  normal_block=new Vector3[(width+1)*(height+1)];
  if (!normal_block) RiError("Cannot reserve memory for normal grid %d x %d",width,height);
  for(x=0;x<width+1;x++)
    normal[x]=&normal_block[x*(height+1)];
}

//=======================================
// Free
//---------------------------------------
void MicroGrid::Free()
{
  int x;

  if (point)
  {
    delete[] point[0];
    delete[] point;
    point=NULL;
  }

  if (colour)
  {
    delete[] colour[0];
    delete[] colour;
    colour=NULL;
  }

  if (opacity)
  {
    delete[] opacity[0];
    delete[] opacity;
    opacity=NULL;
  }

  if (normal)
  {
    delete[] normal[0];
    delete[] normal;
    normal=NULL;
  }
}

//=======================================
//========STREAM INPUT/OUTPUT============
//=======================================

ostream &operator<<(ostream &io,const MicroGrid &m)
{
  int x,y;

  cout << "Width: " << m.width << " Height: " << m.height << " (in micro-polygons)\n";
  cout << "Points:\n";
  for(y=0;y<m.height+1;y++)
  {
    for(x=0;x<m.width+1;x++)
    {
      cout << m.point[x][y] << " ";
    }
    cout << "\n";
  }
  cout << "Colours:\n";
  for(y=0;y<m.height;y++)
  {
    for(x=0;x<m.width;x++)
    {
      cout << m.colour[x][y] << " ";
    }
    cout << "\n";
  }
  cout << "Opacities:\n";
  for(y=0;y<m.height;y++)
  {
    for(x=0;x<m.width;x++)
    {
      cout << m.opacity[x][y] << " ";
    }
    cout << "\n";
  }
  cout << "Normals:\n";
  for(y=0;y<m.height;y++)
  {
    for(x=0;x<m.width;x++)
    {
      cout << m.normal[x][y] << " ";
    }
    cout << "\n";
  }
}

void MicroGrid::WritePostscript(FILE *fp)
{
  int x,y;

  fp=fopen("microgrid.ps","w");
  for(x=0;x<width;x++)
  {
    fprintf(fp,"newpath\n");
    fprintf(fp,"%f %f moveto\n",point[x][0].x*200+300+point[x][0].z*50,point[x][0].y*200+300+point[x][0].z*50);
    for(y=1;y<=height;y++)
      fprintf(fp,"%f %f lineto\n",point[x][y].x*200+300+point[x][y].z*50,point[x][y].y*200+300+point[x][y].z*50);
    fprintf(fp,"closepath\n");
    fprintf(fp,"stroke\n");
  }
  for(y=0;y<height;y++)
  {
    fprintf(fp,"newpath\n");
    fprintf(fp,"%f %f moveto\n",point[0][y].x*200+300+point[0][y].z*50,point[0][y].y*200+300+point[0][y].z*50);
    for(x=1;x<=width;x++)
      fprintf(fp,"%f %f lineto\n",point[x][y].x*200+300+point[x][y].z*50,point[x][y].y*200+300+point[x][y].z*50);
    fprintf(fp,"closepath\n");
    fprintf(fp,"stroke\n");
  }
  fclose(fp);
}

//=======================================
// ExtractMicroPolygon
//---------------------------------------
bool MicroGrid::ExtractMicroPolygon(MicroPolygon &m,int u,int v)
{
  if ((normal[u][v]*point[u][v])<0)
  {
    m=MicroPolygon(point[u][v],point[u+1][v],point[u+1][v+1],point[u][v+1],colour[u][v],opacity[u][v]);
    return TRUE;
  }
  else
  {
    if (RiCurrent.geometryAttributes.sides==2)
    {
      m=MicroPolygon(point[u][v],point[u][v+1],point[u+1][v+1],point[u+1][v],colour[u][v],opacity[u][v]);
      return TRUE;
    }
  }
  return FALSE;
}

//=======================================
// ComputeNormals
//---------------------------------------
void MicroGrid::ComputeNormals()
{
  int u,v;
  Vector3 a,b,c,d;

  for(u=0;u<width;u++)
  {
    for(v=0;v<height;v++)
    {
      /* Check for co-incident vertices */
      a=point[u][v+1]-point[u][v];
      b=point[u+1][v]-point[u][v];
      c=point[u+1][v+1]-point[u+1][v];
      d=point[u+1][v+1]-point[u][v+1];
      if (c.Length()>a.Length()) a=c;
      if (d.Length()>b.Length()) b=d;
      normal[u][v]=a^b;
      normal[u][v].Normalise();
    }
  }
}


//=======================================
// Shade
//---------------------------------------
void MicroGrid::Shade(list<Light*> &lights)
{
  int u,v,i;
  float diffu,diffv;
  Vector3 N;
  list<Light*>::iterator iter;
  Light *light;
  Colour surf_colour;
  Opacity surf_opacity;
  Matrix4 tWorldToEye,tEyeToWorld;

  tWorldToEye=RiGlobal.tWorldToCamera;
  tEyeToWorld=tWorldToEye.Inverse();

  // cout << "WorldToEye (same as dice?):\n" << tWorldToEye << "\n";

  // cout << "Identity ?\n" << tWorldToEye*tEyeToWorld << "\n";

  // Set resolution from microgrid size
  RiCurrent.shadingAttributes.resolution=(int)logn(2,width/(umax-umin));

  // Initialise to black
  for(u=0;u<width;u++)
    for(v=0;v<height;v++)
      colour[u][v]=Colour(0,0,0);

  if (RiGlobal.options.verbosity>0)
  {
    cout << "Rendering microgrid (" << width << " x " << height << ")  ";
    cout << "s=[" << umin << ":" << umax << "] t=[" << vmin << ":" << vmax << "]\n";
  }

  diffu=(umax-umin);
  diffv=(vmax-vmin);
  iter=lights.begin();
  for(i=0;i<lights.size();i++)
  {
    light=*iter;
    for(u=0;u<width;u++)
    {
      for(v=0;v<height;v++)
      {
        if (RiCurrent.shadingAttributes.surface)
	{
	  wP=tEyeToWorld*point[u][v];
          N=((normal[u][v]*point[u][v])<0)?normal[u][v]:-normal[u][v];
          RiCurrent.shadingAttributes.surface->SurfaceColour(point[u][v],umin+(diffu*u)/width,vmin+(diffv*v)/height,N,light,surf_colour,surf_opacity);
	  colour[u][v]+=surf_colour;
	  opacity[u][v]=surf_opacity;
	}
	else
	{
          colour[u][v]=RiCurrent.shadingAttributes.colour;
	  opacity[u][v]=RiCurrent.shadingAttributes.opacity;
	}
      }
    }
    iter++;
  }
}

//=======================================
// Displace
//---------------------------------------
void MicroGrid::Displace()
{
  int u,v,uu,vv;
  float dx,dy,dz;
  float du,dv;

  if (RiCurrent.geometryAttributes.displacement)
  {
    du=(umax-umin)/width;
    dv=(vmax-vmin)/height;
    for(u=0;u<=width;u++)
    {
      for(v=0;v<=height;v++)
      {
        // normal used is per vertex
        point[u][v]=point[u][v]+normal[u][v]*RiCurrent.geometryAttributes.displacement->
                    Value(point[u][v],umin+du*u,vmin+dv*v,normal[u][v]);
      }
    }
    ComputeNormals();
  }
}

//=======================================
// Statistics
//---------------------------------------
void MicroGrid::Statistics(float &zmin,float &zmax,float &maxusize,float &maxvsize)
{
  int u,v;
  float udist,vdist;

  zmin=point[0][0].z;
  zmax=point[0][0].z;
  for(u=0;u<=width;u++)
  {
    for(v=0;v<=height;v++)
    {
      zmin=MIN(zmin,point[u][v].z);
      zmax=MAX(zmin,point[u][v].z);
    }
  }

  for(u=0;u<=width;u++)
  {
    for(v=0;v<=height;v++)
    {
      point[u][v].x=point[u][v].x/point[u][v].z;
      point[u][v].y=point[u][v].y/point[u][v].z;
    }
  }

  if (zmin>0)
  {
    maxusize=0;
    maxvsize=0;
    for(u=0;u<width;u++)
    {
      for(v=0;v<height;v++)
      {
	udist=(point[u+1][v].x-point[u][v].x)*(point[u+1][v].x-point[u][v].x)+
              (point[u+1][v].y-point[u][v].y)*(point[u+1][v].y-point[u][v].y);
	vdist=(point[u][v+1].x-point[u][v].x)*(point[u][v+1].x-point[u][v].x)+
              (point[u][v+1].y-point[u][v].y)*(point[u][v+1].y-point[u][v].y);
	maxusize=MAX(maxusize,udist);
	maxvsize=MAX(maxvsize,vdist);
      }
    }
    maxusize=sqrt(maxusize);
    maxvsize=sqrt(maxvsize);
  }
}











