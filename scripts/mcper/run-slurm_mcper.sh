#!/bin/bash
set -eu

srun \
  --job-name "[7] MCPER x8" \
  --mail-type=ALL \
  --mail-user=yan.dolzhanskiy@gmail.com \
  --ntasks=1 \
  --nodelist=meduza-1 \
  --mem=64G \
  --cpus-per-task=8 \
  --time=180:00:00 \
  --output="output-mcper.log" \
  ./run-singularity_mcper.sh
