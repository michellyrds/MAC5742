#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>

uint64_t shared_value = 0; // Valor compartilhado de 64 bits

void* writer_thread(void* arg) {
    uint32_t low_part = 0x12345678;
    uint32_t high_part = 0x87654321;

    // Simulando a escrita de 64 bits como duas de 32 bits
    *((uint32_t*)&shared_value) = low_part;
    usleep(10); // Pequeno atraso para aumentar a chance de leitura durante a escrita
    *((uint32_t*)&shared_value + 1) = high_part; 

    return NULL;
}

void* reader_thread(void* arg) {
    uint64_t value_read;

    value_read = *((uint64_t*)&shared_value);

    printf("Valor lido: 0x%lx\n", value_read);
    return NULL;
}

int main() {
    pthread_t writer, reader;

    pthread_create(&writer, NULL, writer_thread, NULL);
    pthread_create(&reader, NULL, reader_thread, NULL);

    pthread_join(writer, NULL);
    pthread_join(reader, NULL);

    return 0;
}
