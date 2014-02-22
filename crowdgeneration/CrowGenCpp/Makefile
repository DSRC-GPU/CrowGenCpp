DEBUGDEFS = -DDEBUG

# ----- Make Macros -----

CXXFLAGS = -std=c++11 -Wall -Wextra -Wshadow -pedantic -O3
CXX     = g++ $(DEBUGDEFS) -g

TARGETS = main 
OBJECTS = main.o Vertex.o Crowd.o GroupDescriptor.o MapParser.o CrowdGenerator.o\
 MoveSimulator.o directions.o tinyxml2-master/tinyxml2.o Box.o


# ----- Make Rules -----

all:    $(TARGETS)

main:   $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o main $(OBJECTS) $(SDL)

clean:
	rm -f $(TARGETS) $(OBJECTS)

# ----8<----8<----8<----8<----8<----8<----8<----8<----8<----8<----8<----8<----

%.o: $@.cpp
