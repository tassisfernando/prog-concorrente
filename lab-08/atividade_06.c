#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define N 5 // Número de threads leitoras
#define M 2 // Número de threads escritoras

// Estrutura compartilhada
typedef struct {
    int contador;
    int thread_id;
} SharedData;

SharedData data = {0, 0};

// Variáveis de sincronização
pthread_mutex_t mutex;
pthread_cond_t cond_leitores, cond_escritores;
int leitores = 0;
int escritores = 0;
int esperando_escritores = 0;

// Função executada pelas threads leitoras
void *leitor(void *arg) {
    int id = *((int *)arg);
    while (1) {
        pthread_mutex_lock(&mutex);
        while (escritores > 0 || esperando_escritores > 0) {
            pthread_cond_wait(&cond_leitores, &mutex);
        }
        leitores++;
        pthread_mutex_unlock(&mutex);

        // Leitura da variável compartilhada
        SharedData local_data = data;
        printf("Leitor %d leu: contador = %d, thread_id = %d\n", id, local_data.contador, local_data.thread_id);

        // Processamento "bobo"
        usleep(100000);

        pthread_mutex_lock(&mutex);
        leitores--;
        if (leitores == 0) {
            pthread_cond_signal(&cond_escritores);
        }
        pthread_mutex_unlock(&mutex);

        usleep(100000); // Simula tempo entre leituras
    }
    pthread_exit(NULL);
}

// Função executada pelas threads escritoras
void *escritor(void *arg) {
    int id = *((int *)arg);
    while (1) {
        pthread_mutex_lock(&mutex);
        esperando_escritores++;
        while (leitores > 0 || escritores > 0) {
            pthread_cond_wait(&cond_escritores, &mutex);
        }
        esperando_escritores--;
        escritores++;
        pthread_mutex_unlock(&mutex);

        // Escrita na variável compartilhada
        data.contador++;
        data.thread_id = id;
        printf("Escritor %d escreveu: contador = %d, thread_id = %d\n", id, data.contador, data.thread_id);

        pthread_mutex_lock(&mutex);
        escritores--;
        if (esperando_escritores > 0) {
            pthread_cond_signal(&cond_escritores);
        } else {
            pthread_cond_broadcast(&cond_leitores);
        }
        pthread_mutex_unlock(&mutex);

        usleep(200000); // Simula tempo entre escritas
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t leitores[N], escritores[M];
    int ids_leitores[N], ids_escritores[M];

    // Inicializa as variáveis de sincronização
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_leitores, NULL);
    pthread_cond_init(&cond_escritores, NULL);

    // Cria as threads leitoras
    for (int i = 0; i < N; i++) {
        ids_leitores[i] = i + 1;
        pthread_create(&leitores[i], NULL, leitor, &ids_leitores[i]);
    }

    // Cria as threads escritoras
    for (int i = 0; i < M; i++) {
        ids_escritores[i] = i + 1;
        pthread_create(&escritores[i], NULL, escritor, &ids_escritores[i]);
    }

    // Espera as threads terminarem (nunca vão terminar neste exemplo)
    for (int i = 0; i < N; i++) {
        pthread_join(leitores[i], NULL);
    }
    for (int i = 0; i < M; i++) {
        pthread_join(escritores[i], NULL);
    }

    // Destroi as variáveis de sincronização
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_leitores);
    pthread_cond_destroy(&cond_escritores);

    return 0;
}