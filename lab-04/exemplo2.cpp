//#include "stdafx.h"
#include <iostream>
#include <pthread.h>

using namespace std;

#define NTHREADS  2

//variavel compartilhada entre as threads
volatile int s = 0; 

//funcao executada pelas threads
void *ExecutaTarefa(void *threadid) {
    int i;
    int *tid = (int*) threadid;
    printf("Thread : %d esta executando...\n", *tid);
    for (i = 0; i < 10000000; i++) {
        s++; //incrementa a variavel compartilhada 
    }
    printf("Thread : %d terminou!\n", *tid);
    free(threadid);
    pthread_exit(NULL);
    return 0;
}

int main(int argc, char *argv[]) {
    pthread_t tid[NTHREADS];
    int t, *id;

    //--cria as threads
    for (t = 0; t < NTHREADS; t++) {
        if ((id = (int *) malloc(sizeof (int))) == NULL) {
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

    //--espera todas as threads terminarem
    for (t = 0; t < NTHREADS; t++) {
        if (pthread_join(tid[t], NULL)) {
            printf("--ERRO: pthread_join() \n");
            return -1;
        }
    }

    printf("Valor de s = %d\n", s);

    getchar();
    pthread_exit(NULL);
    return 0;
}

