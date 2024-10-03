#include <pthread.h>
#include <unistd.h>

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <new>
#include <vector>

using namespace std;

#define MAX_COUNT 100

void print_vector(vector<int> vector, int size);

typedef struct JumpingFrogsPuzzle {
    int size;
    vector<int> stones;  // len(stones) == 2*size+1
    int empty_pos;

    int* curr_thread_id;
    pthread_mutex_t lock;

    int counter;

    JumpingFrogsPuzzle(int size) {
        this->size = size;

        vector<int> stones(2 * size + 1);

        for (int i = 0, j = size; i < size; i++, j++) {
            stones[i] = 1;
            stones[j + 1] = 2;
        }
        this->stones = stones;
        this->empty_pos = size + 1;
        this->counter = 0;

        cout << "Initial stones sequence: ";
        print_vector(stones, 2 * size + 1);
    }
} JumpingFrogsPuzzle;

void print_vector(vector<int> vector, int size) {
    for (int i = 0; i < size; i++) {
        cout << vector[i] << " ";
    }
    cout << endl;
}

void blue_frog_jump(JumpingFrogsPuzzle* puzzle, int thread_id) {
    if (thread_id < puzzle->size - 1 && puzzle->stones[thread_id + 1] == 1 &&
        puzzle->stones[thread_id + 2] == 0) {
        puzzle->stones[thread_id] = 0;
        puzzle->stones[thread_id + 2] = 1;

        puzzle->empty_pos = thread_id;
        puzzle->counter = 0;
        cout << "blue frog with thread_id " << thread_id << " jumped!" << endl;
        cout << "Stones: ";
        print_vector(puzzle->stones, 2 * puzzle->size + 1);
    } else if (thread_id < puzzle->size && puzzle->stones[thread_id + 1] == 0) {
        puzzle->stones[thread_id] = 0;
        puzzle->stones[thread_id + 1] = 1;

        puzzle->empty_pos = thread_id;
        puzzle->counter = 0;
        cout << "blue frog with thread_id " << thread_id << " jumped!" << endl;
        cout << "Stones: ";
        print_vector(puzzle->stones, 2 * puzzle->size + 1);
    } else {
        puzzle->counter++;
    }
}

void green_frog_jump(JumpingFrogsPuzzle* puzzle, int thread_id) {
    int pos = puzzle->size + 1 + thread_id;

    if (pos > puzzle->size + 1 && puzzle->stones[pos - 1] == 2 &&
        puzzle->stones[pos - 2] == 0) {
        puzzle->stones[pos] = 0;
        puzzle->stones[pos - 2] = 2;

        puzzle->empty_pos = pos;
        puzzle->counter = 0;

        cout << "green frog with thread_id " << thread_id << " jumped!" << endl;
        cout << "Stones: ";
        print_vector(puzzle->stones, 2 * puzzle->size + 1);
    } else if (pos > puzzle->size && puzzle->stones[pos - 1] == 0) {
        puzzle->stones[pos] = 0;
        puzzle->stones[pos - 1] = 2;

        puzzle->empty_pos = pos;
        puzzle->counter = 0;

        cout << "green frog with thread_id " << thread_id << " jumped!" << endl;
        cout << "Stones: ";
        print_vector(puzzle->stones, 2 * puzzle->size + 1);
    } else {
        puzzle->counter++;
    }
}

bool solved(JumpingFrogsPuzzle* puzzle) {
    int size = puzzle->size;

    if (puzzle->stones[size + 1] != 0) return false;

    for (int i = 0, j = size; i < size; i++, j++) {
        if (puzzle->stones[i] != 2 || puzzle->stones[j + 1] != 1) return false;
    }

    return true;
}

void check_deadlock(JumpingFrogsPuzzle* puzzle) {
    if (solved(puzzle)) {
        cout << "puzzle solved!" << endl;
        exit(EXIT_SUCCESS);
    }

    if (puzzle->counter > MAX_COUNT) {
        cout << "deadlock detected!" << endl;
        exit(EXIT_FAILURE);
    }
}

void check_status(int status) {
    if (status != 0) {
        cout << "pthread_create returned error code " << status << endl;
        exit(EXIT_FAILURE);
    }
}

void* blue_frog_try_jump(void* arg) {
    JumpingFrogsPuzzle* puzzle = (JumpingFrogsPuzzle*)arg;
    int thread_id = *puzzle->curr_thread_id;

    while (true) {
        pthread_mutex_lock(&puzzle->lock);
        blue_frog_jump(puzzle, thread_id);
        check_deadlock(puzzle);
        pthread_mutex_unlock(&puzzle->lock);
        sleep(1);
    }

    return NULL;
}

void* green_frog_try_jump(void* arg) {
    JumpingFrogsPuzzle* puzzle = (JumpingFrogsPuzzle*)arg;
    int thread_id = *puzzle->curr_thread_id;

    while (true) {
        pthread_mutex_lock(&puzzle->lock);
        green_frog_jump(puzzle, thread_id);
        check_deadlock(puzzle);
        pthread_mutex_unlock(&puzzle->lock);
        sleep(1);
    }

    return NULL;
}

int main(int argc, char** argv) {
    static int NUM_THREADS = stoi(argv[1]);

    JumpingFrogsPuzzle puzzle(NUM_THREADS);

    pthread_t blue_frogs[NUM_THREADS], green_frogs[NUM_THREADS];
    int status;

    pthread_mutex_init(&puzzle.lock, NULL);

    for (int i = 0; i < NUM_THREADS; i++) {
        puzzle.curr_thread_id = new int(i);
        status = pthread_create(&blue_frogs[i], NULL, &blue_frog_try_jump,
                                (void*)&puzzle);
        check_status(status);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        puzzle.curr_thread_id = new int(i);
        status = pthread_create(&green_frogs[i], NULL, &green_frog_try_jump,
                                (void*)&puzzle);
        check_status(status);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(blue_frogs[i], NULL);
        pthread_join(green_frogs[i], NULL);
    }

    free(puzzle.curr_thread_id);
    pthread_mutex_destroy(&puzzle.lock);

    return EXIT_SUCCESS;
}