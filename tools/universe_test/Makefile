TARGET=universe.bin
SRCS=$(wildcard *.cpp) ../../universe/src/Universe.cpp ../../universe/src/StarCategory.cpp
OBJS=$(SRCS:%.cpp=%.o)
CXXFLAGS  = -g -std=c++11
#CXXFLAGS += -Wall -Wextra -Werror -Wfatal-errors
CXXFLAGS += -Werror -Wfatal-errors
CXXFLAGS += -I/usr/include/freetype2
CXXFLAGS += -I../../utility/inc
CXXFLAGS += -I../../universe/inc
LIBS=-lglut -lGLU -lGL -lftgl -lrt

all: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c -o $*.o $<

clean:
	$(RM) $(TARGET) $(OBJS) *~ core* *.gch
