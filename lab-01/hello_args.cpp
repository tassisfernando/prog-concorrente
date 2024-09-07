#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

#define NTHREADS  10

//cria a estrutura de dados para armazenar os argumentos da thread
typedef struct {
	int idThread, nThreads;
} t_Args;

//funcao executada pelas threads
void *PrintHello (void *arg) {
	t_Args *args = (t_Args *) arg;

	printf("Sou a thread %d de %d threads\n", args->idThread, args->nThreads);
	free(arg);

	pthread_exit(NULL);
	return 0;
}

//funcao principal do programa
int main() {
	pthread_t tid_sistema[NTHREADS];
	int t;
	t_Args *arg; //receberá os argumentos para a thread

	for(t=0; t<NTHREADS; t++) {
		printf("--Aloca e preenche argumentos para thread %d\n", t+1);
		arg = (t_Args *) malloc(sizeof(t_Args));
		if (arg == NULL) {
			printf("--ERRO: malloc()\n"); 
			return -1;
		}
		arg->idThread = t+1; 
		arg->nThreads = NTHREADS; 

		printf("--Cria a thread %d\n", t+1);
		if (pthread_create(&tid_sistema[t], NULL, PrintHello, (void*) arg)) {
			printf("--ERRO: pthread_create()\n"); 
			return -1;
		}
	}

	printf("--Thread principal terminou\n");
	getchar();
	pthread_exit(NULL);
}

