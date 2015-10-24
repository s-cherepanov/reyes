/*------------------------------------------------------
 * TITLE:  parserib.y
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Read world (.wld) files from a file and create
 *         world object (hopefully!)
 * (C)     Copyright $Date: 1999/04/01 19:59:59 $
 * $Log: parserib.y,v $
 * Revision 1.2  1999/04/01 19:59:59  ady
 * *** empty log message ***
 *
 * Revision 1.1  1998/12/10 20:19:43  ady
 * Initial revision
 *
 * Revision 1.3  1997/11/23 10:43:00  ady
 * Added first parts of stacks based transformations and attributes
 *
 * Revision 1.2  1997/11/21 20:29:28  ady
 * Added transformations, new displacement shaders and new sphere creation
 * interface
 *
 * Revision 1.1  1997/11/16 14:27:26  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

%{
// C includes
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// Reyes includes
#include "parserib.h"
#include "ri.h"
%}

%union
{
  float num;
  char *string;
  ParamList *paramlist;
  float (*filter)(float,float,float,float);
  int wrap;
}

// Structural tokens
%token FRAMEBEGIN
%token FRAMEEND
%token WORLDBEGIN
%token WORLDEND
%token ATTRIBUTEBEGIN
%token ATTRIBUTEEND
%token MOTIONBEGIN
%token MOTIONEND
%token TRANSFORMBEGIN
%token TRANSFORMEND
%token SOLIDBEGIN
%token SOLIDEND
%token OBJECTBEGIN
%token OBJECTEND

// Options - before frame rendering
// Camera options
%token FORMAT
%token FRAMEASPECTRATIO
%token SCREENWINDOW
%token CROPWINDOW
%token PROJECTION
%token CLIPPING
%token DEPTHOFFIELD
%token SHUTTER
// Display options
%token PIXELVARIANCE
%token PIXELSAMPLES
%token PIXELFILTER
%token EXPOSURE
%token IMAGER
%token QUANTIZE
%token DISPLAY
// Additional options
%token HIDER
%token COLORSAMPLES
%token RELATIVEDETAIL
// Implementation specific
%token OPTION

// Shading attributes - during primitive definition
// color+opacity
%token COLOR
%token OPACITY
// texture coordinates
%token TEXTURECOORDINATES
// Surface shading
%token SURFACE
// General shading
%token SHADINGRATE
%token SHADINGINTERPOLATION
%token MATTE
// Light sources
%token LIGHTSOURCE
%token AREALIGHTSOURCE
%token ILLUMINATE
// Volume shading
%token ATMOSPHERE
%token INTERIOR
%token EXTERIOR

// Geometry attributes
// General
%token BOUND
%token DETAIL
%token DETAILRANGE
%token GEOMETRICAPPROXIMATION
%token ORIENTATION
%token REVERSEORIENTATION
%token SIDES
// Transformations
%token IDENTITY
%token TRANSFORM
%token CONCATTRANSFORM
%token PERSPECTIVE
%token TRANSLATE
%token ROTATE
%token SCALE
%token SKEW
%token DEFORMATION
%token DISPLACEMENT
%token COORDINATESYSTEM
%token TRANSFORMPOINTS
// Implementation specific
%token ATTRIBUTE

// Make functions
%token MAKETEXTURE

// Primitives
%token DISK
%token SPHERE
%token CYLINDER
%token CONE
%token TORUS
%token HYPERBOLOID
%token PARABOLOID

// Filter types
%token BOX
%token GAUSSIAN

// Wrap types
%token WRAPPERIODIC
%token WRAPCLAMP
%token WRAPBLACK

// Misc tokens
%token NUM
%token NAME

%type <num> NUM
%type <string> NAME
%type <filter> filter
%type <wrap> wrap

%type <paramlist> paramlist
%%

/******************************************************/
/* Grammar rules                                      */
/******************************************************/

input: commands {}
;

commands:   /* empty */
          | commands command
;

