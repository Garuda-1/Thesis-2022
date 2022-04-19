#!/bin/bash
set -eu

srun \
  --job-name "[5] MCPER-C-PLUS" \
  --mail-type=ALL \
  --mail-user=yan.dolzhanskiy@gmail.com \
  --ntasks=1 \
  --nodelist=meduza-[1,2] \
  --mem=64G \
  --cpus-per-task=1 \
  --time=24:00:00 \
  --output="output-mcper-c-plus.log" \
  ./run-singularity-single.sh /Thesis/experiments/5_mcper-c-plus.json
