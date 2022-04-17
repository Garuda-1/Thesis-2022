#!/bin/bash
set -eu

srun \
  --job-name "[5] EAER x20" \
  --mail-type=ALL \
  --mail-user=yan.dolzhanskiy@gmail.com \
  --ntasks=1 \
  --nodelist={orthrus,meduza}-[1,2] \
  --mem=64G \
  --cpus-per-task=20 \
  --time=24:00:00 \
  --output="output-mcper-c.log" \
  ./run-singularity.sh /Thesis/experiments/5_mcper-c_bsat_php8_steiner.json
