#------------------------------------------------------
# TITLE:  Makefile
# AUTHOR: Adrian Skilling
# ENVIR:  Linux
# DESC:   Reyes makefile
# (C)     Copyright $Date: 1997/11/16 14:28:45 $
# $Log: Makefile,v $
# Revision 1.1  1997/11/16 14:28:45  ady
# Initial revision
#
#------------------------------------------------------

######################################
# Main rules                         #
######################################

INCLUDES = -I. -Isurface_shaders -Idisplacement_shaders -Ilight_shaders

CC     = gcc
CPP    = g++
CFLAGS = -W -ansi
LIBS   = -Lsurface_shaders -Ldisplacement_shaders -Llight_shaders -lg++ -lstdc++ -lm -lsurface_shaders -ldisplacement_shaders -llight_shaders

reyes_sources  = sphere.cpp cone.cpp cylinder.cpp disk.cpp torus.cpp \
                 paraboloid.cpp hyperboloid.cpp primitive.cpp \
                 micropoly.cpp depth.cpp vector3.cpp boundbox3.cpp microgrid.cpp \
                 rgbcolour.cpp colour.cpp camera.cpp \
                 matrix4.cpp noise.cpp parameter.cpp jitter.cpp \
                 image.cpp quaternion.cpp \
                 paramlist.cpp useful.cpp ri.cpp render.cpp \
                 filter.cpp framebuffer.cpp texturemap.cpp

parser_sources = parserib.y

reyes_objs         = $(reyes_sources:%.cpp=%.o) $(parser_sources:%.y=%.o)
sources            = $(reyes_sources)
objs               = $(sources:%.cpp=%.o)
reyes_libs         = surface_shaders/libsurface_shaders.a displacement_shaders/libdisplacement_shaders.a light_shaders/liblight_shaders.a


######################################
# Main rules                         #
######################################

BIN_DIR = /home/ady/bin
ifneq (,$(findstring -g,$(CFLAGS)))
  BIN_DIR = /home/ady/bin-debug
  LIB_DIR = /home/ady/lib-debug
else
  BIN_DIR = /home/ady/bin
  LIB_DIR = /home/ady/lib
endif

.PHONY: clean

all: reyes texture2pbm depthmap2pbm

reyes: reyes.o $(reyes_objs) $(reyes_libs)
	$(CC) $(CFLAGS) $^ -o $@ $(INCLUDES) $(LIBS)

texture2pbm: texture2pbm.o $(reyes_objs) $(reyes_libs)
	$(CC) $(CFLAGS) $^ -o $@ $(INCLUDES) $(LIBS)

depthmap2pbm: depthmap2pbm.o $(reyes_objs) $(reyes_libs)
	$(CC) $(CFLAGS) $^ -o $@ $(INCLUDES) $(LIBS)

clean:
	-rm *.o *.d parserib.cpp core reyes _matrix depthmap2pbm texture2pbm depthmap2pbm
	(cd surface_shaders; $(MAKE) clean)
	(cd displacement_shaders; $(MAKE) clean)
	(cd light_shaders; $(MAKE) clean)

parserib.cpp: parserib.y ri.h
	bison -v parserib.y -o $@

surface_shaders/libsurface_shaders.a: surface_shaders/*.h surface_shaders/*.cpp
	(cd surface_shaders; $(MAKE))
displacement_shaders/libdisplacement_shaders.a: displacement_shaders/*.h displacement_shaders/*.cpp
	(cd displacement_shaders; $(MAKE))
light_shaders/liblight_shaders.a: light_shaders/*.h light_shaders/*.cpp
	(cd light_shaders; $(MAKE))

######################################
# Make archive                       #
######################################

reyes.tar ::
	rm *.d
	tar --exclude "RCS/*" --exclude "RCS" --exclude "surface_shaders/RCS" --exclude "displacement_shaders/RCS" --exclude "light_shaders/RCS" --exclude "TODO" --exclude "reyes.tar" --exclude "archive" --exclude "web_version/*" -c * -f reyes.tar
	gzip reyes.tar

######################################
# Test programs                      #
######################################
_noise: _noise.o noise.o image.o
		$(CC) $(CFLAGS) $^ -o $@ $(INCLUDES) $(LIBS)

_texturemap: _texturemap.o $(reyes_objs) $(reyes_libs)
		$(CC) $(CFLAGS) $^ -o $@ $(INCLUDES) $(LIBS)

_matrix: _matrix.o $(reyes_objs) $(reyes_libs)
		$(CC) $(CFLAGS) $^ -o $@ $(INCLUDES) $(LIBS)

######################################
# Default rules                      #
######################################
%: %.o
	$(CC) $^ -o $@ -lg++ -lstdc++ -lm
%.o: %.cpp
	$(CPP) -c $(CFLAGS) $< -o $@ $(INCLUDES)
%.o: surface_shaders/%.cpp
	$(CPP) -c $(CFLAGS) $< -o $@ $(INCLUDES)
%.cpp: %.y
	bison $^ -o $@

######################################
# Dependency stuff                   #
######################################

include $(sources:.cpp=.d) parserib.d

%.d: %.cpp
	$(SHELL) -ec '$(CC) $(CPPFLAGS) $(INCLUDES) -M $< | sed '\''s/$*.o/& $@/g'\'' > $@'
%.d: surface_shaders/%.cpp
	$(SHELL) -ec '$(CC) $(CPPFLAGS) $(INCLUDES) -M $< | sed '\''s/$*.o/& $@/g'\'' > $@'
%.d: displacement_shaders/%.cpp
	$(SHELL) -ec '$(CC) $(CPPFLAGS) $(INCLUDES) -M $< | sed '\''s/$*.o/& $@/g'\'' > $@'
%.d: light_shaders/%.cpp
	$(SHELL) -ec '$(CC) $(CPPFLAGS) $(INCLUDES) -M $< | sed '\''s/$*.o/& $@/g'\'' > $@'
