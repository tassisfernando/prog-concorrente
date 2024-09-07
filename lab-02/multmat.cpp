
	#include <iostream>
	#include <pthread.h>
	#include <malloc.h>
	#include <time.h>

	using namespace std;

	//variaveis globais
	float **mat1, **mat2, **mat3; //matrizes de entrada e saida
	int linha1, col1, linha2, col2; //dimensoes das matrizes de entrada

	//calcula uma linha na matriz de saida
	void calculaLinha(int numLinha){
		int i, j, n = (int) numLinha;
		float m;
		for (i = 0; i < col2; i++){
			m = 0;
			for (j = 0; j < col1; j++){
				m += mat1[n][j] * mat2[j][i];
			}
			mat3[n][i] = m;
		}
	}

	//funcao principal
	int main(int argc, char *argv[]) {
		int i, j, k;	//variaveis auxiliares para os loops
		FILE *fentrada1, *fentrada2, *fsaida; //arquivos de entrada e saida

		// abertura dos arquivos de entrada e o de saÃ­da
		fentrada1 = fopen("dados/A2048x2048.txt", "r");
		fentrada2 = fopen("dados/A2048x2048.txt" , "r");
		fsaida = fopen("dados/A2048x2048_result.txt" , "w");
		if (fentrada1 == NULL) perror ("Error opening file fentrada1");
		if (fentrada2 == NULL) perror ("Error opening file fentrada2");
		if (fsaida == NULL) perror ("Error opening file fsaida");

		// variÃ¡veis para calcular o tempo
		clock_t tStart;

		//inicia o contador para o tempo de entrada de dados
		tStart = clock();

		fscanf(fentrada1,"%d %d", &linha1, &col1);
		fscanf(fentrada2,"%d %d", &linha2, &col2);

		//verifica se as matrizes de entrada podem ser multiplicadas
		if(col1 != linha2) {
			puts("NÃºmero de colunas da primeira matriz deve ser igual ao nÃºmero de linhas da segunda matriz.");
			return -1;
		}

		//aloca e le a primeira matriz
		if((mat1 = (float**) malloc(sizeof(float*)*linha1)) == NULL) return -1;
		for(i = 0; i < linha1; ++i) {
			if((mat1[i] = (float*) malloc(sizeof(float)*col1)) == NULL) return -1;
			for(j = 0; j < col1; ++j) {
				fscanf(fentrada1, "%f", &mat1[i][j]);
			}
		}
		fclose(fentrada1);

		//aloca e le a segunda matriz
		if((mat2 = (float**) malloc(sizeof(float*)*linha2)) == NULL) return -1;
		for(i = 0; i < linha2; ++i) {
			if((mat2[i] = (float*) malloc(sizeof(float)*col2)) == NULL) return -1;
			for(j = 0; j < col2; ++j) {
				fscanf(fentrada2, "%f", &mat2[i][j]);
			}
		}
		fclose(fentrada2);

		//aloca a matriz resultado
		if((mat3 = (float**) malloc(sizeof(float*)*linha1)) == NULL) return -1;
		for(i = 0; i < col2; ++i) {
			if((mat3[i] = (float*) malloc(sizeof(float)*col2)) == NULL) return -1;
		}

		//encerra o contador para tempo de entrada de dados
		cout << "Tempo de Entrada de Dados: " << (double) (clock() - tStart) / CLOCKS_PER_SEC << endl;

		//inicia o contador para o tempo de processamento
		tStart = clock();

		//multiplica as matrizes
		for (k=0;k<linha1;k++) {
			calculaLinha(k);
		}

		//encerra o contador para tempo de processamento
		cout << "Tempo de Processamento: " << (double) (clock() - tStart) / CLOCKS_PER_SEC << endl;

		//inicia o contador para o tempo de saida de dados
		tStart = clock();

		//imprime a matriz resultante no arquivo de saida
		for(i=0; i<linha1; i++) {
			for(j=0; j<col2; j++) {
				fprintf(fsaida, "%f ",mat3[i][j]);
			}
			fprintf(fsaida, "\n");
		}
		fclose(fsaida);

		//libera a memoria das matrizes
		for(i = 0; i < linha1; ++i) {
			free(mat1[i]);
			free(mat3[i]);
		}
		for(i = 0; i < linha2; ++i) free(mat2[i]);
		free(mat1);
		free(mat2);
		free(mat3);

		//encerra o contador para tempo de processamento
		cout << "Tempo de Saida de Dados: " << (double) (clock() - tStart) / CLOCKS_PER_SEC << endl;

		getchar();
		return 0;
	}