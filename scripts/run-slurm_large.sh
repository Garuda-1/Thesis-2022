#!/bin/bash
set -eu

srun \
  --job-name "[6] Large x20" \
  --mail-type=ALL \
  --mail-user=yan.dolzhanskiy@gmail.com \
  --ntasks=1 \
  --nodelist=meduza-1 \
  --mem=128G \
  --cpus-per-task=20 \
  --time=180:00:00 \
  --output="output-large.log" \
  ./run-singularity.sh /Thesis/experiments/6/6_large.json
