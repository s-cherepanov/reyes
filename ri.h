#ifndef ri_h
#define ri_h
/*------------------------------------------------------
 * TITLE:  ri.h
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Renderman interface prototypes and data structures
 * (C)     Copyright $Date: 1999/04/01 19:59:59 $
 * $Log: ri.h,v $
 * Revision 1.3  1999/04/01 19:59:59  ady
 * *** empty log message ***
 *
 * Revision 1.2  1998/12/10 20:17:55  ady
 * Added loads of Ri functions
 *
 * Revision 1.1  1997/11/29 18:12:35  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// C++ includes
#include <list.h>

// C includes
#include <limits.h>
#include <math.h>

// Reyes includes
#include "shader.h"
#include "paramlist.h"
#include "colour.h"
#include "opacity.h"
#include "box2.h"
#include "matrix4.h"
#include "surface.h"
#include "displacement.h"
#include "light.h"
#include "primitive.h"
#include "camera.h"
#include "filter.h"
#include "framebuffer.h"
#include "useful.h"

/* Definitions */

#define RI_EPSILON     0.01
#define RI_INFINITY    MAXFLOAT

#define RI_NULL        NULL

typedef enum
{
  RtOrthographic,
  RtPerspective
}ProjectionType;

typedef enum
{
  RtFrameBuffer,
  RtFile
}DisplayType;

typedef enum
{
  RtGaussianFilter
}FilterType;

/*------------------------------------------------------
 - Camera options
 -----------------------------------------------------*/
class CameraOptions
{
  public:
    CameraOptions()
      {
	xResolution=640; yResolution=480;
        pixelAspectRatio=1;
        frameAspectRatio=(float)xResolution/yResolution;
	screenWindow=Box2<float>(-frameAspectRatio,-1,frameAspectRatio,1);
	cropWindow=Box2<float>(-1,-1,1,1);
        cameraProjection=RtOrthographic;
	fieldOfView=90;
        clippingNear=0.1;
        clippingFar=10000;
        fStop=RI_INFINITY;
        focalLength=0;
        focalDistance=0;
        shutterOpen=0;
        shutterClose=0;
      }
    void RiFormat(int _xResolution,int _yResolution,float _pixelAspectRatio);
    void RiCropWindow(float left,float right,float bottom,float top);
    void RiScreenWindow(float left,float right,float bottom,float top);
    void RiFrameAspectRatio(float _frameAspectRatio);
    void RiProjection(char *name,ParamList parameters);
    void RiClipping(float near,float far);
    void RiDepthOfField(float _fStop,float _focalLength,float _focalDistance);
    void RiShutter(float min,float max);

    int xResolution,yResolution;
    float pixelAspectRatio;
    Box2<float> cropWindow;
    float frameAspectRatio;
    Box2<float> screenWindow;
    ProjectionType cameraProjection;
    float fieldOfView;
    Matrix4 worldToCamera;
    float clippingNear;
    float clippingFar;
    float fStop;
    float focalLength;
    float focalDistance;
    float shutterOpen;
    float shutterClose;
};

/*------------------------------------------------------
 Quantizer options
------------------------------------------------------*/
class Quantizer
{
  public:
    void RiQuantize(int one,int minimum,int maximum,float ditherAmplitude);

    int one;
    int minimum;
    int maximum;
    float ditherAmplitude;
};

/*------------------------------------------------------
 Display options
------------------------------------------------------*/
class DisplayOptions
{
  public:
    DisplayOptions()
    {
      pixelVariance=1;
      xSamplingRate=1;
      ySamplingRate=1;
      filterFunction=RiBoxFilter;
      filterXWidth=1;
      filterYWidth=1;
      gamma=1;
      gain=1;
      colourQuantizer.RiQuantize(255,0,255,0.5);
      depthQuantizer.RiQuantize(0,-1,-1,-1);
      displayType=RtFile;
      strcpy(displayName,"image.pbm");
      displayMode=RtRGBZ;
    }
    void RiPixelVariance(float variation);
    void RiPixelSamples(int xSamplingRate,int ySamplingRate);
    void RiPixelFilter(float (*)(float,float,float,float),float xwidth,float ywidth);
    void RiExposure(float gain,float gamma);
    void RiImager(char *name,ParamList parameters);
    void RiQuantize(char *type,int one,int min,int max,float ditherAmplitude);
    void RiDisplay(char *name,char *type,char *mode,ParamList parameters);

    float pixelVariance;
    int xSamplingRate,ySamplingRate;
    FilterType filter;
    float (*filterFunction)(float,float,float,float);
    int filterXWidth,filterYWidth;
    float gamma;
    float gain;
    Quantizer colourQuantizer;
    Quantizer depthQuantizer;
    DisplayType displayType;
    char displayName[256];
    RtDisplayMode displayMode;
}; 

/*------------------------------------------------------
 Shading attributes
------------------------------------------------------*/
class ShadingAttributes
{
  public:
    ShadingAttributes()
    {
      colour=Colour(1,1,1);
      opacity=Opacity(1,1,1);
      smin=0; tmin=0; smax=1; tmax=1;
      shadingRate=4;
      surface=NULL;
      resolution=0;
    }

    void RiLightSource(char *name,ParamList parameters);

    Colour colour;
    Opacity opacity;
    float smin,tmin,smax,tmax;
    float shadingRate;          /* Smallest micro-polygon dimensions in pixels */
    Surface *surface;
    int resolution;             /* Required resolution in texture map (size 1<<resolution) */
};

/*------------------------------------------------------
 Geometry attributes
------------------------------------------------------*/
class GeometryAttributes
{
  public:
    GeometryAttributes()
    {
      sides=1;
      displacement=NULL;
    }

