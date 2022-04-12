#!/bin/bash
set -eu

srun \
  --job-name "B11 Single Thread" \
  --mail-type=END,FAIL \
  --mail-user=yan.dolzhanskiy@gmail.com \
  --ntasks=1 \
  --nodelist=meduza-1 \
  --mem=16G \
  --cpus-per-task=1 \
  --time=100:00:00 \
  --output="output.log" \
  ./run-singularity.sh "$@"
