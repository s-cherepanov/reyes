#------------------------------------------------------
# TITLE:  Makefile
# AUTHOR: Adrian Skilling
# ENVIR:  NetBSD
# DESC:   Reyes makefile
# (C)     Copyright $Date$
# $Log$
#------------------------------------------------------

######################################
# Main rules                         #
######################################

reyes_dir = $(HOME)/src/reyes

LIBS = -I$(reyes_dir) -I$(reyes_dir)/light_shaders -I$(reyes_dir)/surface_shaders -I$(reyes_dir)/displacement_shaders

CC = gcc
CFLAGS = -O3

sources  = surface.cpp

objs         = $(sources:%.cpp=%.o)

######################################
# Main rules                         #
######################################

.PHONY: clean

all: libsurface_shaders.a

libsurface_shaders.a: libsurface_shaders.a($(objs))
	ranlib $@

clean:
	-rm *.a *.o *.d core

######################################
# Default rules                      #
######################################
%.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@ $(LIBS)

######################################
# Dependency stuff                   #
######################################

include $(sources:.cpp=.d)

%.d: %.cpp
	$(SHELL) -ec '$(CC) $(CPPFLAGS) $(LIBS) -M $< | sed '\''s/$*.a/& $@/g'\'' > $@'

libsurface_shaders.a: *.h