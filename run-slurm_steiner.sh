#!/bin/bash
set -eu

srun \
  --job-name "[5] Steiner x20" \
  --mail-type=ALL \
  --mail-user=yan.dolzhanskiy@gmail.com \
  --ntasks=1 \
  --nodelist=orthrus-[1,2] \
  --mem=128G \
  --cpus-per-task=20 \
  --time=24:00:00 \
  --output="output-steiner.log" \
  ./run-singularity.sh /Thesis/experiments/5_gaa_gaer_null_steiner.json
