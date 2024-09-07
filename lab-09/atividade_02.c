#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define N 5
#define NITER 5

int vet[N]; // variável global
sem_t sem;
sem_t barrier_sem;
int aux = 1;
int count = 0;
pthread_mutex_t count_mutex;

void barreira() {
  pthread_mutex_lock(&count_mutex);
  count++;
  if (count == N) {
    for (int i = 0; i < N; i++) {
      sem_post(&barrier_sem);
    }
    count = 0;
  }
  pthread_mutex_unlock(&count_mutex);
  sem_wait(&barrier_sem);
}

void *thread(void *threadid) {
  int tid = *((int *)threadid); // Corrigir a forma de acessar o tid
  vet[tid] = tid;
  printf("Thread %d chegou na barreira %d\n", tid, aux);
  barreira();
  printf("Thread %d saiu da barreira %d\n", tid, aux);
  aux++;

  for (int i = 0; i < NITER; i++) {
    vet[tid] = vet[tid] * vet[tid] + vet[tid - 1];
    printf("Thread %d chegou na barreira %d\n", tid, aux);
    barreira();
    printf("Thread %d saiu da barreira %d\n", tid, aux);
    aux++;
  }

  free(threadid); // Liberar a memória alocada
  pthread_exit(NULL);
}

int main() {
  pthread_t threads[N];

  sem_init(&sem, 0, 0); // inicializar semáforo
  sem_init(&barrier_sem, 0, 0); // inicializar semáforo da barreira
  pthread_mutex_init(&count_mutex, NULL);

  for (int i = 0; i < N; i++) {
    int *thread_id = malloc(sizeof(int)); // Alocar memória para o id da thread
    *thread_id = i;
    pthread_create(&threads[i], NULL, thread, (void *)thread_id);
  }

  for (int i = 0; i < N; i++) {
    pthread_join(threads[i], NULL);
  }

  sem_destroy(&sem); // destruir semáforo
  sem_destroy(&barrier_sem); // destruir semáforo da barreira
  pthread_mutex_destroy(&count_mutex);

  return 0;
}