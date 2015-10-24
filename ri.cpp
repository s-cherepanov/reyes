/*------------------------------------------------------
 * TITLE:  ri.cpp
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Renderman interface implementation
 * (C)     Copyright $Date: 1999/04/01 19:59:59 $
 * $Log: ri.cpp,v $
 * Revision 1.3  1999/04/01 19:59:59  ady
 * *** empty log message ***
 *
 * Revision 1.2  1998/12/10 20:11:53  ady
 * Added lots of RI functions for doing things
 *
 * Revision 1.1  1997/11/29 18:07:27  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// C includes
#include <string.h>
#include <stdarg.h>

// Reyes includes
#include "ri.h"
#include "quaternion.h"
#include "surface_shaders.h"
#include "displacement_shaders.h"
#include "light_shaders.h"
#include "sphere.h"
#include "disk.h"
#include "cylinder.h"
#include "cone.h"
#include "torus.h"
#include "hyperboloid.h"
#include "paraboloid.h"
#include "surface.h"
#include "shader.h"
#include "box2.h"
#include "useful.h"

/*------------------------------------------------------
/* Global variables
/*----------------------------------------------------*/
GlobalState RiGlobal;
CurrentState RiCurrent;
list<CurrentState> RiCurrentStack;
list<GlobalState> RiGlobalStack;
list<Matrix4> RiTransformStack;
list<Light*> lights;
list<Light*> arealights;

/* Shader accessable variables */
Point3 wP; // current point in world coords

/******************************************************/
/* Error handling                                     */
/******************************************************/

/*------------------------------------------------------
/* RiError
/*----------------------------------------------------*/
void RiError(const char *format,...)
{
  va_list args;

  va_start(args,format);
  vfprintf(stderr,format,args);
  va_end(args);
}

/******************************************************/
/* Structural commands                                */
/******************************************************/

/*------------------------------------------------------
/* RiBegin
/*----------------------------------------------------*/
void RiBegin(char* name){RiGlobal.RiBegin(name);}
void GlobalState::RiBegin(char* name)
{
}

/*------------------------------------------------------
/* RiEnd
/*----------------------------------------------------*/
void RiEnd(){RiGlobal.RiEnd();}
void GlobalState::RiEnd()
{
}

/*------------------------------------------------------
/* RiFrameBegin
/*----------------------------------------------------*/
void RiFrameBegin(int frame)
{
  RiGlobalStack.push_front(RiGlobal);
  RiGlobal.context.frame_block++;
}

/*------------------------------------------------------
/* RiFrameEnd
/*----------------------------------------------------*/
void RiFrameEnd()
{
  RiGlobal=*RiGlobalStack.begin();
  RiGlobalStack.pop_front();
  RiGlobal.context.frame_block--;
}

/*------------------------------------------------------
/* RiWorldBegin
/*----------------------------------------------------*/
void RiWorldBegin()
  {RiGlobal.RiWorldBegin();}
void GlobalState::RiWorldBegin()
{
  float width,height,xcent,ycent;
  float fovscale;

  framebuffer=new FrameBuffer(camera.xResolution*display.xSamplingRate,camera.yResolution*display.ySamplingRate,display.displayMode);

  // Calculate CameraToScreen transform (perspective transform done before this)
  if (camera.cameraProjection==RtPerspective)
    fovscale=1/atan(DEGTORAD*0.5*camera.fieldOfView);
  else
    fovscale=1;

  width=camera.screenWindow.xmax-camera.screenWindow.xmin;
  height=camera.screenWindow.ymax-camera.screenWindow.ymin;
  xcent=0.5*(camera.screenWindow.xmin+camera.screenWindow.xmax);
  ycent=0.5*(camera.screenWindow.ymin+camera.screenWindow.ymax);
  tCameraToScreen.Scale(Vector3(fovscale/width,fovscale/height,1));
  tCameraToScreen.Translate(Vector3(xcent,ycent,0));

  // Calculate ScreenToRaster transform
  xcent=0.5*camera.xResolution*display.xSamplingRate;
  ycent=0.5*camera.yResolution*display.ySamplingRate;
  tScreenToRaster.Scale(Vector3(0.5*camera.xResolution*display.xSamplingRate,0.5*camera.yResolution*display.ySamplingRate,1));
  tScreenToRaster.Translate(Vector3(1,1,0));

  // Concatenate for CameraToRaster transform
  tCameraToRaster=tScreenToRaster*tCameraToScreen;
  context.world_block++;
}

/*------------------------------------------------------
/* RiWorldEnd
/*----------------------------------------------------*/
void RiWorldEnd()
  {RiGlobal.RiWorldEnd();}
