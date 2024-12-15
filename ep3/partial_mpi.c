#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int rows_per_process(int n_proc, int n, int rank) {
    int rows_base = n / n_proc;
    int rows_rest = n % n_proc;

    int range = rows_base + (rank < rows_rest ? 1 : 0);
    return range;
}

void update_row_range(int n_proc, int n, int rank, int *start_row,
                      int *end_row) {
    int rows_base = n / n_proc;
    int rows_rem = n % n_proc;

    *start_row = rank * rows_base + (rank < rows_rem ? rank : rows_rem);

    int range = rows_base + (rank < rows_rem ? 1 : 0);

    *end_row = *start_row + range - 1;
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int n_proc, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &n_proc);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (argc != 2) {
        if (rank == 0) {
            fprintf(stderr, "Uso: %s <número positivo n>\n", argv[0]);
        }
        MPI_Finalize();
        return 1;
    }

    int n = atoi(argv[1]);

    int start_row, end_row;

    update_row_range(n_proc, n, rank, &start_row, &end_row);

    printf(
        "[Process %d out of %d]: I should compute pixel rows %d to %d, for  "
        "total of %d rows\n",
        rank, n_proc, start_row, end_row, rows_per_process(n_proc, n, rank));

    MPI_Finalize();
    return 0;
}