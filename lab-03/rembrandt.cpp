#include <iostream>
#include <windows.h>
#include <math.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

#define SIZE  500

int main(int argc, char *argv[]) {
    // Iniciando o contador de tempo
    auto start = high_resolution_clock::now();

    HWND myconsole = CreateWindowA("STATIC", "Rembrandt", WS_VISIBLE, 0, 0, SIZE + 15, SIZE + 35, NULL, NULL, NULL, NULL);
    HDC mydc = GetDC(myconsole);

    int r, g, b;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            r = (int)sqrt(pow(i - 0, 2) + pow(j - 0, 2));
            g = (int)sqrt(pow(i - 0, 2) + pow(j - SIZE, 2));
            b = (int)sqrt(pow(i - SIZE, 2) + pow(j - SIZE, 2));
            SetPixel(mydc, i, j, RGB(r, g, b));
        }
    }

    ReleaseDC(myconsole, mydc);

    // Finalizando o contador de tempo
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    cout << "Tempo de execução: " << duration.count() << " milissegundos" << endl;

    getchar();
    return 1;
}
