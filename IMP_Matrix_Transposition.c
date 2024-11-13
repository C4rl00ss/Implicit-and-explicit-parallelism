#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <omp.h>
#define N_ESECUZIONI 3

//16.384    8.192


// Funzione di controllo simmetria
void checkSym(float** matrix, int N){
    for(int i =0; i<N; i++){
        for (int j=0; j<N; j++){
            if (matrix[i][j] != matrix[j][i]){
                printf("La matrice non è simmetrica.\n");
                return;
            }
        }
    }
    printf("La matrice è simmetrica.\n");
}

// Funzione di trasposizione sequenziale
void matTranspose(float** matrix, float** transpose, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            transpose[j][i] = matrix[i][j];
        }
    }
}


// Funzione di trasposizione implicita
void matTransposeImp(float** matrix, float** transpose, int N) {
    for (int i = 0; i < N; i++) {
        #pragma simd
        for (int j = 0; j < N; j++) {
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
    int N;
    bool inputVerification = true;
    double t1, t2;
    
    //input verification
    while (inputVerification){
        printf("Inserisci la dimensione N della matrice (N x N): ");
        scanf("%d", &N);
        if((N > 0) && ((N & (N - 1)) == 0)){ // verifica se N è una potenza di 2
            inputVerification = false;
        } else {
            printf("Il numero inserito non è una potenza di 2.\n");
        }
    }
   
   
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
    
    //symmetry controll
    checkSym(matrix, N);
    
    
    
    
    
    
    
    
    
    for (int j=0; j<N_ESECUZIONI; j++){
    // Calcolo della trasposta in sequenziale
    t1 = omp_get_wtime();
    matTranspose(matrix, transpose, N);
    t2 = omp_get_wtime();
    printf("SERIALE: %f secondi\n", t2 - t1);
    
    //calcolo transposta implicta
    t1 = omp_get_wtime();
    matTransposeImp(matrix, transpose, N);
    t2 = omp_get_wtime();
    printf("SIMD:    %f secondi\n", t2 - t1);
    }
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
  //IN CASO SI VUOLE STAMPARE LA MATRICE TRANSPOSTA TOGLIERE I COMMENTI
  //printf("Matrice originale:\n");
    //printMatrix(matrix, N);
    //print("\n");
    //printf("\nMatrice trasposta:\n");
    //printMatrix(transpose, N);



    

    
    // Deallocazione della memoria
    for (int i = 0; i < N; i++) {
        free(matrix[i]);
        free(transpose[i]);
    }
    free(matrix);
    free(transpose);

    return 0;
}
