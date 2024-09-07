#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

#define NTHREADS  10

//--funcao executada pelas threads
void *PrintHello (void * arg) {

	printf("Hello World\n");

	pthread_exit(NULL);
	return 0;
}


//--funcao principal do programa
int main() {
	pthread_t tid_sistema[NTHREADS]; //identificadoes das threads no sistema
	int t; //variavel auxiliar

	for(t=0; t<NTHREADS; t++) {
		printf("--Cria a thread %d\n", t+1);
		if (pthread_create(&tid_sistema[t], NULL, PrintHello, NULL)) {
			printf("--ERRO: pthread_create()\n"); 
			return -1;
		}
	}

	printf("--Thread principal terminou\n");
	getchar();
	pthread_exit(NULL);
}

