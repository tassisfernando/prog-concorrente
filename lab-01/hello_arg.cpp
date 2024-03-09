#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

#define NTHREADS  10

//funcao executada pelas threads
void *PrintHello (void *arg) {
	int *idThread = (int *) arg;

	printf("Hello World da thread: %d\n", *idThread);
	free(arg);

	pthread_exit(NULL);
	return 0;
}

//funcao principal do programa
int main() {
	pthread_t tid_sistema[NTHREADS];
	int t;
	int *arg; //receberá o argumento para a thread

	for(t=0; t<NTHREADS; t++) {
		printf("--Aloca e preenche argumento para thread %d\n", t+1);
		arg = (int *) malloc(sizeof(int));
		if (arg == NULL) {
			printf("--ERRO: malloc()\n"); 
			return -1;
		}
		*arg = t+1; 

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

