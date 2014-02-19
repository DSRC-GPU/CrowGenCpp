DEBUGDEFS = -DDEBUG

# ----- Make Macros -----

CXXFLAGS = -Wall -Wextra -Wshadow -pedantic -O3 
CXX     = g++ $(DEBUGDEFS) -g

TARGETS = main 
OBJECTS = main.o Vertex.o Crowd.o


# ----- Make Rules -----

all:    $(TARGETS)

main:   $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o main $(OBJECTS) $(SDL)

clean:
	rm -f $(TARGETS) $(OBJECTS)

# ----8<----8<----8<----8<----8<----8<----8<----8<----8<----8<----8<----8<----

%.o: $@.cpp