void GlobalState::RiWorldEnd()
{
  switch(display.displayType)
  {
    case RtFile:
      switch(framebuffer->DisplayMode())
      {
        case RtRGB:
        case RtRGBZ:
        case RtRGBAZ:
          framebuffer->WriteImageAsPBM(display.displayName);
	  break;
        case RtAZ:
        case RtZ:
          framebuffer->WriteDepthMap(display.displayName);
	  break;
      }
      break;
  }
  delete framebuffer;
  context.world_block--;
}

/*------------------------------------------------------
/* RiAttributeBegin
/*----------------------------------------------------*/
void RiAttributeBegin()
{
  RiCurrentStack.push_front(RiCurrent);
  RiGlobal.context.attribute_block++;
}

/*------------------------------------------------------
/* RiAttributeEnd
/*----------------------------------------------------*/
void RiAttributeEnd()
{
  RiCurrent=*RiCurrentStack.begin();
  RiCurrentStack.pop_front();
  RiGlobal.context.attribute_block--;
}

/*------------------------------------------------------
/* RiMotionBegin
/*----------------------------------------------------*/
void RiMotionBegin()
{
  RiGlobal.context.motion_block++;
}

/*------------------------------------------------------
/* RiMotionEnd
/*----------------------------------------------------*/
void RiMotionEnd()
{
  RiGlobal.context.motion_block--;
}

/*------------------------------------------------------
/* RiTransformBegin
/*----------------------------------------------------*/
void RiTransformBegin()
{
  RiTransformStack.push_front(RiCurrent.transform);
  RiGlobal.context.transform_block++;
}

/*------------------------------------------------------
/* RiTransformEnd
/*----------------------------------------------------*/
void RiTransformEnd()
{
  RiCurrent.transform=*RiTransformStack.begin();
  RiTransformStack.pop_front();
  RiGlobal.context.transform_block--;
}

/*------------------------------------------------------
/* RiSolidBegin
/*----------------------------------------------------*/
void RiSolidBegin()
{
  RiGlobal.context.solid_block++;
}

/*------------------------------------------------------
/* RiSolidEnd
/*----------------------------------------------------*/
void RiSolidEnd()
{
  RiGlobal.context.solid_block--;
}

/*------------------------------------------------------
/* RiObjectBegin
/*----------------------------------------------------*/
void RiObjectBegin()
{
  RiGlobal.context.object_block++;
}

/*------------------------------------------------------
/* RiObjectEnd
/*----------------------------------------------------*/
void RiObjectEnd()
{
  RiGlobal.context.object_block--;
}

/******************************************************/
/* Camera options                                     */
/******************************************************/

/*------------------------------------------------------
/* RiFormat
/*----------------------------------------------------*/
void RiFormat(int xResolution,int yResolution,float pixelAspectRatio)
  {RiGlobal.camera.RiFormat(xResolution,yResolution,pixelAspectRatio);}
void CameraOptions::RiFormat(int _xResolution,int _yResolution,float _pixelAspectRatio)
{
  xResolution=_xResolution;
  yResolution=_yResolution;
  pixelAspectRatio=_pixelAspectRatio;
  frameAspectRatio=(float)xResolution/yResolution;
  screenWindow.xmin=-frameAspectRatio;
  screenWindow.xmax=frameAspectRatio;
  screenWindow.ymin=-1;
  screenWindow.ymax=1;
}

/*------------------------------------------------------
/* RiFrameAspectRatio
/*----------------------------------------------------*/
void RiFrameAspectRatio(float frameAspectRatio)
  {RiGlobal.camera.RiFrameAspectRatio(frameAspectRatio);}
void CameraOptions::RiFrameAspectRatio(float _frameAspectRatio)
{
  frameAspectRatio=_frameAspectRatio;
}

/*------------------------------------------------------
/* RiScreenWindow
/*----------------------------------------------------*/
void RiScreenWindow(float left,float right,float bottom,float top)
  {RiGlobal.camera.RiScreenWindow(left,right,bottom,top);}
void CameraOptions::RiScreenWindow(float left,float right,float bottom,float top)
{
  screenWindow.xmin=left;
  screenWindow.xmax=right;
  screenWindow.ymin=bottom;
  screenWindow.ymax=top;
}

/*------------------------------------------------------
/* RiCropWindow
/*----------------------------------------------------*/
void RiCropWindow(float left,float right,float bottom,float top)
  {RiGlobal.camera.RiCropWindow(left,right,bottom,top);}
