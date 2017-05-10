module load intel/2017.00
module load Mono/4.2.2.10-intel-2016.01
module load CMake/3.5.2-intel-2017.00

mkdir Graphs/BetweennessInputs
mkdir Graphs/BetweennessOutputs
mkdir Graphs/OpenStreetMaps
mkdir Graphs/VisualizationInputs
cd Code
rm -r build
mkdir build
cd build
cmake ..
make