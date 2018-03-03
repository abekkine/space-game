TARGET=spaceGame
SRCS=$(wildcard *.cpp)
OBJS=$(SRCS:%.cpp=%.o)
FLAGS =-g
FLAGS += -I./3rd-party
FLAGS += -I /usr/include/SDL
FLAGS += --std=c++11
FLAGS += -Wall -Wextra -Werror -Wfatal-errors
CXXLIBS= -lglfw -lGL -lSDL -lSDL_image

all: $(OBJS)
	$(CXX) $(FLAGS) -o $(TARGET) $(OBJS) $(CXXLIBS)

.cpp.o:
	$(CXX) $(FLAGS) -c -o $*.o $<

run:
	./$(TARGET) $(ARGS)

clean:
	$(RM) $(TARGET) *.o *.gch *~ core*
