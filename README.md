# Betweenness centrality Code Repository
This repository contains C++ implementation of Betweenness algorithm according to *U. Brandes: On Variants of Shortest-Path Betweenness Centrality and their Generic Computation (2008)*.

## Compile instructions
```
module load intel/2017.00 CMake/3.5.2-intel-2017.00
cd Code
mkdir build && cd build
cmake ..
make
```
## Use instructions

1. Unzip experiment_graphs.zip
```
unzip Graphs/experiment_graphs.zip -d Graphs/
```

2. Run betweenness centrality algorithm
```
cd Code/build/
./betweenness -f ../../Graphs/graph-cze-brno.csv
```

## Experiments

1. Sequential version on one node
```
qsub -q qexp -l select=1 -I
module load intel/2017.00
cd Code/build/
./betweenness -f ../../Graphs/graph-cze-brno.csv
```

File | Vertices | Time[s]
------------- |-------------|-------------
graph-cze-brno.csv    | 55492  | 103.9
graph-cze-ostrava.csv | 45404  | 73.5
graph-cze-praha.csv   | 160613 | 956.9
graph-prt-lisbon.csv  | 61513  | 93.3
graph-prt-port.csv    | 112679 | 80.5


2. Our parallel version on 10 nodes = 240 processes and 1 thread per process

File | Vertices | Time[s]
------------- |-------------|-------------
graph-cze.csv         | 969389 | 3599
graph-cze-brno.csv    | 55492  | 0.99
graph-cze-ostrava.csv | 45404  | 0.68
graph-cze-praha.csv   | 160613 | 8.65
graph-prt.csv         | 942609 | 3694
graph-prt-lisbon.csv  | 61513  | 0.88
graph-prt-port.csv    | 112679 | 1.36


# LICENSE
This implementation of the Betweenness algorithm is licensed under the **GNU Lesser General Public License (LGPL)**. Full text of the LGPL can be found at [https://www.gnu.org/licenses/lgpl-3.0-standalone.html](https://www.gnu.org/licenses/lgpl-3.0-standalone.html) or [here](../LICENSE.LGPL.md).

This repository contains information from the Open Street Map, which is made available here under the **Open Database License (ODbL)**. Full text of the ODbL is available at [https://opendatacommons.org/licenses/odbl/1.0/](https://opendatacommons.org/licenses/odbl/1.0/) or [here](../LICENSE.ODBL.md).