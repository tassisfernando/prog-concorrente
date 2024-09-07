#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define NTHREADS 4

// Variáveis globais
sem_t condt3, condt4; // Semáforos para sincronizar a ordem de execução das threads

// Função executada pela thread 1
void *t1(void *threadid) {
    printf("Thread 1: olá, tudo bem?\n");
    sem_post(&condt3); // Permite que T3 execute
    sem_post(&condt4); // Permite que T4 execute
    pthread_exit(NULL);
}

// Função executada pela thread 2
void *t2(void *threadid) {
    printf("Thread 2: hello!\n");
    sem_post(&condt3); // Permite que T3 execute
    sem_post(&condt4); // Permite que T4 execute
    pthread_exit(NULL);
}

// Função executada pela thread 3
void *t3(void *threadid) {
    sem_wait(&condt3); // Espera T1 ou T2 executar
    sem_wait(&condt3); // Espera T1 ou T2 executar
    printf("Thread 3: até mais tarde.\n");
    pthread_exit(NULL);
}

// Função executada pela thread 4
void *t4(void *threadid) {
    sem_wait(&condt4); // Espera T1 ou T2 executar
    sem_wait(&condt4); // Espera T1 ou T2 executar
    printf("Thread 4: tchau!\n");
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    pthread_t threads[NTHREADS];
    int thread_ids[NTHREADS];

    // Inicializa os semáforos
    sem_init(&condt3, 0, 0);
    sem_init(&condt4, 0, 0);

    // Cria as threads
    pthread_create(&threads[0], NULL, t1, (void*)&thread_ids[0]);
    pthread_create(&threads[1], NULL, t2, (void*)&thread_ids[1]);
    pthread_create(&threads[2], NULL, t3, (void*)&thread_ids[2]);
    pthread_create(&threads[3], NULL, t4, (void*)&thread_ids[3]);

    // Espera todas as threads terminarem
    for (int i = 0; i < NTHREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroi os semáforos
    sem_destroy(&condt3);
    sem_destroy(&condt4);

    return 0;
}