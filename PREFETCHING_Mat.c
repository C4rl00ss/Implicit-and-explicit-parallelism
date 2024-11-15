#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <omp.h>
#include <xmmintrin.h> // Include per le istruzioni SIMD come _mm_prefetch
#define M 15000


// Funzione di controllo simmetria
void checkSym(float** matrix, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (matrix[i][j] != matrix[j][i]) {
                printf("La matrice non è simmetrica.\n");
                return;
            }
        }
    }
    printf("La matrice è simmetrica.\n");
}


// SEQUENZIALE
void matTranspose(float** matrix, float** transpose, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            transpose[j][i] = matrix[i][j];
        }
    }
}

// PREFETCHING
void matTransposePrefetching(float** matrix, float** transpose, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            // Prefetch: carica preventivamente il dato di matrix[i][j] nella cache
            if (j + 4 < N) {
                  _mm_prefetch((const char*)&matrix[i][j + 4], _MM_HINT_T0);
              }
            transpose[j][i] = matrix[i][j];
        }
    }
}






// Inizializzazione della matrice con valori casuali
void initializeMatrix(float** matrix, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i][j] = (float)rand() / RAND_MAX * 10.0; 
        }
    }
}

// Stampa della matrice
void printMatrix(float** matrix, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%6.2f ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main() {
    int N=M;
    bool inputVerification = true;
    double t1, t2;

/*
    // Input verification
    while (inputVerification) {
        printf("Inserisci la dimensione N della matrice (N x N): ");
        scanf("%d", &N);
        if ((N > 0) && ((N & (N - 1)) == 0)) { // verifica se N è una potenza di 2
            inputVerification = false;
        } else {
            printf("Il numero inserito non è una potenza di 2.\n");
        }
    }
*/


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

    // Symmetry control
    checkSym(matrix, N);

    // SEQUENZIALE
    t1 = omp_get_wtime();
    matTranspose(matrix, transpose, N);
    t2 = omp_get_wtime();
    // Risultati
    double sequentialTime= t2-t1;
    printf("SEQUENZIALE: %f secondi\n", sequentialTime);
    
    
    // PREFETCHING
    t1 = omp_get_wtime();
    matTransposePrefetching(matrix, transpose, N);
    t2 = omp_get_wtime();
    // Risultati
    printf("PREFETCHING: %f secondi\n", t2 - t1);
    printf("speed up : %f \n", sequentialTime/(t2-t1));
    

    // Deallocazione della memoria
    for (int i = 0; i < N; i++) {
        free(matrix[i]);
        free(transpose[i]);
    }
    free(matrix);
    free(transpose);

    return 0;
}

