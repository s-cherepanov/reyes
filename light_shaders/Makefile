#------------------------------------------------------
# TITLE:  Makefile
# AUTHOR: Adrian Skilling
# ENVIR:  NetBSD
# DESC:   Light shaders makefile
# (C)     Copyright $Date$
# $Log$
#------------------------------------------------------

######################################
# Main rules                         #
######################################

reyes_dir = $(HOME)/src/reyes

LIBS = -I$(reyes_dir) -I$(reyes_dir)/light_shaders -I$(reyes_dir)/surface_shaders -I$(reyes_dir)/displacement_shaders

CC = gcc
CFLAGS = 

sources  = distantlight.cpp spotlight.cpp shadowspot.cpp

objs         = $(sources:%.cpp=%.o)

######################################
# Main rules                         #
######################################

.PHONY: clean

all: liblight_shaders.a

liblight_shaders.a: liblight_shaders.a($(objs))
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

liblight_shaders.a: *.h