command:   structure
         | options
         | shadingattributes
         | geometryattributes
         | primitive
         | error
         | makefunctions
;

/* Structural/block commands */
structure:   FRAMEBEGIN NUM   {RiFrameBegin((int)$2)}
           | FRAMEEND         {RiFrameEnd()}
           | WORLDBEGIN       {RiWorldBegin()}
           | WORLDEND         {RiWorldEnd()}
           | ATTRIBUTEBEGIN   {RiAttributeBegin()}
           | ATTRIBUTEEND     {RiAttributeEnd()}
           | MOTIONBEGIN      {RiMotionBegin()}
           | MOTIONEND        {RiMotionEnd()}
           | TRANSFORMBEGIN   {RiTransformBegin()}
           | TRANSFORMEND     {RiTransformEnd()}
           | SOLIDBEGIN       {RiSolidBegin()}
           | SOLIDEND         {RiSolidEnd()}
           | OBJECTBEGIN      {RiObjectBegin()}
           | OBJECTEND        {RiObjectEnd()}
;

options:   camera
         | display
         | OPTION '"' NAME '"' paramlist    {RiOption($3,*$5)}
;

camera:   FORMAT NUM NUM NUM                {RiFormat((int)$2,(int)$3,$4)}
        | FRAMEASPECTRATIO NUM              {RiFrameAspectRatio($2)}
        | SCREENWINDOW NUM NUM NUM NUM      {RiScreenWindow($2,$3,$4,$5)}
        | CROPWINDOW NUM NUM NUM NUM        {RiCropWindow($2,$3,$4,$5)}
        | PROJECTION '"' NAME '"' paramlist {RiProjection($3,*$5)}
;

display:   PIXELVARIANCE NUM                                        {RiPixelVariance($2)}
         | PIXELSAMPLES NUM NUM                                     {RiPixelSamples((int)$2,(int)$3)}
         | PIXELFILTER '"' filter '"' NUM NUM                       {RiPixelFilter($3,$5,$6)}
         | EXPOSURE NUM NUM                                         {RiExposure($2,$3)}
         | IMAGER '"' NAME '"' paramlist                            {RiImager($3,*$5)}
         | QUANTIZE '"' NAME '"' NUM NUM NUM NUM                    {RiQuantize($3,(int)$5,(int)$6,(int)$7,$8)}
         | DISPLAY '"' NAME '"' '"' NAME '"' '"' NAME '"' paramlist {RiDisplay($3,$6,$9,*$11)}
;

/* Shading attributes */
shadingattributes:   colorandopacity
                   | lights
                   | SURFACE '"' NAME '"' paramlist      {RiSurface($3,*$5)}
                   | SHADINGRATE NUM                     {RiShadingRate($2)}
;

colorandopacity:   COLOR '[' NUM NUM NUM ']'             {RiColour(Colour($3,$4,$5));}
                 | OPACITY '[' NUM NUM NUM ']'           {RiOpacity(Opacity($3,$4,$5));}
;

lights:  LIGHTSOURCE '"' NAME '"' paramlist            {RiLightSource($3,*$5);}
;

/* Geometry attributes */
geometryattributes:   transformations
                    | SIDES NUM                        {RiSides((int)$2);}
;

/* Primitive definition */
primitive:   DISK NUM NUM NUM                          {RiDisk($2,$3,$4);}
           | SPHERE NUM NUM NUM NUM                    {RiSphere($2,$3,$4,$5);}
           | CYLINDER NUM NUM NUM NUM                  {RiCylinder($2,$3,$4,$5);}
           | CONE NUM NUM NUM                          {RiCone($2,$3,$4);}
           | TORUS NUM NUM NUM NUM NUM                 {RiTorus($2,$3,$4,$5,$6);}
           | HYPERBOLOID NUM NUM NUM NUM NUM NUM NUM   {RiHyperboloid(Point3($2,$3,$4),Point3($5,$6,$7),$8);}
           | PARABOLOID NUM NUM NUM NUM                {RiParaboloid($2,$3,$4,$5);}
