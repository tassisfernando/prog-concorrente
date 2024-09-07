#include <iostream>
#include <pthread.h>
#include <malloc.h>
#include <time.h>

using namespace std;

#define NTHREADS 1500

// Variáveis globais
float **mat1, **mat2, **mat3; // Matrizes de entrada e saída
int linha1, col1, linha2, col2; // Dimensões das matrizes de entrada

// Estrutura para passar argumentos para a função da thread
struct thread_data {
    int thread_id;
};

// Calcula uma linha na matriz de saida
void calculaLinha(int numLinha) {
    int i, j, n = (int) numLinha;
    float m;
    for (i = 0; i < col2; i++) {
        m = 0;
        for (j = 0; j < col1; j++) {
            m += mat1[n][j] * mat2[j][i];
        }
        mat3[n][i] = m;
    }
}

// Função executada pela thread
void *multiplicaMatriz(void *threadarg) {
    struct thread_data *my_data;
    my_data = (struct thread_data *) threadarg;
    int id = my_data->thread_id;
    int start = id * (linha1 / NTHREADS);
    int end = (id + 1) * (linha1 / NTHREADS);
    
    // Calcula as linhas atribuídas a esta thread
    for (int k = start; k < end; k++) {
        calculaLinha(k);
    }

    pthread_exit(NULL);
    return 0;
}

int main(int argc, char *argv[]) {
    int i, j; // Variáveis auxiliares para os loops
    FILE *fentrada1, *fentrada2, *fsaida; // Arquivos de entrada e saida

    // Abertura dos arquivos de entrada e o de saída
    fentrada1 = fopen("dados/A2048x2048.txt", "r");
    fentrada2 = fopen("dados/A2048x2048.txt", "r");
    fsaida = fopen("dados/A2048x2048_result.txt", "w");
    if (fentrada1 == NULL) perror("Error opening file fentrada1");
    if (fentrada2 == NULL) perror("Error opening file fentrada2");
    if (fsaida == NULL) perror("Error opening file fsaida");

    // Variáveis para calcular o tempo
    clock_t tStart;

    // Inicia o contador para o tempo de entrada de dados
    tStart = clock();

    fscanf(fentrada1, "%d %d", &linha1, &col1);
    fscanf(fentrada2, "%d %d", &linha2, &col2);

    // Verifica se as matrizes de entrada podem ser multiplicadas
    if (col1 != linha2) {
        puts("Número de colunas da primeira matriz deve ser igual ao número de linhas da segunda matriz.");
        return -1;
    }

    // Aloca e le a primeira matriz
    if ((mat1 = (float **)malloc(sizeof(float *) * linha1)) == NULL) return -1;
    for (i = 0; i < linha1; ++i) {
        if ((mat1[i] = (float *)malloc(sizeof(float) * col1)) == NULL) return -1;
        for (j = 0; j < col1; ++j) {
            fscanf(fentrada1, "%f", &mat1[i][j]);
        }
    }
    fclose(fentrada1);

    // Aloca e le a segunda matriz
    if ((mat2 = (float **)malloc(sizeof(float *) * linha2)) == NULL) return -1;
    for (i = 0; i < linha2; ++i) {
        if ((mat2[i] = (float *)malloc(sizeof(float) * col2)) == NULL) return -1;
        for (j = 0; j < col2; ++j) {
            fscanf(fentrada2, "%f", &mat2[i][j]);
        }
    }
    fclose(fentrada2);

    // Aloca a matriz resultado
    if ((mat3 = (float **)malloc(sizeof(float *) * linha1)) == NULL) return -1;
    for (i = 0; i < col2; ++i) {
        if ((mat3[i] = (float *)malloc(sizeof(float) * col2)) == NULL) return -1;
    }

    // Encerra o contador para tempo de entrada de dados
    cout << "Tempo de Entrada de Dados: " << (double)(clock() - tStart) / CLOCKS_PER_SEC << endl;

    // Inicia o contador para o tempo de processamento
    tStart = clock();


    // Cria as threads
    pthread_t threads[NTHREADS];
    struct thread_data td[NTHREADS];
    for (int t = 0; t < NTHREADS; t++) {
        td[t].thread_id = t;
        pthread_create(&threads[t], NULL, multiplicaMatriz, (void *)&td[t]);
    }

    // Espera que todas as threads terminem
    for (int t = 0; t < NTHREADS; t++) {
        pthread_join(threads[t], NULL);
    }

    // Encerra o contador para tempo de processamento
    cout << "Tempo de Processamento: " << (double)(clock() - tStart) / CLOCKS_PER_SEC << endl;

    // Inicia o contador para o tempo de saida de dados
    tStart = clock();

    // Imprime a matriz resultante no arquivo de saida
    for (i = 0; i < linha1; i++) {
        for (j = 0; j < col2; j++) {
            fprintf(fsaida, "%f ", mat3[i][j]);
        }
        fprintf(fsaida, "\n");
    }
    fclose(fsaida);

    // Libera a memoria das matrizes
    for (i = 0; i < linha1; ++i) {
        free(mat1[i]);
        free(mat3[i]);
    }
    for (i = 0; i < linha2; ++i) free(mat2[i]);
    free(mat1);
    free(mat2);
    free(mat3);

    // Encerra o contador para tempo de saida de dados
    cout << "Tempo de Saida de Dados: " << (double)(clock() - tStart) / CLOCKS_PER_SEC << endl;

    getchar();
    return 0;
}
