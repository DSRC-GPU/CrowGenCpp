
# CrowGenCpp

CrowGenCpp (cgc) is a small application for crowd generation, implemented in
cpp. This program has three main features:

 - Simulating random movement based on a move map.
 - Visualizing random movement simulation
 - Generating a proximity graph based on the simulated movement.

Each of these features is explained below.

## Simulating random movement

To simulate crowd movement, cgc requires a map file as input. This map file
should specify the groups present in the crowd, the direction of every group,
and the number of people in every group. Specifically, this means supplying an
xml file with the following data:

  - crowd -- root node
    - groupdescriptor -- one for every node
      - sources -- a container for source elements. These specify where people
        from this group should enter the crowd area.
        - source -- one for every area where people from this group can enter
          the crowd area.
      - sinks -- a container for sink elements. These specift where people from
        this group could enter the crowd area.
        - sink -- one for every area where people from this group can leave the
          crowd area.
      - map -- this element defines what direction people that belong to this
        group move in.

## Visualizing movement simulation

## Generating proximity graph

