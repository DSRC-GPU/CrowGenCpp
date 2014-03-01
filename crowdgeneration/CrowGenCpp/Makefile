DEBUGDEFS = -DDEBUG

# ----- Make Macros -----

CXXFLAGS = -std=c++11 -Wall -Wextra -Wshadow -pedantic -O3
CXX     = g++ $(DEBUGDEFS) -g

TARGETS = main 
OBJECTS = main.o Node.o Crowd.o GroupDescriptor.o MapParser.o CrowdGenerator.o\
 MoveSimulator.o directions.o tinyxml2-master/tinyxml2.o Box.o Point.o\
 SimulationWriter.o Vertex.o Edge.o CrowdParser.o GraphWriter.o


# ----- Make Rules -----

all:    $(TARGETS)

main:   $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o main $(OBJECTS) $(SDL)

fast:
	make -j 8

clean:
	rm -f $(TARGETS) $(OBJECTS)

# ----8<----8<----8<----8<----8<----8<----8<----8<----8<----8<----8<----8<----

%.o: $@.cpp