    void RiSides(int _sides)
      {sides=_sides;}

    int sides;
    Displacement *displacement;
};

/*------------------------------------------------------
 Options
------------------------------------------------------*/
class Options
{
  public:
    Options()
    {
      verbosity=0;
      maxMicroGridSize=10000;
      jitter=FALSE;
    }

    int verbosity;
    int maxMicroGridSize;
    bool jitter;
};

/*------------------------------------------------------
 Context
------------------------------------------------------*/
class Context
{
  public:
    Context()
    {
      frame_block=0;
      world_block=0;
      attribute_block=0;
      solid_block=0;
      object_block=0;
      motion_block=0;
    }

    int frame_block;
    int world_block;
    int attribute_block;
    int transform_block;
    int solid_block;
    int object_block;
    int motion_block;
};

/*------------------------------------------------------
 Global graphics state
------------------------------------------------------*/
class GlobalState
{
  public:
    void RiBegin(char* name);
    void RiEnd();
    void RiWorldBegin();
    void RiWorldEnd();
    void RenderPrimitive(Primitive *primitive);
    void RiOption(char *name,ParamList parameters);

  public:
    Context context;
    CameraOptions camera;
    DisplayOptions display;
    FrameBuffer *framebuffer;
    /* Transformations */
    Matrix4 tWorldToCamera;  // Camera space= vantage point at origin, view along Z axis
    Matrix4 tCameraToScreen; // Screen space= 2-D normalized co-ordinate system corresponding to image plane
    Matrix4 tScreenToRaster; // Raster space= Pixel co-ordinates space (0,0) is bottom left
    // Concatenated transform
    Matrix4 tCameraToRaster;
    Options options;
};

/*------------------------------------------------------
 Current graphics state
------------------------------------------------------*/
class CurrentState
{
  public:
    ShadingAttributes shadingAttributes;
    GeometryAttributes geometryAttributes;
    Matrix4 transform;                   // Object to World space transform
};

/*------------------------------------------------------
 Global variables
------------------------------------------------------*/
GLOBAL GlobalState RiGlobal;
GLOBAL CurrentState RiCurrent;
GLOBAL list<Light*> lights;
GLOBAL list<Light*> arealights;

/* Shader accessable variables */
GLOBAL Point3 wP;

/******************************************************/
/* Error handling                                     */
/******************************************************/

void RiError(const char *format,...);

/******************************************************/
/* Structural commands                                */
/******************************************************/

void RiBegin(char* name);
void RiEnd();
void RiFrameBegin(int frame);
void RiFrameEnd();
void RiWorldBegin();
void RiWorldEnd();
void RiAttributeBegin();
void RiAttributeEnd();
void RiMotionBegin();
void RiMotionEnd();
void RiTransformBegin();
void RiTransformEnd();
void RiSolidBegin();
void RiSolidEnd();
void RiObjectBegin();
void RiObjectEnd();

/******************************************************/
/* Error handling                                     */
/******************************************************/

void RiError(const char *format,...);

/******************************************************/
/* Camera options                                     */
/******************************************************/

void RiFormat(int xResolution,int yResolution,float pixelAspectRatio);
void RiFrameAspectRatio(float frameAspectRatio);
void RiCropWindow(float left,float right,float bottom,float top);
void RiScreenWindow(float left,float right,float bottom,float top);
void RiProjection(char *name,ParamList parameters);
void RiClipping(float near,float far);
void RiDepthOfField(float _fStop,float _focalLength,float _focalDistance);
void RiShutter(float min,float max);

/******************************************************/
/* Display options                                    */
/******************************************************/

void RiPixelVariance(float variation);
void RiPixelSamples(int xSamplingRate,int ySamplingRate);
void RiPixelFilter(float (*)(float,float,float,float),float xwidth,float ywidth);
void RiExposure(float gain,float gamma);
void RiImager(char *name,ParamList parameters);
void RiQuantize(char *type,int one,int min,int max,float ditherAmplitude);
void RiDisplay(char *name,char *type,char *mode,ParamList parameters);

/******************************************************/
/* Shading attributes                                 */
/******************************************************/

void RiShadingRate(float shadingRate);
void RiColour(Colour colour);
void RiOpacity(Opacity opacity);

/******************************************************/
/* Lighting attributes                                */
/******************************************************/

void RiLightSource(char *name,ParamList parameters);

/******************************************************/
/* Primitive creation                                 */
/******************************************************/

void RiSphere(float radius,float zmin,float zmax,float thetamax);
void RiDisk(float height,float radius,float thetamax);
void RiCone(float height,float radius,float thetamax);
void RiCylinder(float radius,float zmin,float zmax,float thetamax);
void RiHyperboloid(Point3 point1,Point3 point2,float thetamax);
void RiParaboloid(float rmax,float zmin,float zmax,float thetamax);
void RiTorus(float major_radius,float minor_radius,float phimin,float phimax,float thetamax);

/******************************************************/
/* Surface shading attributes                         */
/******************************************************/

void RiSurface(char *name,ParamList parameters);

/******************************************************/
/* Geometry commands                                  */
/******************************************************/

void RiDisplacement(char *name,ParamList parameters);
void RiDisplacement();
void RiSides(int sides);
void RiTranslate(Vector3 v);
void RiScale(Vector3 v);
void RiRotate(float angle,Vector3 v);

/******************************************************/
/* Other options                                      */
/******************************************************/

void RiOption(char *name,ParamList parameters);

/******************************************************/
/* Texture mapping                                    */
/******************************************************/

void RiMakeTexture(char *imagefile,char *texturefile,int swrap,int twrap,
                   float (*filterfunc)(float,float,float,float),float swidth,float twidth);
#endif