;

paramlist:   /* empty */                               {$$=new ParamList();}
           | paramlist '"' NAME '"'                    {$$->AddParameter(Parameter($3));}
           | paramlist NUM                             {$$->AddParameter(Parameter($2));}
           | paramlist '[' NUM NUM NUM ']'             {$$->AddParameter(Parameter(Vector3($3,$4,$5)));}
;

transformations:    ROTATE NUM NUM NUM NUM             {RiRotate($2,Vector3($3,$4,$5))}
                 | SCALE NUM NUM NUM                   {RiScale(Vector3($2,$3,$4))}
                 | TRANSLATE NUM NUM NUM               {RiTranslate(Vector3($2,$3,$4))}
                 | DISPLACEMENT '"' NAME '"' paramlist {RiDisplacement($3,*$5)}
                 | DISPLACEMENT '"' '"'                {RiDisplacement()}
;

makefunctions: MAKETEXTURE '"' NAME '"' '"' NAME '"' '"' wrap '"' '"' wrap '"' '"' filter '"' NUM NUM {RiMakeTexture($3,$6,$9,$12,$15,$17,$18)}
;

filter:  BOX             {$$=RiBoxFilter}
       | GAUSSIAN        {$$=RiGaussianFilter}
;

wrap:  WRAPPERIODIC      {$$=RI_PERIODIC}
     | WRAPCLAMP         {$$=RI_CLAMP}
     | WRAPBLACK         {$$=RI_BLACK}
;

%%

/******************************************************/
/* Global variables                                   */
/******************************************************/
FILE *fp_rib=NULL;

/******************************************************/
/* yyerror                                            */
/******************************************************/
int yyerror (char *s)
{
  printf ("%s\n", s);
}

/******************************************************/
/* iswhitespace                                       */
/******************************************************/
int iswhitespace(int c)
{
  return (c==' ')||(c=='\n')||(c=='\t');
}

/******************************************************/
/* isnumber                                           */
/******************************************************/
int isnumber(int c)
{
  return (isdigit(c)||(c=='.')||(c=='-'));
}

int myfgetc(FILE *stream)
{
  int c;
  c=fgetc(stream);
  if (!feof(stream)) putchar(c);
  return c;
}

