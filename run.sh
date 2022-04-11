#!/bin/bash
set -eu

MAX_THREADS=3
NUM_THREADS=0

function acquire() {
	while [[ $MAX_THREADS -le $NUM_THREADS ]]; do
		sleep 1
	done
	NUM_THREADS=$(( $NUM_THREADS + 1 ))
}

function release() {
	echo "Test $1 finished at $(date)"
	NUM_THREADS=$(( $NUM_THREADS - 1 ))
}

for (( i = 0; i < 9; ++i )); do
	config="config_${i}"
	acquire && echo "test with config: $config" && sleep 1 && release "$config" &
done

wait
