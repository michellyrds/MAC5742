#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>

uint64_t shared_value = 0; // Valor compartilhado de 64 bits
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // Inicializa o mutex

void* writer_thread(void* arg) {
    uint32_t low_part = 0x12345678;
    uint32_t high_part = 0x87654321;

    pthread_mutex_lock(&mutex);

    // Simulando a escrita de 64 bits como duas de 32 bits
    *((uint32_t*)&shared_value) = low_part;
    usleep(10); // Pequeno atraso para aumentar a chance de leitura durante a escrita
    *((uint32_t*)&shared_value + 1) = high_part;

    pthread_mutex_unlock(&mutex);

    return NULL;
}

void* reader_thread(void* arg) {
    uint64_t value_read;

    pthread_mutex_lock(&mutex);

    value_read = *((uint64_t*)&shared_value);

    pthread_mutex_unlock(&mutex);

    printf("Valor lido: 0x%lx\n", value_read);
    return NULL;
}

int main() {
    pthread_t writer, reader;

    pthread_create(&writer, NULL, writer_thread, NULL);
    pthread_create(&reader, NULL, reader_thread, NULL);

    pthread_join(writer, NULL);
    pthread_join(reader, NULL);

    pthread_mutex_destroy(&mutex); // Destrói o mutex ao final

    return 0;
}
