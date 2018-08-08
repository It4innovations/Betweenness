#!/bin/bash

# Get parent directory of this script
ROOT_DIR="$( cd "$(dirname "$0")" ; pwd -P )"
echo "Root dir: $ROOT_DIR"

if (( $# < 1 ))
then
  echo "Usage: $0 [scalability.log]"
fi

LOGFILE=$1
TIMEFILE="./$LOGFILE.times"

if [ ! -f $LOGFILE ]
then
  echo "File $LOGFILE does not exist."
fi

echo "run,variant,threads,ranks,nodes,jobid,time" | tee $TIMEFILE

IFS=','
while read -r run variant threads ranks nodes jobid
do
 

  OUTFILE=$ROOT_DIR/Results/scalability/output_$jobid/output.log

  if [ ! -f $OUTFILE ]
  then
    echo "$OUTFILE does not exist."
    continue
  fi

  # Open logfile
  TIME=$(grep "took" $OUTFILE | awk '{print $3}')
 
  if [ -z "$TIME" ]
  then
    TIME="NA"
    echo "Time not found for job $jobid"
  fi

  echo "$run,$variant,$threads,$ranks,$nodes,$jobid,$TIME" | tee -a $TIMEFILE

done <<< "$( tail -n +2 $LOGFILE)"

