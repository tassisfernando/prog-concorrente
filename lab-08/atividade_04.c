#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <math.h>

#define BUFFER_SIZE 5
#define NUM_ELEMENTS 100

// Buffer e variáveis globais
int buffer[BUFFER_SIZE];
int in = 0, out = 0;
sem_t empty, full;
pthread_mutex_t mutex;

// Função para determinar se um número é primo
int ehPrimo(long unsigned int n) {
    int i;
    if (n <= 1) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    for (i = 3; i < sqrt(n) + 1; i += 2) {
        if (n % i == 0) return 0;
    }
    return 1;
}

// Função executada pela thread produtora
void *produtor(void *arg) {
    for (int t = 0; t < NUM_ELEMENTS; t++) {
        int item = 3 * t * t + 7 * t;
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        buffer[in] = item;
        in = (in + 1) % BUFFER_SIZE;
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }
    pthread_exit(NULL);
}

// Função executada pela thread consumidora
void *consumidor(void *arg) {
    for (int i = 0; i < NUM_ELEMENTS; i++) {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        int item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
        if (ehPrimo(item)) {
            printf("Número %d é primo.\n", item);
        } else {
            printf("Número %d não é primo.\n", item);
        }
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t prod, cons;

    // Inicializa os semáforos e o mutex
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    // Cria as threads
    pthread_create(&prod, NULL, produtor, NULL);
    pthread_create(&cons, NULL, consumidor, NULL);

    // Espera as threads terminarem
    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    // Destroi os semáforos e o mutex
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}