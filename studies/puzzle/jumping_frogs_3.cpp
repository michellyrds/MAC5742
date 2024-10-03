#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

#define FROG 1
#define TOAD 2
#define EMPTY 0

int N, M; // Quantidade de rãs e sapos
std::vector<int> pond; // Lagoa de pedras
int empty_pos; // Posição vazia
int counter = 0; // Contador de tentativas sem sucesso
int max_counter = 100; // Limite para indicar deadlock

std::mutex mtx;
std::condition_variable cond;

void print_pond() {
    for (int i = 0; i < N + M + 1; i++) {
        if (pond[i] == FROG) {
            std::cout << "F ";
        } else if (pond[i] == TOAD) {
            std::cout << "T ";
        } else {
            std::cout << "_ ";
        }
    }
    std::cout << std::endl;
}

void frog(int index) {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);

        // Verifica se a rã pode pular
        while (!(index + 1 == empty_pos || (index + 2 == empty_pos && pond[index + 1] == TOAD))) {
            counter++;
            if (counter >= max_counter) {
                std::cout << "Deadlock detectado pelas rãs!" << std::endl;
                return;
            }
            cond.wait(lock); // Aguarda até que a condição mude
        }

        // Movimento possível
        pond[empty_pos] = FROG;
        pond[index] = EMPTY;
        empty_pos = index;
        counter = 0; // Zera o contador
        print_pond();

        cond.notify_all(); // Notifica as outras threads que a condição mudou
        lock.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Pausa para dar tempo às outras threads
    }
}

void toad(int index) {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);

        // Verifica se o sapo pode pular
        while (!(index - 1 == empty_pos || (index - 2 == empty_pos && pond[index - 1] == FROG))) {
            counter++;
            if (counter >= max_counter) {
                std::cout << "Deadlock detectado pelos sapos!" << std::endl;
                return;
            }
            cond.wait(lock); // Aguarda até que a condição mude
        }

        // Movimento possível
        pond[empty_pos] = TOAD;
        pond[index] = EMPTY;
        empty_pos = index;
        counter = 0; // Zera o contador
        print_pond();

        cond.notify_all(); // Notifica as outras threads que a condição mudou
        lock.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Pausa para dar tempo às outras threads
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cout << "Uso: " << argv[0] << " <N_rãs> <M_sapos>" << std::endl;
        return 1;
    }

    N = std::stoi(argv[1]);
    M = std::stoi(argv[2]);

    pond.resize(N + M + 1);

    // Inicializa a lagoa com as rãs à esquerda, os sapos à direita e uma pedra vazia no meio
    for (int i = 0; i < N; i++) {
        pond[i] = FROG;
    }
    for (int i = N; i < N + M; i++) {
        pond[i] = TOAD;
    }
    empty_pos = N + M;
    pond[empty_pos] = EMPTY;

    print_pond(); // Exibe o estado inicial

    std::vector<std::thread> frogs, toads;

    // Cria threads para as rãs
    for (int i = 0; i < N; i++) {
        frogs.emplace_back(frog, i);
    }

    // Cria threads para os sapos
    for (int i = 0; i < M; i++) {
        toads.emplace_back(toad, N + i);
    }

    // Espera pelas threads das rãs e sapos terminarem
    for (auto &f : frogs) {
        f.join();
    }
    for (auto &t : toads) {
        t.join();
    }

    return 0;
}
