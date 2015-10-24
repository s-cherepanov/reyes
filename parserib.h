#ifndef parserib_h
#define parserib_h
/*------------------------------------------------------
 * TITLE:  parserib.h
 * AUTHOR: Adrian Skilling
 * ENVIR:  Linux
 * DESC:   Read world (.wld) files from a file and create
 *         world object (hopefully!)
 * (C)     Copyright $Date: 1998/12/10 20:17:03 $
 * $Log: parserib.h,v $
 * Revision 1.1  1998/12/10 20:17:03  ady
 * Initial revision
 *
 * Revision 1.1  1997/02/23 23:00:16  ady
 * Initial revision
 *
 *-----------------------------------------------------*/

int yyerror(char *s);
int yylex(void);

/******************************************************/
/* Parserib                                          */
/******************************************************/
void ParseRIB(char *filename);

#endif
