#!/bin/bash
set -eu

singularity exec --bind /nfs/home/ydolzhanskiy/Thesis-2022:/Thesis /nfs/home/ydolzhanskiy/Thesis-2022/experiments-env /nfs/home/ydolzhanskiy/Thesis-2022/scripts/run.sh "$@"
