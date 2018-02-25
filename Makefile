TARGET=spaceGame
SRCS=$(wildcard *.cpp)
OBJS=$(SRCS:%.cpp=%.o)
FLAGS =-g
FLAGS += --std=c++11
FLAGS += -Wall -Wextra -Werror -Wfatal-errors
CXXLIBS= -lglfw -lGL

all: $(OBJS)
	$(CXX) $(FLAGS) -o $(TARGET) $(OBJS) $(CXXLIBS)

.cpp.o:
	$(CXX) $(FLAGS) -c -o $*.o $<

run:
	./$(TARGET) $(ARGS)

clean:
	$(RM) $(TARGET) *.o *~ core*
