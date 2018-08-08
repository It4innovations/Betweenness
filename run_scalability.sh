#!/bin/bash

# Load required modules
ml intel/2018.03

# PBS Job parameters
QUEUE=qprod
PROJECT=OPEN-8-23

# Parameters
BTW_VARIANTS=(double float double-ext)

#THREADS=(1 4 8 12 16 20 24)
THREADS=(16 20 24)
NODES=(1 2 4 8 16 32)

DATE=$(date +%y%m%d_%H%M)
LOGFILE="./betweenness_scalability_$DATE.log"

echo "run,variant,threads,ranks,nodes,jobid" | tee $LOGFILE

#
# Job preparation and submission
#
if [ -z "$PBS_JOBID" ]
then

  # Check argument count
  if (( $# < 1 ))
  then
    echo "Usage $0 [path_to_graph.csv]"
    exit 1
  fi
  
  # Get parent directory of this script
  ROOT_DIR="$( cd "$(dirname "$0")" ; pwd -P )"
  echo "Root dir: $ROOT_DIR"

  
  echo "Creating Betweenness PBS job"
      
  # Get input graph as first script argument
  INPUT_GRAPH=$1
  if [ ! -f $INPUT_GRAPH ]
  then
    echo "File $INPUT_GRAPH does not exists."
    exit 1
  fi

  # Determine absolute path to the input graph file
  INPUT_GRAPH=$(readlink -f $INPUT_GRAPH)
 
  echo "Single node OpenMP" 

  RUNS=0
  for r in $(seq $RUNS)
  do
    for v in ${BTW_VARIANTS[@]}
    do
      for i in "${THREADS[@]}"
      do
        JOBID=$(qsub -q $QUEUE -A $PROJECT -N BTW_SCAL \
          -l select=1:mpiprocs=1:ompthreads=$i,walltime=00:05:00 \
          -v INPUT_GRAPH=$INPUT_GRAPH,ROOT_DIR=$ROOT_DIR,VERSION=1,VARIANT=$v $0)
        echo "$r,$v,$i,1,1,$JOBID" | tee -a $LOGFILE
      done
    done
  done

  echo "Single node MPI" 
  RUNS=1
  for r in $(seq $RUNS)
  do
    for v in ${BTW_VARIANTS[@]}
    do
      for i in "${THREADS[@]}"
      do
        JOBID=$(qsub -q $QUEUE -A $PROJECT -N BTW_SCAL \
          -l select=1:mpiprocs=$i:ompthreads=1,walltime=00:10:00 \
          -v INPUT_GRAPH=$INPUT_GRAPH,ROOT_DIR=$ROOT_DIR,VERSION=2,VARIANT=$v $0)
        echo "$r,$v,1,$i,1,$JOBID" | tee -a $LOGFILE
      done
    done
  done
  
  echo "Multinode" 
  RUNS=0
  for r in $(seq $RUNS)
  do
    for v in ${BTW_VARIANTS[@]}
    do
      for i in "${NODES[@]}"
      do
        JOBID=$(qsub -q $QUEUE -A $PROJECT -N BTW_SCAL \
          -l select=$i:mpiprocs=24:ompthreads=1,walltime=00:05:00 \
          -v INPUT_GRAPH=$INPUT_GRAPH,ROOT_DIR=$ROOT_DIR,VERSION=2,VARIANT=$v $0)
        echo "$r,$v,1,24,$i,$JOBID" | tee -a $LOGFILE
      done
    done
  done
fi

#
# PBS Jobscript
#
if [ -n "$PBS_JOBID" ]
then

  BTW_BINARY_DIR=$ROOT_DIR/Code/build

  if [ ! -d "$BTW_BINARY_DIR" ]
  then
    echo "Betwenneness binary directory not found. Run compile.sh."
    exit 1
  fi

  RESULT_DIR=$ROOT_DIR/Results/scalability/output_$PBS_JOBID
  echo "Results: $RESULT_DIR"

  mkdir -p $RESULT_DIR || exit 1
  cd $RESULT_DIR

  mpirun $BTW_BINARY_DIR/betweenness-$VARIANT -f $INPUT_GRAPH -v $VERSION -t $OMP_NUM_THREADS |& tee output.log 

fi
