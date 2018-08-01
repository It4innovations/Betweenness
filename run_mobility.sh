#!/bin/bash

# Load required modules
ml intel/2018.03

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

  # Submit job for all beta settings
  BETA_NUM=$(find $ROOT_DIR -name "beta_*.txt" | wc -l)
  echo "Found $BETA_NUM beta settings" 

  SUBMITTED_JOB_ID=$(qsub -q qexp -A OPEN-8-23 -N BTW_MOBI -J 1-$BETA_NUM \
    -l select=2:ncpus=24:mpiprocs=24:ompthreads=1,walltime=01:00:00 \
    -v INPUT_GRAPH=$INPUT_GRAPH,ROOT_DIR=$ROOT_DIR $0)

  echo "PBS Job ID: $SUBMITTED_JOB_ID"

fi

#
# PBS Jobscript
#
if [ -n "$PBS_JOBID" ]
then
  echo $INPUT_GRAPH
  echo $ROOT_DIR

  BTW_BINARY=$ROOT_DIR/Code/build/betweenness-double-ext

  if [ ! -f "$BTW_BINARY" ]
  then
    echo "Betwenneness binary not found. Run compile.sh."
    exit 1
  fi

  RESULT_DIR=$ROOT_DIR/Results/output_$PBS_JOBID
  echo "Results: $RESULT_DIR"

  BETA_FILE="$ROOT_DIR/Beta/beta_m$PBS_ARRAY_INDEX.txt"
  if [ ! -f $BETA_FILE ]
  then
    echo "Beta file $BETA_FILE does not exist."
    exit 1
  fi

  mkdir -p $RESULT_DIR || exit 1
  cd $RESULT_DIR

  mpiexec $BTW_BINARY -f $INPUT_GRAPH -v 2 -t 1 -b $BETA_FILE |& tee output.log 

fi
