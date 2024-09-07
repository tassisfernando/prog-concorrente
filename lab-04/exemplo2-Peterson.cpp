#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

#define NTHREADS  2

volatile short int queroEntrar[2];
volatile short int turno;
//short int queroEntrar[2];
//short int turno;

//variavel compartilhada entre as threads
volatile int s = 0;

//funcao executada pelas threads
void *ExecutaTarefa(void *threadid) {
    int i;
    int *tid = (int*) threadid;
    printf("Thread : %d esta executando...\n", *tid);
    for (i = 0; i < 10000000; i++) {
        //--entrada na SC
        queroEntrar[*tid] = 1;
        turno = (*tid + 1) % 2;
        while (queroEntrar[(*tid + 1) % 2] && turno == (*tid + 1) % 2);
        
        //--SC
        s++; //incrementa o contador de tarefas realizadas 
        //--saida da SC
        
        queroEntrar[*tid] = 0;
    }
    printf("Thread : %d terminou!\n", *tid);
    free(threadid);
    pthread_exit(NULL);
}

//thread principal
int main(int argc, char *argv[]) {
    pthread_t tid[NTHREADS];
    int t, *id;

    queroEntrar[0] = 0;
    queroEntrar[1] = 0;

    //--cria as threads
    for (t = 0; t < NTHREADS; t++) {
        if ((id = (int *) malloc(sizeof (int))) == NULL) {
            pthread_exit(NULL);
            return -1;
        }
        *id = t;
        printf("--Cria a thread %d\n", t);
        if (pthread_create(&tid[t], NULL, ExecutaTarefa, (void *) id)) {
            printf("--ERRO: pthread_create()\n");
            exit(-1);
        }
    }

    //--espera todas as threads terminarem
    for (t = 0; t < NTHREADS; t++) {
        if (pthread_join(tid[t], NULL)) {
            printf("--ERRO: pthread_join() \n");
            exit(-1);
        }
    }

    printf("Valor de s = %d\n", s);

    pthread_exit(NULL);
    return 1;
}
