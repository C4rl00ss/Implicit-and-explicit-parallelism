#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>


//SYMMETRIC CONTROL FUNCTION 
void checkSym(float** matrix, int N){
    for(int i =0; i<N; i++){
        for (int j=0; j<N; j++){
            if (matrix[i][j]!=matrix[j][i]){
               printf("the matrix is not symmetric \n");
               return;
            }
        }
    }
    printf("the matrix is symmetric \n");
}
//TRANSPOSE MATRIX FUNCTION
void matTranspose(float** matrix, float** transpose, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            transpose[j][i] = matrix[i][j];
        }
    }
}





void initializeMatrix(float** matrix, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i][j] = (float)rand() / RAND_MAX * 10.0; 
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
    bool inputVerification=true;
    
    clock_t t1, t2;
    
    
    
    
    while (inputVerification){
      printf("Inserisci la dimensione N della matrice (N x N): ");
      scanf("%d", &N);
      if((N > 0) && ((N & (N - 1)) == 0)){ //verify the power of 2
        inputVerification=false;
      }else {
        printf("The input number is not a power of 2 \n");
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

    //printf("Matrice originale:\n");
    //printMatrix(matrix, N);
    
    checkSym(matrix, N);
    
    

    // Calcolo della trasposta
    
    t1=clock();
    matTranspose(matrix, transpose, N);
    t2=clock();
    
    printf("tempo calcolo trasposta: %f \n", (double)(t2-t1)/CLOCKS_PER_SEC);
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









