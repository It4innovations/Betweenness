#!/bin/bash

ml intel/2018.03 Python pandas

if (( $# < 1 ))
then
  echo "Postprocess result from mobility run"
  echo "Usage: $0 [PBS job id]"
fi

# Get parent directory of this script
ROOT_DIR="$( cd "$(dirname "$0")" ; pwd -P )"
echo "Root dir: $ROOT_DIR"

JOBID=$1

# Create postprocess dir
PDIR="postprocess_$JOBID"

pushd Results
mkdir $PDIR 
pushd $PDIR || exit 1

# Combine all results of single run to a single directory
for d in $(find .. -type d -name "output_$JOBID*.isrv5")
do
  RESULT_INDEX=$(echo $d | sed -r 's/.*output_.*\[([0-9]+)\].*/\1/')
  echo "Got result $RESULT_INDEX"
  cp -v $d/result_edge_betweenness.csv ./result_edge_betweenness_$RESULT_INDEX.csv
done

echo "Running normalization"
python $ROOT_DIR/normalization_max.py -d ./

echo "Running comparator"
python $ROOT_DIR/comparator.py -d ./normalized -o ./compared -p 0.07 -t 1 -v 3

echo "Postprocessing finished."
popd
popd
