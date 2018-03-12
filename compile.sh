module load intel/2017.00
module load CMake/3.5.2-intel-2017.00

cd Code
rm -r build
mkdir build
cd build
cmake ..
make