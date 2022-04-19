#!/bin/bash
set -eu

srun \
  --job-name "[5] PHP6 fix x20" \
  --mail-type=ALL \
  --mail-user=yan.dolzhanskiy@gmail.com \
  --ntasks=1 \
  --nodelist=meduza-[1,2] \
  --mem=64G \
  --cpus-per-task=20 \
  --time=24:00:00 \
  --output="output-php8.log" \
  ./run-singularity.sh /Thesis/experiments/5_php6_fix.json
