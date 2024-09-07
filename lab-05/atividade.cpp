#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define N 10000 // Tamanho da matriz (N x N)

int matriz[N][N]; // Matriz global
pthread_mutex_t mutex; // Mutex para exclusão mútua
int contador_thread1 = 0, contador_thread2 = 0; // Contadores de células preenchidas

// Função executada pelas threads
void *preenche_matriz(void *threadid) {
    int i, j;
    int *tid = (int *) threadid;

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            pthread_mutex_lock(&mutex); // Adquire o lock

            if (matriz[i][j] == 0) { // Verifica se a célula está vazia
                matriz[i][j] = *tid; // Preenche a célula com o ID da thread
                if (*tid == 1) {
                    contador_thread1++;
                } else {
                    contador_thread2++;
                }
            }

            pthread_mutex_unlock(&mutex); // Libera o lock
            usleep(100);
        }
    }

    free(threadid);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    pthread_t threads[2];
    int *id[2];
    int i;

    // Inicializa a matriz com zeros
    for (i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matriz[i][j] = 0;
        }
    }

    // Inicializa o mutex
    pthread_mutex_init(&mutex, NULL);

    // Cria as threads
    for (i = 0; i < 2; i++) {
        id[i] = (int *) malloc(sizeof(int));
        *id[i] = i + 1; // Identificadores 1 e 2
        pthread_create(&threads[i], NULL, preenche_matriz, (void *) id[i]);
    }

    // Aguarda a finalização das threads
    for (i = 0; i < 2; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destrói o mutex
    pthread_mutex_destroy(&mutex);

    // Exibe os resultados
    printf("Thread 1 preencheu %d células.\n", contador_thread1);
    printf("Thread 2 preencheu %d células.\n", contador_thread2);

    if (contador_thread1 > contador_thread2) {
        printf("Thread 1 venceu!\n");
    } else if (contador_thread2 > contador_thread1) {
        printf("Thread 2 venceu!\n");
    } else {
        printf("Empate!\n");
    }

    return 0;
}