void CameraOptions::RiCropWindow(float left,float right,float bottom,float top)
{
  cropWindow.xmin=left;
  cropWindow.xmax=right;
  cropWindow.ymin=bottom;
  cropWindow.ymax=top;
}

/*------------------------------------------------------
/* RiProjection
/*----------------------------------------------------*/
void RiProjection(char *name,ParamList parameters){RiGlobal.camera.RiProjection(name,parameters);}
void CameraOptions::RiProjection(char *name,ParamList parameters)
{
  if (!strcmp(name,"orthographic"))
    cameraProjection=RtOrthographic;
  else
  {
    if (!strcmp(name,"perspective"))
    {
      cameraProjection=RtPerspective;
      if (parameters.Exists("fov"))
      {
	fieldOfView=parameters.Float("fov");
      }
    }
    else
      RiError("Projection type `%s' not understood",name);
  }
}

/*------------------------------------------------------
/* RiClipping
/*----------------------------------------------------*/
void RiClipping(float near,float far)
  {RiGlobal.camera.RiClipping(near,far);}
void CameraOptions::RiClipping(float near,float far)
{
  clippingNear=near;
  clippingFar=far;
}

/*------------------------------------------------------
/* RiDepthOfField
/*----------------------------------------------------*/
void RiDepthOfField(float fStop,float focalLength,float focalDistance)
  {RiGlobal.camera.RiDepthOfField(fStop,focalLength,focalDistance);}
void CameraOptions::RiDepthOfField(float _fStop,float _focalLength,float _focalDistance)
{
  fStop=_fStop;
  focalLength=_focalLength;
  focalDistance=_focalDistance;
}

/*------------------------------------------------------
/* RiShutter
/*----------------------------------------------------*/
void RiShutter(float min,float max)
  {RiGlobal.camera.RiShutter(min,max);}
void CameraOptions::RiShutter(float min,float max)
{
  shutterOpen=min;
  shutterClose=max;
}

/******************************************************/
/* Display options                                    */
/******************************************************/

/*------------------------------------------------------
/* RiPixelVariance
/*----------------------------------------------------*/
void RiPixelVariance(float variation)
  {RiGlobal.display.RiPixelVariance(variation);}
void DisplayOptions::RiPixelVariance(float variation)
{
  pixelVariance=variation;
}

/*------------------------------------------------------
/* RiPixelSamples
/*----------------------------------------------------*/
void RiPixelSamples(int xSamplingRate,int ySamplingRate)
{RiGlobal.display.RiPixelSamples(xSamplingRate,ySamplingRate);}
void DisplayOptions::RiPixelSamples(int _xSamplingRate,int _ySamplingRate)
{
  xSamplingRate=_xSamplingRate;
  ySamplingRate=_ySamplingRate;
}

/*------------------------------------------------------
/* RiPixelFilter
/*----------------------------------------------------*/
void RiPixelFilter(float (*filterfunc)(float,float,float,float),float xwidth,float ywidth)
  {RiGlobal.display.RiPixelFilter(filterfunc,xwidth,ywidth);}
void DisplayOptions::RiPixelFilter(float (*filterfunc)(float,float,float,float),float xwidth,float ywidth)
{
  filterFunction=filterfunc;
  filterXWidth=(int)xwidth;
  filterYWidth=(int)ywidth;
}

/*------------------------------------------------------
/* RiExposure
/*----------------------------------------------------*/
void RiExposure(float gain,float gamma)
  {RiGlobal.display.RiExposure(gain,gamma);}
void DisplayOptions::RiExposure(float _gain,float _gamma)
{
  gain=_gain;
  gamma=_gamma;
}

/*------------------------------------------------------
/* RiImager
/*----------------------------------------------------*/
void RiImager(char *name,ParamList parameters)
  {RiGlobal.display.RiImager(name,parameters);}
void DisplayOptions::RiImager(char *name,ParamList parameters)
{
}

/*------------------------------------------------------
/* RiQuantize
/*----------------------------------------------------*/
void RiQuantize(char *type,int one,int min,int max,float ditherAmplitude)
  {RiGlobal.display.RiQuantize(type,one,min,max,ditherAmplitude);}
void DisplayOptions::RiQuantize(char *type,int one,int min,int max,float ditherAmplitude)
{
  if (!strcmp(type,"rgb"))
  {
    colourQuantizer.RiQuantize(one,min,max,ditherAmplitude);
  }
  else
  {
    if (!strcmp(type,"z"))
      depthQuantizer.RiQuantize(one,min,max,ditherAmplitude);
    else
      cerr << "Unknown quantizer type `" << type << "'\n";
  }
}

