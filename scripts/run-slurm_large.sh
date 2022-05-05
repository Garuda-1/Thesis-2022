#!/bin/bash
set -eu

srun \
  --job-name "[6] Large x24" \
  --mail-type=ALL \
  --mail-user=yan.dolzhanskiy@gmail.com \
  --ntasks=1 \
  --nodelist=meduza-1 \
  --mem=256G \
  --cpus-per-task=24 \
  --time=180:00:00 \
  --output="output-large-2.log" \
  ./run-singularity.sh /Thesis/experiments/6/6_large_2.json
