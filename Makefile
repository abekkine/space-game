TARGET=bin/spaceGame.bin

SRCS  = main.cpp
SRCS += $(wildcard src/*.cpp)
SRCS += $(wildcard systems/src/*.cpp)
SRCS += $(wildcard universe/src/*.cpp)
OBJS=$(SRCS:%.cpp=%.o)

# Flags
CXXFLAGS  = -std=c++11
CXXFLAGS += -g
#CXXFLAGS += -Wall
#CXXFLAGS += -Wextra
CXXFLAGS += -Werror
CXXFLAGS += -Wfatal-errors
CXXFLAGS += -I.
CXXFLAGS += -I./inc
CXXFLAGS += -I./systems/inc
CXXFLAGS += -I./universe/inc
CXXFLAGS += -I./3rd-party
CXXFLAGS += -I/usr/include/SDL
CXXFLAGS += -I/usr/include/libnoise
CXXFLAGS += -I/usr/include/freetype2
CXXFLAGS += -I/usr/include/Box2D

# Libraries
LIBS  =
LIBS += -lglfw
LIBS += -lGL
LIBS += -lSDL
LIBS += -lSDL_image
LIBS += -lftgl
LIBS += -lpthread
LIBS += -lBox2D
LIBS += -lnoise

VERSION_CMD = python git_version.py

all: Version.h $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c -o $*.o $<

Version.h:
	$(VERSION_CMD)

clean:
	$(RM) $(TARGET) $(OBJS) Version.h *~ core* *.gch