void Quantizer::RiQuantize(int _one,int _minimum,int _maximum,float _ditherAmplitude)
{
  one=_one;
  minimum=_minimum;
  maximum=_maximum;
  ditherAmplitude=_ditherAmplitude;
}

/******************************************************/
/* Other options                                      */
/******************************************************/

/*------------------------------------------------------
/* RiOption
/*----------------------------------------------------*/
void RiOption(char *name,ParamList parameters)
  {RiGlobal.RiOption(name,parameters);}
void GlobalState::RiOption(char *name,ParamList parameters)
{
  if (!strcmp(name,"jitter"))
  {
    options.jitter=(parameters.Int("jitter")==1)?TRUE:FALSE;
  }
}

/*------------------------------------------------------
/* RiDisplay
/*----------------------------------------------------*/
void RiDisplay(char *name,char *type,char *mode,ParamList parameters)
  {RiGlobal.display.RiDisplay(name,type,mode,parameters);}
void DisplayOptions::RiDisplay(char *name,char *type,char *mode,ParamList parameters)
{
  displayMode=RtRGB;

  if (!strcmp(type,"framebuffer")) displayType=RtFrameBuffer;
  if (!strcmp(type,"file"))        displayType=RtFile;
  strcpy(displayName,name);
  if (!strcmp(mode,"rgb"))         displayMode=RtRGB;
  if (!strcmp(mode,"rgbz"))        displayMode=RtRGBZ;
  if (!strcmp(mode,"rgba"))        displayMode=RtRGBAZ;
  if (!strcmp(mode,"rgbaz"))       displayMode=RtRGBAZ;
  if (!strcmp(mode,"z"))           displayMode=RtZ;
}

/******************************************************/
/* Shading attributes                                 */
/******************************************************/

/*------------------------------------------------------
/* RiShadingRate
/*----------------------------------------------------*/
void RiShadingRate(float shadingRate)
{
  RiCurrent.shadingAttributes.shadingRate=shadingRate;
}

/*------------------------------------------------------
/* RiColour
/*----------------------------------------------------*/
void RiColour(Colour colour)
{
  RiCurrent.shadingAttributes.colour=colour;
}

/*------------------------------------------------------
/* RiOpacity
/*----------------------------------------------------*/
void RiOpacity(Opacity opacity)
{
  RiCurrent.shadingAttributes.opacity=opacity;
}

/******************************************************/
/* Primitive creation                                 */
/******************************************************/

/*------------------------------------------------------
/* RiSphere
/*----------------------------------------------------*/
void RiSphere(float radius,float zmin,float zmax,float thetamax)
{
  Sphere *sphere;
  ParamList parameters;

  sphere=new Sphere(radius,zmin,zmax,0,thetamax);
  RiGlobal.RenderPrimitive(sphere);
}

/*------------------------------------------------------
/* RiDisk
/*----------------------------------------------------*/
void RiDisk(float height,float radius,float thetamax)
{
  Disk *disk;

  disk=new Disk(height,radius,0,thetamax);
  RiGlobal.RenderPrimitive(disk);
}

/*------------------------------------------------------
/* RiCone
/*----------------------------------------------------*/
void RiCone(float height,float radius,float thetamax)
{
  Cone *cone;

  cone=new Cone(height,radius,0,thetamax,0,height);
  RiGlobal.RenderPrimitive(cone);
}

/*------------------------------------------------------
/* RiCylinder
/*----------------------------------------------------*/
void RiCylinder(float radius,float zmin,float zmax,float thetamax)
{
  Cylinder *cylinder;

  cylinder=new Cylinder(radius,zmin,zmax,0,thetamax);
  RiGlobal.RenderPrimitive(cylinder);
}

/*------------------------------------------------------
/* RiHyperboloid
/*----------------------------------------------------*/
void RiHyperboloid(Point3 point1,Point3 point2,float thetamax)
{
  Hyperboloid *hyperboloid;

  hyperboloid=new Hyperboloid(point1,point2,0,thetamax);
  RiGlobal.RenderPrimitive(hyperboloid);
}

/*------------------------------------------------------
/* RiParaboloid
/*----------------------------------------------------*/
void RiParaboloid(float rmax,float zmin,float zmax,float thetamax)
{
  Paraboloid *paraboloid;

  paraboloid=new Paraboloid(rmax,zmin,zmax,0,thetamax);
  RiGlobal.RenderPrimitive(paraboloid);
}

