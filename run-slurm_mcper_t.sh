#!/bin/bash
set -eu

srun \
  --job-name "[5] MCPER-T x20" \
  --mail-type=ALL \
  --mail-user=yan.dolzhanskiy@gmail.com \
  --ntasks=1 \
  --nodelist={orthrus,meduza}-[1,2] \
  --mem=128G \
  --cpus-per-task=1 \
  --time=24:00:00 \
  --output="output-mcper-t.log" \
  ./run-singularity-single.sh /Thesis/experiments/5_mcper-t_bsat_php8_steiner.json
