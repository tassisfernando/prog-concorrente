//#include "stdafx.h"
#include <iostream>
#include <pthread.h>

using namespace std;

#define NTHREADS  2

volatile short int turno=1; //para ilustrar a necessidade de volatile, compilar fazendo: gcc -O1 exemplo2-turn.c -lpthread -Wall sem usar o qualificador
//short int turno=1;

volatile int s = 0; //variavel compartilhada entre as threads

//funcao executada pelas threads
void *ExecutaTarefa (void *threadid) {
	int i; 
	int *tid = (int*) threadid;
	printf("Thread : %d esta executando...\n", *tid);
	for (i=0; i<10000000; i++) {
		//--entrada na SC
		while(turno != *tid) {;}
		//--SC
		//printf("Thread : %d esta na SC...\n", *tid);
		s++; //incrementa o contador de tarefas realizadas 
		//--saida da SC
		turno = (*tid+1)%2;
	}
	printf("Thread : %d terminou!\n", *tid);
	free(threadid);
	pthread_exit(NULL);
	return 0;
}

//thread principal
int main(int argc, char *argv[]) {
	pthread_t tid[NTHREADS];
	int t, *id;

	//--cria as threads
	for(t=0; t<NTHREADS; t++) {
		if((id=(int *)malloc(sizeof(int)))==NULL) {
			pthread_exit(NULL); 
			return -1;
		}
		*id = t;
		printf("--Cria a thread %d\n", t);
		if (pthread_create(&tid[t], NULL, ExecutaTarefa, (void *)id)) {
			printf("--ERRO: pthread_create()\n"); 
			return -1;
		}
	}

	//--espera todas as threads terminarem
	for (t=0; t<NTHREADS; t++) {
		if (pthread_join(tid[t], NULL)) {
			printf("--ERRO: pthread_join() \n"); 
			return -1;
		} 
	} 

	printf("Valor de s = %d\n", s);

	getchar();
	pthread_exit(NULL);
	return 1;
}
