TARGET=universe_gui.bin

SRCS  = $(wildcard *.cpp)
OBJS=$(SRCS:%.cpp=%.o)

# Flags
CXXFLAGS  = -std=c++11
CXXFLAGS += -g
# CXXFLAGS += -Wall
# CXXFLAGS += -Wextra
CXXFLAGS += -Werror
CXXFLAGS += -Wfatal-errors
CXXFLAGS += -I../../utility/inc
CXXFLAGS += -I$(EXTLIBS)/imgui/inc
CXXFLAGS += -L$(EXTLIBS)/imgui/lib

# Libraries
LIBS  =
LIBS += -lGL
LIBS += -lglut
LIBS += -limgui
LIBS += -lrt

all: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c -o $*.o $<

clean:
	$(RM) $(TARGET) $(OBJS) *~ core* *.gch
