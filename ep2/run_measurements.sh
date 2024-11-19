#! /bin/bash

set -o xtrace

MEASUREMENTS=20
ITERATIONS=8

INITIAL_SIZE=2
N_POINTS=100
ITER_LIMIT=1024

THREADS_PER_BLOCK=$INITIAL_SIZE

NAMES=('heat' 'cuda_heat')

make all
mkdir -p results

for NAME in ${NAMES[@]}; do
    mkdir -p results/$NAME

    for ((i=1; i<=$ITERATIONS; i++)); do
        BLOCK_SIZE=$(echo "sqrt($THREADS_PER_BLOCK)" | bc)
        GRID_DIM=$((($N_POINTS + $BLOCK_SIZE - 1) / $BLOCK_SIZE ))
        BLOCKS_PER_GRID=$(( GRID_DIM * GRID_DIM ))
        for ((j=1; j<=$MEASUREMENTS; j++)); do
            nvprof ./$NAME.o $N_POINTS $ITER_LIMIT $THREADS_PER_BLOCK $BLOCKS_PER_GRID --cpu-profiling >> $NAME.log 2>&1
        done
        THREADS_PER_BLOCK=$(($THREADS_PER_BLOCK * $THREADS_PER_BLOCK))
    done

    THREADS_PER_BLOCK=$INITIAL_SIZE

    mv *.log results/$NAME
done

rm -f room.txt cuda_room.txt