#!/bin/bash
set -eu

srun \
  --job-name "[5] x20" \
  --mail-type=END,FAIL \
  --mail-user=yan.dolzhanskiy@gmail.com \
  --ntasks=1 \
  --nodelist=orthrus-2 \
  --mem=128G \
  --cpus-per-task=20 \
  --time=100:00:00 \
  --output="output.log" \
  ./run-singularity.sh "$@"