/*------------------------------------------------------
/* RiTorus
/*----------------------------------------------------*/
void RiTorus(float major_radius,float minor_radius,float phimin,float phimax,float thetamax)
{
  Torus *torus;

  torus=new Torus(major_radius,minor_radius,phimin,phimax,0,thetamax);
  RiGlobal.RenderPrimitive(torus);
}

/******************************************************/
/* Surface and displacement shader definition         */
/******************************************************/

/*------------------------------------------------------
/* RiSurface
/*----------------------------------------------------*/
void RiSurface(char *name,ParamList parameters)
{
  Surface *surface=NULL;

  if (!strcmp(name,"plastic"))    surface=new Plastic(parameters);
  if (!strcmp(name,"matte"))      surface=new Matte(parameters);
  if (!strcmp(name,"marble"))     surface=new Marble(parameters);
  if (!strcmp(name,"checker"))    surface=new Checker(parameters);
  if (!strcmp(name,"wood"))       surface=new Wood(parameters);
  if (!strcmp(name,"screen"))     surface=new Screen(parameters);
  if (!strcmp(name,"txtplastic")) surface=new TxtPlastic(parameters);

  if (surface)
  {
    RiCurrent.shadingAttributes.surface=surface;
  }
  else
    cerr << "Surface shader `" << name << "' does not exist\n";
}

/*------------------------------------------------------
/* RiLightSource
/*----------------------------------------------------*/
void RiLightSource(char *name,ParamList parameters)
  {RiCurrent.shadingAttributes.RiLightSource(name,parameters);}
void ShadingAttributes::RiLightSource(char *name,ParamList parameters)
{
  Light *light=NULL;

  if (!strcmp(name,"distantlight")) light=new Distantlight(parameters);
  if (!strcmp(name,"spotlight"))    light=new Spotlight(parameters);
  if (!strcmp(name,"shadowspot"))   light=new Shadowspot(parameters);

  if (light)
    lights.push_back(light);
  else
    cerr << "Light shader `" << name << "' does not exist\n";
}

/******************************************************/
/* Geometry commands                                  */
/******************************************************/

/*------------------------------------------------------
/* RiSides
/*----------------------------------------------------*/
void RiSides(int sides)
  {RiCurrent.geometryAttributes.RiSides(sides);}

/*------------------------------------------------------
/* RiDisplacement
/*----------------------------------------------------*/
void RiDisplacement(char *name,ParamList parameters)
{
  Displacement *displacement=NULL;
  bool exists=FALSE;

  if (!strcmp(name,"fractal")) {displacement=new Fractal(parameters); exists=TRUE;}
  if (!strcmp(name,"noisy"))   {displacement=new Noisy(parameters);   exists=TRUE;}
  if (!strcmp(name,"threads")) {displacement=new Threads(parameters); exists=TRUE;}

  RiCurrent.geometryAttributes.displacement=displacement;
  
  if (!exists) {cerr << "Displacement shader `" << name << "' does not exist\n";}
}

/*------------------------------------------------------
/* RiDisplacement
/*----------------------------------------------------*/
void RiDisplacement()
{
  RiCurrent.geometryAttributes.displacement=NULL;
}

/*------------------------------------------------------
/* RiTranslate
/*----------------------------------------------------*/
void RiTranslate(Vector3 v)
{
  if (RiGlobal.context.world_block>0)
    RiCurrent.transform.Translate(v);
  else
    RiGlobal.tWorldToCamera.Translate(v);
}

/*------------------------------------------------------
/* RiScale
/*----------------------------------------------------*/
void RiScale(Vector3 v)
{
  if (RiGlobal.context.world_block>0)
    RiCurrent.transform.Scale(v);
  else
    RiGlobal.tWorldToCamera.Scale(v);
}

/*------------------------------------------------------
/* RiRotate
/*----------------------------------------------------*/
void RiRotate(float angle,Vector3 v)
{
  Quaternion q;

  q.Rotation(angle,v);
  if (RiGlobal.context.world_block>0)
    RiCurrent.transform=RiCurrent.transform*q.RotationMatrix();
  else
    RiGlobal.tWorldToCamera=RiGlobal.tWorldToCamera*q.RotationMatrix();
}

/******************************************************/
/* Texture mapping                                    */
/******************************************************/

/*------------------------------------------------------
/* RiMakeTexture
/*----------------------------------------------------*/
void RiMakeTexture(char *imagefile,char *texturefile,int swrap,int twrap,
                   float (*filterfunc)(float,float,float,float),float swidth,float twidth)
{
  TextureMap texmap(imagefile,swrap,twrap,filterfunc,swidth,twidth);

  texmap.SaveTextureMap(texturefile);
}
