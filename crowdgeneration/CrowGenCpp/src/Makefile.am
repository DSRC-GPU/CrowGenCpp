
bin_PROGRAMS = cgc
cgc_SOURCES = main.cpp Crowd.cpp GroupDescriptor.cpp MapParser.cpp\
  CrowdGenerator.cpp MoveSimulator.cpp directions.cpp\
  tinyxml2-master/tinyxml2.cpp Box.cpp Point.cpp SimulationWriter.cpp\
  Vertex.cpp Edge.cpp CrowdParser.cpp GraphWriter.cpp\
  ProximityGraphGenerator.cpp MoveVisualizer.cpp
AUTOMAKE_OPTIONS = subdir-objects
cgc_LDFLAGS = -fopenmp -lpthread
cgc_CPPFLAGS = -std=c++11 -Wall -Wextra -Wshadow -pedantic -fopenmp
