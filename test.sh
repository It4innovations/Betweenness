sh compile.sh

#load necessary modules
module load intel/2017.00

#unzip OpenStreetMaps
unzip Graphs/experiment_graphs.zip -d Graphs/

#run BC algorithm on generated files
./Code/build/betweenness -f Graphs/graph-prt-port.csv

#clean
sh clean.sh