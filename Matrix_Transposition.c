#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void initializeMatrix(float** matrix, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i][j] = (float)rand() / RAND_MAX * 10.0; 
        }
    }
}

void transposeMatrix(float** matrix, float** transpose, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            transpose[j][i] = matrix[i][j];
        }
    }
}

void printMatrix(float** matrix, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%6.2f ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main() {
    int N;
    printf("Inserisci la dimensione N della matrice (N x N): ");
    scanf("%d", &N);

    // Allocazione dinamica delle matrici
    float** matrix = (float**)malloc(N * sizeof(float*));
    float** transpose = (float**)malloc(N * sizeof(float*));
    for (int i = 0; i < N; i++) {
        matrix[i] = (float*)malloc(N * sizeof(float));
        transpose[i] = (float*)malloc(N * sizeof(float));
    }

    // Inizializzazione della matrice con valori casuali
    srand(time(NULL));
    initializeMatrix(matrix, N);

    printf("Matrice originale:\n");
    printMatrix(matrix, N);

    // Calcolo della trasposta
    transposeMatrix(matrix, transpose, N);

    printf("\nMatrice trasposta:\n");
    printMatrix(transpose, N);

    // Deallocazione della memoria
    for (int i = 0; i < N; i++) {
        free(matrix[i]);
        free(transpose[i]);
    }
    free(matrix);
    free(transpose);

    return 0;
}
