#!/bin/bash
set -eu

srun \
  --job-name "[5] MCPER-C x20" \
  --mail-type=ALL \
  --mail-user=yan.dolzhanskiy@gmail.com \
  --ntasks=1 \
  --nodelist={orthrus,meduza}-[1,2] \
  --mem=128G \
  --cpus-per-task=1 \
  --time=24:00:00 \
  --output="output-mcper-c.log" \
  ./run-singularity-single.sh /Thesis/experiments/5_mcper-c_bsat_php8_steiner.json
