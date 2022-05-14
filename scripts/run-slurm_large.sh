#!/bin/bash
set -eu

srun \
  --job-name "[6] Large x24" \
  --mail-type=ALL \
  --mail-user=yan.dolzhanskiy@gmail.com \
  --ntasks=1 \
  --nodelist=meduza-1 \
  --mem=192G \
  --cpus-per-task=24 \
  --time=180:00:00 \
  --output="output-large-8.log" \
  ./run-singularity.sh /Thesis/experiments/8/8_large.json
