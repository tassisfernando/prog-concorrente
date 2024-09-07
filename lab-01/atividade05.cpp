#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

#define NTHREADS  2
#define SIZE 20

//cria a estrutura de dados para armazenar os argumentos da thread
typedef struct {
	int idThread;
	int *vet;
} tv_Args;


//funcao para imprimir um vetor de inteiros
void imprimeVetor (int *vetor, int tamanho) {
	for (int i = 0; i < tamanho; i++){
		printf("%d\n", vetor[i] );
	}
	printf("\n\n");
}

//funcao executada pelas threads
void *IncrementaValores (void *arg) {
	tv_Args *args = (tv_Args *) arg;

	for (int i = args->idThread; i < SIZE; i += 2) {
		*(args->vet + i) += 10;
	}

	printf("Sou a thread %d \n", args->idThread + 1);
	free(arg);

	pthread_exit(NULL);
	return 0;
}

//funcao principal do programa
int main() {
	pthread_t tid_sistema[NTHREADS];
	int t;
	tv_Args *arg; //receber� os argumentos para a thread

	//Para criar e dar valor ao vetor na main
	int vet[SIZE];
	for (int i = 0; i < SIZE; vet[i++]=i);

	printf("Vetor antes de incrementar valores: \n");
	imprimeVetor(vet, SIZE);

	for(t=0; t<NTHREADS; t++) {
		arg = (tv_Args *) malloc(sizeof(tv_Args));
		if (arg == NULL) {
			printf("--ERRO: malloc()\n"); 
			return -1;
		}

		arg->idThread = t; 
		//Para copiar o endereco do vetor na struct antes de chamar a thread
		arg->vet = &vet[0];


		printf("--Cria a thread %d\n", t+1);
		if (pthread_create(&tid_sistema[t], NULL, IncrementaValores, (void*) arg)) {
			printf("--ERRO: pthread_create()\n"); 
			return -1;
		}
	}

	//--espera todas as threads terminarem
	for (t=0; t<NTHREADS; t++) {
		if (pthread_join(tid_sistema[t], NULL)) {
			printf("--ERRO: pthread_join() \n"); 
			return -1; 
		} 
	}

	printf("\n\nVetor depois de incrementar valores: \n");
	imprimeVetor(vet, SIZE);

	printf("--Thread principal terminou\n");
	getchar();
	pthread_exit(NULL);
}

