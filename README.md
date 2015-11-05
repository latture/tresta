Tresta
======

Getting started
---------------
The project requires Qt and CMake. If not installed, install both before continuing.

### To compile the code: ###
  1. Open the `tresta` root directory.
  2. create a folder named `build`.
  3. Open a terminal and navigate to the newly formed build directory.
  4. Execute `cmake .. -DCMAKE_BUILD_TYPE=release`.
    * Use `-DCMAKE_BUILD_TYPE=debug` if you would like to build the code for debugging purposes.
    * `-DCMAKE_PREFIX_PATH=/path/to/qt` tells CMake where to find the Qt installation.
      For example, on my computer this would be set to `/home/ryan/Qt/5.5/gcc_64`.
  5. On Linux run `make` in the terminal from the build directory to build all the targets. On Windows the solution file will be located in the build directory. Open the solution file in Visual Studio and compile.
  6. Upon successful compilation the tresta binary should be located in `build/bin/`

Introduction
------------
This program is used for visualization of 3D truss structures.
A truss is defined using CSV files to specify nodal positions,
connections between nodes, and (optionally) displacements.
The path to each of these files is specified using a JSON document.
For example,

    {
        "nodes" : "/path/to/nodes.csv"
        "elems" : "/path/to/elems.csv"
        "displacements" : "/path/to/displacements.csv"
    }

The `"nodes"` and `"elems"` keys must be present. If `"displacements"` are
provided then both the original and deformed truss structure is rendered.
This configuration file is used by tresta to load the structure. After launching
the tresta binary, open a valid configuration file to display the structure.

CSV files
---------
All CSV file must be comma delimited with no spaces between values, i.e. one row of the nodal coordinates file might resemble `1.0,2.0,3.0`.
### Nodes ###
The nodes CSV file specifies the `x, y, z` coordinate of each node in the truss.
The format should be:

    x1,y1,z1
    x2,y2,z2
    ...
    ...
    ...
    xN,yN,zN


where each entry is a floating point number and every line must have 3 entries for the `x,y,z` position.

### Elements ###
The elements CSV file specifies which nodes are connected. Each element is comprised
of two nodes, and each node is referenced by its index in the nodes CSV file, i.e.
if there a strut between the first and second node in `nodes.csv` the first line
in `elems.csv` would be `0,1`. Nodal indices are zero based and incremented by row.
The format of the element CSV file should be:

    el1_node_num_1,el1_node_num_2
    el2_node_num_1,el2_node_num_2
    ...
    ...
    ...
    elN_node_num_1,elN_node_num_2

### Displacements ###
Displacements are optional, but if provided allow visualization of the deformed
truss structure. This file will, in general, be computed by a finite element
analysis (see [threed-beam-fea](https://github.com/latture/threed-beam-fea)).
The format is such:

    dx_1, dy_1, dz_1, rx_1, ry_1, rz_1
    dx_2, dy_2, dz_2, rx_2, ry_2, rz_2
    ...
    ...
    ...
    dx_N, dy_N, dz_N, rx_N, ry_N, rz_N

where dx_1, dy_1, and dz_1 are the translational displacements applied to node 1
in the x-, y-, and z-directions, respectively, and rx_1, ry_1, rz_1 are nodal
rotations about each global coordinate axis.

### Example ###
An example configuration file is in the `examples` folder.
Before opening with tresta, change the path specified by `"nodes"`, `"elems"`,
and `"displacements"` to the correct path on your computer.
