#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <omp.h>
#include <xmmintrin.h> // Includes for SIMD instructions like _mm_prefetch
#define M 16384

//16384    8192



//TRANSPOSE FUNCTIONS
/////////////////////////////////////////////////////////////////////////////////////////


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

//PREFETCHING UNROLL
void matTransposePrefetchUnroll(float** matrix, float** transpose, int N) {
    int blockSize = 4; // Utilizzo blocchi di dimensione 4 per adattare il caricamento SIMD
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j += blockSize) {
            // Prefetch: carica preventivamente righe successive della matrice nella cache
            if (j + blockSize * 2 < N) {
                _mm_prefetch((const char*)&matrix[i][j + blockSize * 2], _MM_HINT_T0);
            }
            
            #pragma unroll(blockSize) // Srotola il ciclo interno
            for (int k = 0; k < blockSize && (j + k) < N; k++) {
                transpose[j + k][i] = matrix[i][j + k];
            }
        }
    }
}


///////////////////////////////////////////////////////////////////////////////////////





//OTHER FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////////


// Symmetry control function
void checkSym(float** matrix, int N){
    for(int i =0; i<N; i++){
        for (int j=0; j<N; j++){
            if (matrix[i][j] != matrix[j][i]){
                printf("\nThe matrix is not symmetric.\n");
                return;
            }
        }
    }
    printf("The matrix is symmetric.\n");
}

// matrix initialization function with random float values
void initializeMatrix(float** matrix, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i][j] = (float)rand() / RAND_MAX * 10.0; //from 0 to 10.0
        }
    }
}

// print matrix function
void printMatrix(float** matrix, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%6.2f ", matrix[i][j]);
        }
        printf("\n");
    }
}


////////////////////////////////////////////////////////////////////////////////////////



int main() {

//conditional code for manual insertion of the size of the matrices or automatic insertion
//-----------------------------------------------------------------------------------------
#ifdef Automatico
    int N = M; //automatic insertion
#else
    int N;
    bool inputVerification = true;
    // input verification
    while (inputVerification) {
        printf("Enter the size of the matrix (N x N): ");
        scanf("%d", &N);
        if ((N > 0) && ((N & (N - 1)) == 0)) { 
            inputVerification = false;
        } else {
            printf("The number entered is not a power of 2.\n");
        }
    }
#endif
//-----------------------------------------------------------------------------------------



    // Dynamic matrix allocation
    float** matrix = (float**)malloc(N * sizeof(float*));
    float** transpose = (float**)malloc(N * sizeof(float*));
    for (int i = 0; i < N; i++) {
        matrix[i] = (float*)malloc(N * sizeof(float));
        transpose[i] = (float*)malloc(N * sizeof(float));
    }
    
    //time variables initialization
    double t1, t2;

    // Initialize the array with random values
    srand(time(NULL));
    initializeMatrix(matrix, N);
    
    
    //symmetry controll call function
    checkSym(matrix, N);


    
//EXECUTION OF FUNCTIONS
///////////////////////////////////////////////////////////////////////////////////////////


    printf("//////////////////////////////////////////////////////////\n");
    
    //PREPARATORY TEST
    //used because the first run function is always the slowest due to the load of cache
    t1 = omp_get_wtime();
    matTranspose(matrix, transpose, N);
    t2 = omp_get_wtime();
    printf("FIRST TEST, DO NOT CONSIDER THIS TIME:    %f secondi\n\n", t2 - t1);

    // SEQUENZIALE
    t1 = omp_get_wtime();
    matTranspose(matrix, transpose, N);
    t2 = omp_get_wtime();
    double sequentialTime= t2-t1;
    printf("SEQUENZIALE: %f secondi\n", sequentialTime);
    
    
    // PREFETCHING
    t1 = omp_get_wtime();
    matTransposePrefetching(matrix, transpose, N);
    t2 = omp_get_wtime();
    printf("PREFETCHING: %f secondi ---->", t2 - t1);
    printf("speed up : %f \n", sequentialTime/(t2-t1));
    
    // PREFETCHING UNROLL
    t1 = omp_get_wtime();
    matTransposePrefetchUnroll(matrix, transpose, N);
    t2 = omp_get_wtime();
    printf("PREF-AGGRES: %f secondi ---->", t2 - t1);
    printf("speed up : %f \n", sequentialTime/(t2-t1));
    
    
    
    printf("//////////////////////////////////////////////////////////\n");
    
    

//////////////////////////////////////////////////////////////////////////////////////////
    
    
    
    
    //IF YOU WANT TO PRINT THE TRANSPOSED MATRIX, REMOVE THE COMMENTS
    /*
    printf("Matrice originale:\n");
    printMatrix(matrix, N);
    printf("\n");
    printf("\nMatrice trasposta:\n");
    printMatrix(transpose, N);
    */


    // Deallocazione della memoria
    for (int i = 0; i < N; i++) {
        free(matrix[i]);
        free(transpose[i]);
    }
    free(matrix);
    free(transpose);

    return 0;
}
