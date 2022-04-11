#!/bin/bash
set -eu

srun \
   ... \
   'singularity exec ips.sif --bind ... ./run.sh'
