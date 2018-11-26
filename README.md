# Betweenness centrality
C++ implementation of Betweenness algorithm according to *U. Brandes: On Variants of Shortest-Path Betweenness Centrality and their Generic Computation (2008)*.
Includes sequential and parallel versions (OpenMP and MPI).

The code has been developed and optimized for the [IT4Innovations Salomon cluster](https://docs.it4i.cz/salomon/hardware-overview/).

## Build instructions
1. Clone the repository
2. Run `compile.sh` script


## Variants
After successfull compilation the `Code/build` directory should contain three betweenness executables (`float`,`double` and `double-ext`).

The first two versions do not use the vertex and edge importance extension and differ only in the used floating point data type. The third version uses the importance extension along with the double precision floating point numbers.

## Usage

Sequential and OpenMP version (0,1):
```
betweenness-[variant] -f <file> -v <version> -w <weight> -s <start> -e <end> -ch <chunk> -t <threads> -a <alphas> -b <betas>
```

MPI version (2):
```
mpirun -n 24 betweenness-[variant] -f <file> -v <version> -w <weight> -s <start> -e <end> -ch <chunk> -t <threads> -a <alphas> -b <betas>
```

Parameters:
```
 <file>: 	File with graph in .csv format
 <version>: 	0 is serial[default], 1 is OpenMP version, 2 is MPI
 <start>: 	From which vertex id calculate betweenness [default=0]
 <end>: 	To which vertex id calculate [default=last vertex of graph]
 <chunk>: 	Size of work for mpi slave processes (number of vertices)
 <threads>: 	Used threads by parallel version or each slave process
 <alphas>: 	File with alpha values for node importance
 <betas>: 	File with beta values for node importance
```

## Experiments

### Scalability
There are two experiments available, both are automated by their respective bash scripts. The first one is the scalability experiment executed by the `run_scalability.sh` script. This experiment measures single compute node scalabilty for both OpenMP and MPI versions and multi node scalability up to 16 nodes using MPI. The individual runs are separated to a set of PBS jobs. The script accepts single argument which is path to the input graph CSV file which will be used for the experiment. More parameters such as number of consecutive runs or threads can be set by editing the script directly. Each run is executed in its own job, their assignment is available in log file prodced by the script. To obtain run times for each run, use the enclosed `extract_times.sh` script and pass name of the logfile as argument.

### Mobility
The second experiment computes Betweenness centrality of Vienna road network graph representation. It is executed by the `run_mobility.sh` script. This script also accepts single parameter with the input graph. The script runs the extended version of the algorithm for each file in the Beta directory. Files in this directory contains values of the alpha and beta parameters of the extended algorithm described in the paper. Results for each run are available in the `Results` directory. To obtain postprocessed data for visualisation, use the `postprocess.sh` script which accepts job id of the mobility run as a parameter. Output of this script is stored in `Results/postprocessed_XXXXX`, where `XXXXX` is the job id of the run.

# LICENSE
This implementation of the Betweenness algorithm is licensed under the [New BSD License](../LICENSE.md).

This repository contains information from the Open Street Map, which is made available here under the **Open Database License (ODbL)**. Full text of the ODbL is available at [https://opendatacommons.org/licenses/odbl/1.0/](https://opendatacommons.org/licenses/odbl/1.0/) or [here](../LICENSE.ODBL.md).
