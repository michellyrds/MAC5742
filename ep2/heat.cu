#include <cuda.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define WALL_TEMP 20.0
#define FIREPLACE_TEMP 100.0

#define FIREPLACE_START 3
#define FIREPLACE_END 7
#define ROOM_SIZE 10

void initialize(double ** h, int n){
    int fireplace_start = (FIREPLACE_START * n) / ROOM_SIZE;
    int fireplace_end = (FIREPLACE_END * n) / ROOM_SIZE;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == 0 || i == n - 1 || j == 0 || j == n - 1) {
                h[i][j] = (i == n - 1 && j >= fireplace_start && j <= fireplace_end) ? FIREPLACE_TEMP : WALL_TEMP;
            } else {
                h[i][j] = 0.0;
            }
        }
    }
}

// CUDA
__global__ void jacobi_iteration(double ** h, double ** g, int n, int iter_limit) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    int j = blockIdx.y * blockDim.y + threadIdx.y;

    if (i > 0 && i < n - 1 && j > 0 && j < n - 1) {
        for (int iter = 0; iter < iter_limit; iter++) {
            g[i][j] = 0.25 * (h[i - 1][j] + h[i + 1][j] + h[i][j - 1] + h[i][j + 1]);
            __syncthreads();

            h[i][j] = g[i][j];
            __syncthreads();
        }
    }
}


double calculate_elapsed_time(struct timespec start, struct timespec end) {
    double start_sec = (double) start.tv_sec * 1e9 + (double) start.tv_nsec;
    double end_sec = (double) end.tv_sec * 1e9 + (double) end.tv_nsec;
    return (end_sec - start_sec) / 1e9;
}

void save_to_file(double ** h, int n) {
    FILE * file = fopen("cuda_room.txt", "w");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fprintf(file, "%lf ", h[i][j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

int main(int argc, char * argv[]) {
    if (argc < 5) {
        fprintf(stderr, "Uso: %s <número de pontos> <limite de iterações> <número de threads por bloco> <número de blocos por grade>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    int iter_limit = atoi(argv[2]);
    int t = atoi(argv[3]);
    int b = atoi(argv[4]);

    int block_size = sqrt(t);

    int b = (n + block_size - 1 )/ block_size;

    dim3 threads_per_block = dim3(block_size, block_size);
    dim3 grid_size = dim3(b, b);
    

    printf("block_size: %d x %d\n", block_size, block_size);
    printf("grid_size: %d x %d\n", b, b);

    double **h, **g;

    cudaMallocManaged(&h, n * sizeof(double*));
    cudaMallocManaged(&g, n * sizeof(double*));

    if (h == NULL || g == NULL) {
        fprintf(stderr, "Erro ao alocar memória para h ou g\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; i++) {
        cudaMallocManaged(&h[i], n * sizeof(double));
        cudaMallocManaged(&g[i], n * sizeof(double));
        if (h[i] == NULL || g[i] == NULL) {
            fprintf(stderr, "Erro ao alocar memória para h[%d] ou g[%d]\n", i, i);
            exit(EXIT_FAILURE);
        }
    }

    struct timespec start, end;
    initialize(h, n);
    clock_gettime(CLOCK_MONOTONIC, &start);
    jacobi_iteration<<<grid_size, threads_per_block>>>(h, g, n, iter_limit);
    clock_gettime(CLOCK_MONOTONIC, &end);
    cudaDeviceSynchronize();
    save_to_file(h, n);

    double elapsed_time = calculate_elapsed_time(start, end);
    printf("Tempo de execução: %.9f segundos\n", elapsed_time);

    for (int i = 0; i < n; i++) {
        cudaFree(h[i]);
        cudaFree(g[i]);
    }
    cudaFree(h);
    cudaFree(g);

    return 0;
}