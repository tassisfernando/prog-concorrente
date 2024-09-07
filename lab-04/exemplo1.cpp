//#include "stdafx.h"
#include <iostream>
#include <pthread.h>

using namespace std;

#define NTHREADS 3

//variavel compartilhada entre as threads
int x = 0; 

//funcao executada pela thread 1
void *t1(void *threadid) {
    int *tid = (int*) threadid;
    printf("Thread : %d esta executando...\n", *tid);
    x = 1;
    printf("Thread : %d terminou!\n", *tid);
    free(threadid);
    pthread_exit(NULL);
    return 0;
}

//funcao executada pela thread 2
void *t2(void *threadid) {
    int *tid = (int*) threadid;
    printf("Thread : %d esta executando...\n", *tid);
    x = 2;
    printf("Thread : %d terminou!\n", (int) *tid);
    free(threadid);
    pthread_exit(NULL);
    return 0;
}

//funcao executada pela thread 3
void *t3(void *threadid) {
    int y;
    int *tid = (int*) threadid;
    printf("Thread : %d esta executando...\n", *tid);
    y = x;
    printf("Thread : %d terminou!\n", *tid);
    printf("Valor de y = %d\n", y);
    free(threadid);
    pthread_exit(NULL);
    return 0;
}

//thread principal
int main(int argc, char *argv[]) {
    pthread_t tid[NTHREADS];
    int *t;

    //cria as 3 threads
    if ((t = (int *) malloc(sizeof (int)))) {
        *t = 1;
        if (pthread_create(&tid[0], NULL, t1, (void *) t)) {
            printf("--ERRO: pthread_create()\n");
            return -1;
        }
    } else {
        pthread_exit(NULL);
        return 0;
    }

    if ((t = (int *) malloc(sizeof (int)))) {
        *t = 2;
        if (pthread_create(&tid[1], NULL, t2, (void *) t)) {
            printf("--ERRO: pthread_create()\n");
            return -1;
        }
    } else {
        pthread_exit(NULL);
        return 0;
    }

    if ((t = (int *) malloc(sizeof (int)))) {
        *t = 3;
        if (pthread_create(&tid[2], NULL, t3, (void *) t)) {
            printf("--ERRO: pthread_create()\n");
            return -1;
        }
    } else {
        pthread_exit(NULL);
        return 0;
    }

    getchar();
    pthread_exit(NULL);
    return 0;
}
