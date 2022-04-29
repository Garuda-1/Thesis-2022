#!/bin/bash
set -eu

srun \
  --job-name "[5] Background x12" \
  --mail-type=ALL \
  --mail-user=yan.dolzhanskiy@gmail.com \
  --ntasks=1 \
  --nodelist={orthrus,meduza}-[1,2] \
  --mem=128G \
  --cpus-per-task=12 \
  --time=144:00:00 \
  --output="output-background.log" \
  ./run-singularity-rem.sh /Thesis/experiments/5_background.json
