module load intel/2017.00
mpicc -O3 -qopenmp -xHost -std=c++11 -o betweenness.exe main.cpp Betweenness.cpp BetweennessResult.cpp Edge.cpp KeyValuePair.cpp Utils.cpp WeightedDirectedGraph.cpp CsvReader.cpp -I .
