#!/bin/bash
set -eu

srun \
  --job-name "B11 Single Thread" \
  --mail-type=END,FAIL \
  --mail-user=yan.dolzhanskiy@gmail.com \
  --ntasks=1 \
  --nodelist=orthrus-2 \
  --mem=128G \
  --cpus-per-task=32 \
  --time=100:00:00 \
  --output="output.log" \
  "singularity exec experiments-env --bind /nfs/home/ydolzhanskiy/Thesis-2022:/Thesis /nfs/home/ydolzhanskiy/Thesis-2022/run.sh $*" \
  && { sleep 1 ; kill -9 "$PID"; } \
  || { sleep 1 ; kill -9 "$PID"; }
