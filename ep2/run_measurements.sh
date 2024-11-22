#! /bin/bash

set -o xtrace

MEASUREMENTS=20
ITERATIONS=10

INITIAL_SIZE=2
N_POINTS=50
ITER_LIMIT=1024

THREADS_PER_BLOCK=$INITIAL_SIZE

NAMES=('heat' 'heat_cuda' 'cuda_heat_37')
make all
mkdir -p results

for NAME in ${NAMES[@]}; do
    mkdir -p results/$NAME

    for ((n=1; n<=6; n++)); do

        for ((i=1; i<=$ITERATIONS; i++)); do
            BLOCK_SIZE=$(echo "scale=0; sqrt($THREADS_PER_BLOCK)" | bc)
            
            GRID_DIM=$(echo "($N_POINTS + $BLOCK_SIZE - 1) / $BLOCK_SIZE" | bc)
            
            BLOCKS_PER_GRID=$(echo "$GRID_DIM * $GRID_DIM" | bc)
            
            for ((j=1; j<=$MEASUREMENTS; j++)); do
                ./$NAME.o $N_POINTS $ITER_LIMIT $THREADS_PER_BLOCK $BLOCKS_PER_GRID >> $NAME.log 2>&1
            done

            THREADS_PER_BLOCK=$(echo "$THREADS_PER_BLOCK * 2" | bc)
        done

        THREADS_PER_BLOCK=$INITIAL_SIZE

        N_POINTS=$(echo "$N_POINTS * 2" | bc)

    done
    N_POINTS=50

done
