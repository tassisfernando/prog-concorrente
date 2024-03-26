#include <iostream>
#include <windows.h>
#include <math.h>
#include <pthread.h>
#include <vector>
#include <chrono>

using namespace std;
using namespace std::chrono;

#define SIZE  500
#define NUM_THREADS 3 // Definindo o número de threads

// Estrutura para armazenar os valores dos pixels
struct PixelData {
    int r;
    int g;
    int b;
};

// Função para calcular os valores dos pixels na parte designada
void* calculatePixels(void* arguments) {
    vector<vector<PixelData>>* pixelData = (vector<vector<PixelData>>*)arguments;

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            int r = (int)sqrt(pow(i - 0, 2) + pow(j - 0, 2));
            int g = (int)sqrt(pow(i - 0, 2) + pow(j - SIZE, 2));
            int b = (int)sqrt(pow(i - SIZE, 2) + pow(j - SIZE, 2));

            (*pixelData)[i][j] = {r, g, b};
        }
    }

    pthread_exit(NULL);
    return 0;
}

// Função para desenhar a imagem na janela
void drawImage(HWND myconsole, vector<vector<PixelData>>& pixelData) {
    HDC mydc = GetDC(myconsole);

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            PixelData pixel = pixelData[i][j];
            SetPixel(mydc, i, j, RGB(pixel.r, pixel.g, pixel.b));
        }
    }

    ReleaseDC(myconsole, mydc);
}

int main(int argc, char *argv[]) {
    // Iniciando o contador de tempo
    auto start = high_resolution_clock::now();

    HWND myconsole = CreateWindowA("STATIC", "Rembrandt", WS_VISIBLE, 0, 0, SIZE + 15, SIZE + 35, NULL, NULL, NULL, NULL);

    // Inicializando dados dos pixels
    vector<vector<PixelData>> pixelData(SIZE, vector<PixelData>(SIZE));

    // Inicializando threads
    pthread_t threads[NUM_THREADS];

    // Criando threads para calcular os valores dos pixels
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, calculatePixels, (void*)&pixelData);
    }

    // Aguardando a conclusão das threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Desenhando a imagem na janela
    drawImage(myconsole, pixelData);

     // Finalizando o contador de tempo
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    cout << "Tempo de execução: " << duration.count() << " milissegundos" << endl;

    getchar();
    return 0;
}
