#include <iostream>
#include <windows.h>
#include <math.h>
#include <pthread.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

#define SIZE  500
#define NUM_THREADS 200 // Definindo o número de threads

// Estrutura para passar argumentos para a função da thread
typedef struct {
    HWND* myconsole;
    int idThread;
    int nThreads;
    int size;
} t_Args;

// Função para calcular a cor dos pixels na parte designada e desenhá-los
void* calculateAndDrawPixels(void* arguments) {
    t_Args* args = (t_Args*)arguments;
    HDC mydc = GetDC(*args->myconsole);

    int r, g, b;
    for (int i = args->idThread; i < args->size; i += args->nThreads) {
        for (int j = 0; j < SIZE; j++) {
            r = (int)sqrt(pow(i - 0, 2) + pow(j - 0, 2));
            g = (int)sqrt(pow(i - 0, 2) + pow(j - SIZE, 2));
            b = (int)sqrt(pow(i - SIZE, 2) + pow(j - SIZE, 2));

            SetPixel(mydc, i, j, RGB(r, g, b));
        }
    }

    ReleaseDC(*args->myconsole, mydc);
    pthread_exit(NULL);
    return 0;
}

int main(int argc, char *argv[]) {
    // Iniciando o contador de tempo
    auto start = high_resolution_clock::now();

    HWND myconsole = CreateWindowA("STATIC", "Rembrandt", WS_VISIBLE, 0, 0, SIZE + 15, SIZE + 35, NULL, NULL, NULL, NULL);

    // Inicializando threads
    pthread_t threads[NUM_THREADS];
    t_Args threadArgs[NUM_THREADS];

    // Inicializando argumentos das threads
    for (int i = 0; i < NUM_THREADS; i++) {
        threadArgs[i].myconsole = &myconsole;
        threadArgs[i].idThread = i;
        threadArgs[i].nThreads = NUM_THREADS;
        threadArgs[i].size = SIZE;
        
        // Criando threads
        pthread_create(&threads[i], NULL, calculateAndDrawPixels, (void*)&threadArgs[i]);
    }

    // Aguardando a conclusão das threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Finalizando o contador de tempo
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    cout << "Tempo de execução: " << duration.count() << " milissegundos" << endl;


    getchar();
    return 0;
}
