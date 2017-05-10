sh compile.sh

#load necessary modules
module load intel/2017.00
module load Mono/4.2.2.10-intel-2016.01

#unzip OpenStreetMaps
unzip Graphs/experiment_graphs.zip -d Graphs/OpenStreetMaps/

#generate input files for betweenness algorithm
mono Code/build/BetweennessGraphManipulator.exe -f Graphs -o 1

#run BC algorithm on generated files
./Code/build/betweenness -f Graphs/BetweennessInputs/graph-prt-port_preprocessed.csv

#move edge BC result and delete vertex BC result, that is not needed for visualization
mv Graphs/BetweennessInputs/graph-prt-port_preprocessed.csv_result_edge_betweenness.csv Graphs/BetweennessOutputs/
rm Graphs/BetweennessInputs/graph-prt-port_preprocessed.csv_result_vertex_betweenness.csv

#run utility again to merge OpenStreetMaps files with BC outputs to generate files for visualization
mono Code/build/BetweennessGraphManipulator.exe -f Graphs -o 2

sh clean.sh
