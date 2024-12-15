#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * compute_julia_pixel(): calcula os valores RGB de um pixel em
 *                        uma imagem específica de um conjunto de Julia.
 *
 *  Entrada:
 *      (x, y):           coordenadas do pixel
 *      (width, height):  dimensões da imagem
 *      tint_bias:        um valor float para ajustar a tonalidade (1.0 é "sem
 * ajuste") Saída: rgb: um array já alocado de 3 bytes onde serão escritos os
 * valores R, G e B.
 *
 *  Retorno:
 *      0 em caso de sucesso, -1 em caso de falha
 *
 */
int compute_julia_pixel(int x, int y, int width, int height, float tint_bias,
                        unsigned char *rgb) {
    // Verifica se as coordenadas são válidas
    if ((x < 0) || (x >= width) || (y < 0) || (y >= height)) {
        fprintf(stderr,
                "Coordenadas inválidas (%d,%d) para um pixel em uma imagem de "
                "%d x %d\n",
                x, y, width, height);
        return -1;
    }

    // "Amplia" a visualização para mostrar uma área agradável do conjunto de
    // Julia
    float X_MIN = -1.6, X_MAX = 1.6, Y_MIN = -0.9, Y_MAX = +0.9;
    float float_y = (Y_MAX - Y_MIN) * (float)y / height + Y_MIN;
    float float_x = (X_MAX - X_MIN) * (float)x / width + X_MIN;

    // Ponto que define o conjunto de Julia
    float julia_real = -.79;
    float julia_img = .15;

    // Número máximo de iterações
    int max_iter = 300;

    // Calcula a convergência da série complexa
    float real = float_y, img = float_x;
    int num_iter = max_iter;
    while ((img * img + real * real < 2 * 2) && (num_iter > 0)) {
        float xtemp = img * img - real * real + julia_real;
        real = 2 * img * real + julia_img;
        img = xtemp;
        num_iter--;
    }

    // Pinta o pixel com base no número de iterações usando uma coloração
    // estilizada
    float color_bias = (float)num_iter / max_iter;
    rgb[0] =
        (num_iter == 0 ? 200
                       : -500.0 * pow(tint_bias, 1.2) * pow(color_bias, 1.6));
    rgb[1] = (num_iter == 0 ? 100 : -255.0 * pow(color_bias, 0.3));
    rgb[2] = (num_iter == 0
                  ? 100
                  : 255 - 255.0 * pow(tint_bias, 1.2) * pow(color_bias, 3.0));

    return 0;
}

/* write_bmp_header():
 *
 *   Entrada:
 *      f: Um arquivo aberto para escrita ('w')
 *      (width, height): dimensões da imagem
 *
 *   Retorno:
 *      0 em caso de sucesso, -1 em caso de falha
 *
 */
int write_bmp_header(FILE *f, int width, int height) {
    unsigned int row_size_in_bytes =
        width * 3 + ((width * 3) % 4 == 0 ? 0 : (4 - (width * 3) % 4));

    // Define todos os campos no cabeçalho do BMP
    char id[3] = "BM";
    unsigned int filesize =
        54 + (int)(row_size_in_bytes * height * sizeof(char));
    short reserved[2] = {0, 0};
    unsigned int offset = 54;

    unsigned int size = 40;
    unsigned short planes = 1;
    unsigned short bits = 24;
    unsigned int compression = 0;
    unsigned int image_size = width * height * 3 * sizeof(char);
    int x_res = 0;
    int y_res = 0;
    unsigned int ncolors = 0;
    unsigned int importantcolors = 0;

    // Escreve os bytes no arquivo, mantendo o controle do
    // número de "objetos" escritos
    size_t ret = 0;
    ret += fwrite(id, sizeof(char), 2, f);
    ret += fwrite(&filesize, sizeof(int), 1, f);
    ret += fwrite(reserved, sizeof(short), 2, f);
    ret += fwrite(&offset, sizeof(int), 1, f);
    ret += fwrite(&size, sizeof(int), 1, f);
    ret += fwrite(&width, sizeof(int), 1, f);
    ret += fwrite(&height, sizeof(int), 1, f);
    ret += fwrite(&planes, sizeof(short), 1, f);
    ret += fwrite(&bits, sizeof(short), 1, f);
    ret += fwrite(&compression, sizeof(int), 1, f);
    ret += fwrite(&image_size, sizeof(int), 1, f);
    ret += fwrite(&x_res, sizeof(int), 1, f);
    ret += fwrite(&y_res, sizeof(int), 1, f);
    ret += fwrite(&ncolors, sizeof(int), 1, f);
    ret += fwrite(&importantcolors, sizeof(int), 1, f);

    // Sucesso significa que escrevemos 17 "objetos" com êxito
    return (ret != 17);
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
    int width = 2 * n;
    int height = n;
    float tint_bias = 1.0;

    int start_row, end_row;
    update_row_range(n_proc, n, rank, &start_row, &end_row);

    int rows_to_compute = end_row - start_row + 1;
    unsigned char *local_pixels = (unsigned char *)malloc(
        3 * width * rows_to_compute * sizeof(unsigned char));

    if (local_pixels == NULL) {
        fprintf(stderr, "Erro ao alocar memória no processo %d\n", rank);
        MPI_Finalize();
        return 1;
    }

    // printf(
    //     "[Process %d out of %d]: I should compute pixel rows %d to %d, for  "
    //     "total of %d rows\n",
    //     rank, n_proc, start_row, end_row, rows_to_compute);

    double start_time = MPI_Wtime();

    for (int y = start_row; y <= end_row; y++) {
        for (int x = 0; x < width; x++) {
            unsigned char *pixel =
                &local_pixels[(y - start_row) * width * 3 + x * 3];
            if (compute_julia_pixel(x, y, width, height, tint_bias, pixel) !=
                0) {
                fprintf(stderr,
                        "Erro ao calcular pixel (%d, %d) no processo %d\n", x,
                        y, rank);
                MPI_Finalize();
                return 1;
            }
        }
    }

    double end_time = MPI_Wtime();
    double elapsed_time = end_time - start_time;

    double* elapsed_times;

    if (rank == 0){
        elapsed_times = (double*)malloc(n_proc * sizeof(double));
    }
    MPI_Gather(&elapsed_time, 1, MPI_DOUBLE, elapsed_times, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if(rank == 0){
        FILE *file = fopen("parallel_logs.txt", "a");
        fprintf(file, "%s %d %d\n", argv[0], n, n_proc);
        fprintf(file, "Tempo de execução dos processos:\n");
        for (int i = 0; i < n_proc; i++) {
            fprintf(file, "Processo %d: %f segundos\n", i, elapsed_times[i]);
        }
        fclose(file);
        free(elapsed_times);
    }

    // printf("[Processo %d]: Tempo de execução: %.9f segundos\n", rank,
    //        elapsed_time);

    if (rank == 0) {
        FILE *output_file = fopen("julia.bmp", "wb");
        if (output_file == NULL) {
            fprintf(stderr, "Erro ao abrir arquivo julia.bmp\n");
            MPI_Finalize();
            return 1;
        }

        if (write_bmp_header(output_file, width, height) != 0) {
            fprintf(stderr, "Erro ao escrever cabeçalho do arquivo BMP\n");
            fclose(output_file);
            MPI_Finalize();
            return 1;
        }

        fwrite(local_pixels, sizeof(unsigned char), 3 * width * rows_to_compute,
               output_file);

        for (int i = 1; i < n_proc; i++) {
            int recv_start_row, recv_end_row;
            update_row_range(n_proc, n, i, &recv_start_row, &recv_end_row);

            int recv_rows = recv_end_row - recv_start_row + 1;
            unsigned char *recv_buffer = (unsigned char *)malloc(
                3 * width * recv_rows * sizeof(unsigned char));

            if (recv_buffer == NULL) {
                fprintf(stderr, "Erro ao alocar buffer da imagem\n");
                MPI_Finalize();
                return 1;
            }

            MPI_Recv(recv_buffer, 3 * width * recv_rows, MPI_UNSIGNED_CHAR, i,
                     0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            fwrite(recv_buffer, sizeof(unsigned char), 3 * width * recv_rows,
                   output_file);
            free(recv_buffer);
        }

        fclose(output_file);
    } else {
        MPI_Send(local_pixels, 3 * width * rows_to_compute, MPI_UNSIGNED_CHAR,
                 0, 0, MPI_COMM_WORLD);
    }

    free(local_pixels);
    MPI_Finalize();
    return 0;
}