/******************************************************/
/* yylex                                              */
/******************************************************/
int yylex(void)
{
  int c;
  char buffer[32];
  int i=0;
     
  do
  {
    c=myfgetc(fp_rib);
    /* Handle single char types we might see */
    switch(c)
    {
      case '{':
      case '}':
      case ',':
      case '(':
      case ')':
      case '=':
	return c;
	break;
      case '#':
	c=myfgetc(fp_rib);
	while((c!=EOF)&&(c!='\n'))
	{
	  c=myfgetc(fp_rib);
	}
	break;
    }
  }
  while(iswhitespace(c));

  /* Get chars up until white space */
  if (isalpha(c))
  {
    while(isupper(c)||islower(c)||isdigit(c)||(c=='.')){buffer[i++]=c; c=myfgetc(fp_rib);}
    if (!iswhitespace(c)) ungetc(c,fp_rib);
    buffer[i]='\0';
    /* Block begin/end */
    if (!strcmp(buffer,"WorldBegin"))       return WORLDBEGIN;
    if (!strcmp(buffer,"WorldEnd"))         return WORLDEND;
    if (!strcmp(buffer,"FrameBegin"))       return FRAMEBEGIN;
    if (!strcmp(buffer,"FrameEnd"))         return FRAMEEND;
    if (!strcmp(buffer,"AttributeBegin"))   return ATTRIBUTEBEGIN;
    if (!strcmp(buffer,"AttributeEnd"))     return ATTRIBUTEEND;
    if (!strcmp(buffer,"TransformBegin"))   return TRANSFORMBEGIN;
    if (!strcmp(buffer,"TransformEnd"))     return TRANSFORMEND;
    /* Primitives */
    if (!strcmp(buffer,"Disk"))             return DISK;
    if (!strcmp(buffer,"Sphere"))           return SPHERE;
    if (!strcmp(buffer,"Cylinder"))         return CYLINDER;
    if (!strcmp(buffer,"Cone"))             return CONE;
    if (!strcmp(buffer,"Torus"))            return TORUS;
    if (!strcmp(buffer,"Hyperboloid"))      return HYPERBOLOID;
    if (!strcmp(buffer,"Paraboloid"))       return PARABOLOID;
    /* Geometry attributes */
    if (!strcmp(buffer,"Sides"))            return SIDES;
    if (!strcmp(buffer,"Translate"))        return TRANSLATE;
    if (!strcmp(buffer,"Rotate"))           return ROTATE;
    if (!strcmp(buffer,"Scale"))            return SCALE;
    /* Surface attributes */
    if (!strcmp(buffer,"ShadingRate"))      return SHADINGRATE;
    /* Surface and light tokens */
    if (!strcmp(buffer,"Color"))            return COLOR;
    if (!strcmp(buffer,"Opacity"))          return OPACITY;
    if (!strcmp(buffer,"Surface"))          return SURFACE;
    if (!strcmp(buffer,"LightSource"))      return LIGHTSOURCE;
    if (!strcmp(buffer,"Surface"))          return SURFACE;
    if (!strcmp(buffer,"Displacement"))     return DISPLACEMENT;
    /* Camera options */
    if (!strcmp(buffer,"Format"))           return FORMAT;
    if (!strcmp(buffer,"CropWindow"))       return CROPWINDOW;
    if (!strcmp(buffer,"ScreenWindow"))     return SCREENWINDOW;
    if (!strcmp(buffer,"FrameAspectRatio")) return FRAMEASPECTRATIO;
    if (!strcmp(buffer,"Projection"))       return PROJECTION;
    /* Display options */
    if (!strcmp(buffer,"PixelVariance"))    return PIXELVARIANCE;
    if (!strcmp(buffer,"PixelSamples"))     return PIXELSAMPLES;
    if (!strcmp(buffer,"PixelFilter"))      return PIXELFILTER;
    if (!strcmp(buffer,"Exposure"))         return EXPOSURE;
    if (!strcmp(buffer,"Imager"))           return IMAGER;
    if (!strcmp(buffer,"Quantize"))         return QUANTIZE;
    if (!strcmp(buffer,"Display"))          return DISPLAY;
    /* Additional options */
    if (!strcmp(buffer,"Option"))           return OPTION;
    /* Filter types */
    if (!strcmp(buffer,"box"))              return BOX;
    if (!strcmp(buffer,"gaussian"))         return GAUSSIAN;
    /* Making functions */
    if (!strcmp(buffer,"MakeTexture"))      return MAKETEXTURE;
    /* Wrap types */
    if (!strcmp(buffer,"periodic"))         return WRAPPERIODIC;
    if (!strcmp(buffer,"black"))            return WRAPBLACK;
    if (!strcmp(buffer,"clamp"))            return WRAPCLAMP;

    yylval.string=(char*)calloc(strlen(buffer)+1,1);
    strcpy(yylval.string,buffer);
    return NAME;
  }
  else
  {
    if (isnumber(c))
    {
      while(isnumber(c)){buffer[i++]=c; c=myfgetc(fp_rib);}
      if (!iswhitespace(c)) ungetc(c,fp_rib);
      buffer[i]='\0';
      yylval.num=atof(buffer);
      return NUM;
    }
  }
  if (c==EOF) return 0;
  return c;
}

/******************************************************/
/* ParseRIB                                           */
/******************************************************/
void ParseRIB(char *filename)
{
  fp_rib=fopen(filename,"r");
  int success;

  if (fp_rib)
  {
#ifdef YYDEBUG
    yydebug=1;
#endif
    RiBegin("");
    success=yyparse();
    fclose(fp_rib);
    RiEnd();
  }
  else
    fprintf(stderr,"Cannot open RIB file `%s'\n",filename);
}
