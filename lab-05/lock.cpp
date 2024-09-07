//#include "stdafx.h"
#include <iostream>
#include <pthread.h>

#define NTHREADS  2

// variaveis globais compartilhadas entre as threads
volatile int s = 0;

// lock para exclusao mutua entre as threads
pthread_mutex_t mutex; 

// funcao executada pelas threads criadas no programa
void *ExecutaTarefa(void *threadid) {
    int i;
    int *tid = (int*) threadid;
    printf("Thread : %d esta executando...\n", *tid);
    for (i = 0; i < 100000; i++) {
        pthread_mutex_lock(&mutex); //entrada na secao critica
        s++; //secao critica 
        pthread_mutex_unlock(&mutex); //saida da secao critica
    }
    printf("Thread : %d terminou!\n", *tid);
    free(threadid);
    pthread_exit(NULL);
    return 0;
}

// funcao/thread principal do programa
int main(int argc, char *argv[]) {
    pthread_t tid[NTHREADS];
    int t, *id;

    // inicilaiza o mutex (lock de exclusao mutua)
    pthread_mutex_init(&mutex, NULL);

    // cria as threads
    for (t = 0; t < NTHREADS; t++) {
        if ((id = (int*) malloc(sizeof (int))) == NULL) {
            pthread_exit(NULL);
            return 1;
        }
        *id = t;

        printf("--Cria a thread %d\n", t);
        if (pthread_create(&tid[t], NULL, ExecutaTarefa, (void *) id)) {
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
