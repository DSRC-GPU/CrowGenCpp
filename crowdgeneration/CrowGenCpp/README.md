
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

To make this more intuitive, have a look at this example file:

    <crowd>
      <groupdescriptor gid="1">
        <population>50</population>
        <sources>
          <source type="spawn">
            <xborder start="0" end="100"></xborder>
            <yborder start="505" end="605"></yborder>
          </source>
        </sources>
        <sinks>
        </sinks>
        <map width="1000" height="1000" rows="4" cols="1">
          2
          2
          6
          8
        </map>
      </groupdescriptor>
      <groupdescriptor gid="2">
        <population>50</population>
        <sources>
          <source type="spawn">
            <xborder start="900" end="1000"></xborder>
            <yborder start="395" end="495"></yborder>
          </source>
        </sources>
        <sinks>
        </sinks>
        <map width="1000" height="1000" rows="4" cols="1">
          2
          4
          8
          8
        </map>
      </groupdescriptor>
    </crowd>

## Visualizing movement simulation

## Generating proximity graph

