/*------------------------------------------------------
 * TITLE:  reyes.cpp
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Reyes rendering system
 * (C)     Copyright $Date: 1999/04/01 20:32:48 $
 * $Log: reyes.cpp,v $
 * Revision 1.7  1999/04/01 20:32:48  ady
 * changed version number
 *
 * Revision 1.6  1999/04/01 19:59:59  ady
 * *** empty log message ***
 *
 * Revision 1.5  1998/12/10 20:11:28  ady
 * Changed options to make more Ri like
 *
 * Revision 1.4  1997/11/29 18:32:28  ady
 * Changed version number
 *
 * Revision 1.3  1997/11/29 18:07:27  ady
 * Now has verbosity option
 *
 * Revision 1.2  1997/11/21 20:17:43  ady
 * Uncommented noise initialisation
 *
 * Revision 1.1  1997/11/16 14:18:48  ady
 * Initial revision
 *
 * Revision 1.3  1997/08/28 00:05:57  ady
 * Added doublesided and shadows option
 *
 * Revision 1.2  1997/03/10 22:00:39  ady
 * Added jitter switch
 *
 * Revision 1.1  1997/02/23 22:58:14  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

// C++ includes

// C includes
#include <stdio.h>
#include <stdlib.h>

// Reyes includes
#include "ri.h"
#include "surface.h"
#include "camera.h"
#include "parserib.h"
#include "noise.h"
#include "jitter.h"

//=======================================
// Definitions
//---------------------------------------
#define PROG_NAME    "Reyes"
#define PROG_DESC    "High-quality 3d rendering"
#define PROG_VERSION "1.0.1"

//=======================================
// Global variables
//---------------------------------------
char *ribfile=NULL;

//=======================================
// usage
//---------------------------------------
void usage(void)
{
  int i;
  char buffer[256];

  sprintf(buffer,"%s Version %s - %s",PROG_NAME,PROG_VERSION,PROG_DESC);
  fprintf(stderr,"%s\n",buffer);
  for(i=0;i<strlen(buffer);i++)
    fprintf(stderr,"~");
  fprintf(stderr,"\n");
  fprintf(stderr,"Usage: reyes -i <ribfile>\n");
  fprintf(stderr,"-i <ribfile>             Filename of RIB file to render\n");
  exit(EXIT_FAILURE);
}

//=======================================
// usage
//---------------------------------------
void getoptions(int argc,char *argv[])
{
  int opt=1;

  while(opt<argc)
  {
    if (argv[opt][0]=='-')
    {
      switch(argv[opt][1])
      {
        case 'i':
	  ribfile=argv[opt+1];
	  opt+=2;
	  break;
        case 'v':
	  RiGlobal.options.verbosity=atoi(argv[opt+1]);
	  opt+=2;
	  break;
        default:
	  fprintf(stderr,"Cannot understand option `%s'\n",argv[opt]);
	  opt++;
	  break;
      }
    }
  }
}

//=======================================
// main
//---------------------------------------
int main(int argc,char *argv[])
{
  if (argc==1) usage();

  getoptions(argc,argv);

  /* Initialisation */
  initialise_noise();
  JitterInit();

  /* Load world */
  if (ribfile)
    ParseRIB(ribfile);
  else
    cerr << "You must specify a RIB file to render. You turnip !\n";

  return EXIT_SUCCESS;
}
