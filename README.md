# Betweenness centrality Code Repository
This repository contains C++ implementation of Betweenness algorithm according to *U. Brandes: On Variants of Shortest-Path Betweenness Centrality and their Generic Computation (2008)*.

## Compile instructions
```
module load intel/2017.00 Mono/4.2.2.10-intel-2016.01 CMake/3.5.2-intel-2017.00
cd Code
mkdir build && cd build
cmake ..
make
```
## Use instructions

1. Unzip experiment_graphs.zip and put files to Graphs/OpenStreetMaps folder
```
unzip Graphs/experiment_graphs.zip -d Graphs/OpenStreetMaps/
```

2. Use graph manipulator utility to generate inputs for betweenness algorithm
```
module load Mono/4.2.2.10-intel-2016.01
mono Code/build/BetweennessGraphManipulator.exe -f ../../Graphs -o 1
```

3. Run betweenness centrality algorithm on generated data and move edge BC outputs to Graphs/BetweennessOutputs folder, vertex BC output can be deleted(is not used for visualization)
```
module purge
module load intel/2017.00
cd Code/build/
./betweenness -f ../../Graphs/BetweennessInputs/graph-cze-brno_preprocessed.csv
cd ../../Graphs/BetweennessInputs/
mv graph-cze-brno_preprocessed.csv_result_edge_betweenness.csv ../BetweennessOutputs/
rm graph-cze-brno_preprocessed.csv_result_vertex_betweenness.csv
```

4. Put just edge results from BC algorithm to Graphs/BetweennessOutputs folder and run utility again, but now for generating inputs for visualization
```
module purge
module load Mono/4.2.2.10-intel-2016.01
cd Code/build/
mono BetweennessGraphManipulator.exe -f ../../Graphs -o 2
```

5. Use generated files for visualization from Graphs/VisualizationInputs folder

## Experiments

1. Sequential version on one node
```
qsub -q qexp -l select=1 -I
module load intel/2017.00
cd Code/Betweenness/
./betweenness.exe -f ../../Graphs/BetweennessInputs/graph-cze-brno_preprocessed.csv
```

File | Vertices | Time[s]
------------- |-------------|-------------
graph-cze-brno_preprocessed.csv    | 55492  | 103.9
graph-cze-ostrava_preprocessed.csv | 45404  | 73.5
graph-cze-praha_preprocessed.csv   | 160613 | 956.9
graph-prt-lisbon_preprocessed.csv  | 61513  | 93.3
graph-prt-port_preprocessed.csv    | 112679 | 80.5


2. Our parallel version on 10 nodes = 240 processes and 1 thread per process

File | Vertices | Time[s]
------------- |-------------|-------------
graph-cze_preprocessed.csv         | 969389 | 3599
graph-cze-brno_preprocessed.csv    | 55492  | 0.99
graph-cze-ostrava_preprocessed.csv | 45404  | 0.68
graph-cze-praha_preprocessed.csv   | 160613 | 8.65
graph-prt_preprocessed.csv         | 942609 | 3694
graph-prt-lisbon_preprocessed.csv  | 61513  | 0.88
graph-prt-port_preprocessed.csv    | 112679 | 1.36

# LICENSE
This implementation of the Betweenness algorithm is licensed under the **GNU Lesser General Public License (LGPL)**. Full text of the LGPL can be found at [https://www.gnu.org/licenses/lgpl-3.0-standalone.html](https://www.gnu.org/licenses/lgpl-3.0-standalone.html) or [here](../LICENSE.LGPL.md).

This repository contains information from the Open Street Map, which is made available here under the **Open Database License (ODbL)**. Full text of the ODbL is available at [https://opendatacommons.org/licenses/odbl/1.0/](https://opendatacommons.org/licenses/odbl/1.0/) or [here](../LICENSE.ODBL.md).
