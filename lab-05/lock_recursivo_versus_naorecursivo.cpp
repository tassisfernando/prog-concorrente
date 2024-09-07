//#include "stdafx.h"
#include <iostream>
#include <pthread.h>

#define NTHREADS  2

// variaveis globais compartilhadas entre as threads
volatile int s = 0;

// lock para exclusao mutua entre as threads
pthread_mutex_t mutex;

void F(void *threadid) {
    int *tid = (int*) threadid;
    printf("Thread : %d esta executando F...\n", *tid);
    pthread_mutex_lock(&mutex);
    printf("Thread : %d deu lock em F...\n", *tid);
    s++;
    printf("Thread : %d deu unlock em F...\n", *tid);
    pthread_mutex_unlock(&mutex);
    printf("Thread : %d terminou F!\n", *tid);
}

void *G(void *threadid) {
    int *tid = (int*) threadid;
    printf("Thread : %d esta executando G...\n", *tid);
    pthread_mutex_lock(&mutex);
    printf("Thread : %d deu lock em G...\n", *tid);
    F(threadid);
    printf("Thread : %d deu unlock em G...\n", *tid);
    pthread_mutex_unlock(&mutex);
    printf("Thread : %d terminou G!\n", *tid);
    pthread_exit(NULL);
}


// funcao/thread principal do programa

int main(int argc, char *argv[]) {
    pthread_t tid[NTHREADS];
    int t, *id;

    // lock NAO RECURSIVO
    pthread_mutex_init(&mutex, NULL);

    // lock RECURSIVO
//    pthread_mutexattr_t mutexAttr;
//    pthread_mutexattr_init(&mutexAttr);
//    pthread_mutexattr_settype(&mutexAttr, PTHREAD_MUTEX_RECURSIVE);
//    pthread_mutex_init(&mutex, &mutexAttr);

    // cria as threads
    for (t = 0; t < NTHREADS; t++) {
        if ((id = (int*) malloc(sizeof (int))) == NULL) {
            pthread_exit(NULL);
            return 1;
        }
        *id = t;

        printf("--Cria a thread %d\n", t);
        if (pthread_create(&tid[t], NULL, G, (void *) id)) {
            printf("--ERRO: pthread_create()\n");
            return -1;
        }
    }

    // espera todas as threads terminarem para exibir o valor final de s
    for (t = 0; t < NTHREADS; t++) {
        if (pthread_join(tid[t], NULL)) {
            printf("--ERRO: pthread_join() \n");
            return -1;
        }
    }

    // desaloca o lock de exclusao mutua
    pthread_mutex_destroy(&mutex);

    printf("Valor de s = %d\n", s);
    getchar();
    pthread_exit(NULL);
    return 0;
}
