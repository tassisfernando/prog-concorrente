/* Descricao: implementa  o problema dos produtores/consumidores usando variaveis de condicao da biblioteca Pthreads*/

// #include "stdafx.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h> //uso do Sleep

#define N 5
#define P 4
#define C 2

//variaveis do problema
int Buffer[N];
int count=0, in=0, out=0;

//variaveis para sincronizacao
pthread_mutex_t mutex;
pthread_cond_t cond_cons, cond_prod;

//inicializa o buffer
void IniciaBuffer(int n) {
	int i;
	for(i=0; i<n; i++)
		Buffer[i] = 0;
}

//imprime o buffer
void ImprimeBuffer(int n) {
	int i;
	for(i=0; i<n; i++)
		printf("%d ", Buffer[i]);
	printf("\n");
}

//insere um elemento no Buffer ou bloqueia a thread caso o Buffer esteja cheio
void Insere (int item, int id) {
	pthread_mutex_lock(&mutex);
	printf("P[%d] quer inserir\n", id);
	while(count == N) {
		printf("P[%d] bloqueou\n", id);
		pthread_cond_wait(&cond_prod, &mutex);
		printf("P[%d] desbloqueou\n", id);
	}
	Buffer[in] = item;
	in = (in + 1)%N;
	count++;
	printf("P[%d] inseriu\n", id);
	pthread_cond_signal(&cond_cons);
	printf("Insercao: ");
	ImprimeBuffer(N);
	pthread_mutex_unlock(&mutex);
}

//retira um elemento no Buffer ou bloqueia a thread caso o Buffer esteja vazio
int Retira (int id) {
	int item;
	pthread_mutex_lock(&mutex);
	printf("C[%d] quer consumir\n", id);
	while(count == 0) {
		printf("C[%d] bloqueou\n", id);
		pthread_cond_wait(&cond_cons, &mutex);
		printf("C[%d] bloqueou\n", id);
	}
	item = Buffer[out];
	Buffer[out] = 0;
	out = (out + 1)%N;
	count--;
	printf("C[%d] consumiu %d\n", id, item);
	pthread_cond_signal(&cond_prod);
	printf("Remocao: ");
	ImprimeBuffer(N);
	pthread_mutex_unlock(&mutex);
	return item;
}

//thread produtora
void * produtor(void * arg) {
	int *id = (int *) arg;
	printf("Sou a thread produtora %d\n", *id);
	while(1) {
		//produz item...
		Sleep(1); //para simular um tempo de producao
		Insere(*id, *id);
	} 
	free(arg);
	pthread_exit(NULL);
}

//thread consumidora
void * consumidor(void * arg) {
	int *id = (int *) arg;
	int item;
	printf("Sou a thread consumidora %d\n", *id);
	while(1) {
		item = Retira(*id);
		//consome item...
		Sleep(1000);//para simular um tempo de consumo
	} 
	free(arg);
	pthread_exit(NULL);
}

//insere N elementos no Buffer de uma vez
void InsereTudo(int id) {
	pthread_mutex_lock(&mutex);
	printf("P[%d] quer inserir tudo\n", id);
	while(count > 0) {
		printf("P[%d] bloqueou\n", id);
		pthread_cond_wait(&cond_prod, &mutex);
		printf("P[%d] desbloqueou\n", id);
	}
	for(int i = 0; i < N; i++) {
		Buffer[in] = id;
		in = (in + 1) % N;
		count++;
	}
	printf("P[%d] inseriu tudo\n", id);
	pthread_cond_broadcast(&cond_cons);
	printf("Insercao de tudo: ");
	ImprimeBuffer(N);
	pthread_mutex_unlock(&mutex);
}

//thread produtora especial
void * produtorEspecial(void * arg) {
	int *id = (int *) arg;
	printf("Sou a thread produtora especial %d\n", *id);
	while(1) {
		//produz N itens...
		Sleep(1); //para simular um tempo de producao
		InsereTudo(*id);
	} 
	free(arg);
	pthread_exit(NULL);
}


//funcao principal
int main(void) {
	//variaveis auxiliares
	int i;

	//identificadores das threads
	pthread_t tid[P+C];
	int *id[P+C];

	//aloca espaco para os IDs das threads
	for(i=0; i<P+C;i++) {
		id[i] = (int *)malloc(sizeof(int));
		if(id[i] == NULL) exit(-1);
		*id[i] = i+1;
	}

	//inicializa o Buffer
	IniciaBuffer(N);  

	//inicializa as variaveis de sincronizacao
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond_cons, NULL);
	pthread_cond_init(&cond_prod, NULL);

	//cria as threads produtoras
	for(i=0; i<P; i++) {
		if(pthread_create(&tid[i], NULL, produtor, (void *) id[i])) exit(-1);
	} 

	//cria as threads consumidoras
	for(i=0; i<C; i++) {
		if(pthread_create(&tid[i+P], NULL, consumidor, (void *) id[i+P])) exit(-1);
	} 

	pthread_exit(NULL);
	return 1;
}
