#!/bin/bash
set -eu

srun \
  --job-name "[5] MCPER-C XL" \
  --mail-type=ALL \
  --mail-user=yan.dolzhanskiy@gmail.com \
  --ntasks=1 \
  --nodelist=meduza-[1,2] \
  --mem=128G \
  --cpus-per-task=1 \
  --time=140:00:00 \
  --output="output-mcper-c-xl.log" \
  ./run-singularity-single.sh /Thesis/experiments/5_mcper-c-xl.json
