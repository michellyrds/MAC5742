#! /bin/bash

set -o xtrace

MEASUREMENTS=20
ITERATIONS=4

IMAGE=(500 1000 2500 5000)
N=500
N_PROC=2

NAMES=('sequential_julia' '1D_parallel_julia')
make all

for NAME in ${NAMES[@]}; do

    for ((i = 1; i <= $ITERATIONS; i++)); do

        N=${IMAGE[$((i-1))]} 

        for ((j = 1; j <= $MEASUREMENTS; j++)); do
            if [[ "$NAME" == 'sequential_julia' ]]; then
                ./$NAME.o $N
            else
                # mpirun -np $N_PROC ./$NAME $N
                smpirun -np $N_PROC -hostfile ./simple_cluster_hostfile.txt -platform ./simple_cluster.xml ./$NAME $N
            fi
        done
        N_PROC=$(echo "$N_PROC * 2" | bc)

    done
    N_PROC=2
done
