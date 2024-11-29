#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <omp.h>
#include <xmmintrin.h> // Includes for SIMD instructions like _mm_prefetch

#define AUTOMATIC_MATRIX_SIZE 8192
#define MAX_NUM_THREAD 98   //select max number of threads


//TRANSPOSE FUNCTIONS
/////////////////////////////////////////////////////////////////////////////////////////////////////


// SEQUENTIAL
void matTranspose(float** matrix, float** transpose, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            transpose[j][i] = matrix[i][j];
        }
    }
}



//                                           IMPLICIT PARALLELISM FUNCTIONS
//-----------------------------------------------------------------------------------------------------

// SIMD
void matTransposeSIMD(float** matrix, float** transpose, int N) {
    for (int i = 0; i < N; i++) {
        #pragma simd
        for (int j = 0; j < N; j++) {
            transpose[j][i] = matrix[i][j];
        }
    }
}

// PREFETCHING
void matTransposePrefetching(float** matrix, float** transpose, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            ////loads the data into memory in advance
            if (j + 4 < N) {
                  _mm_prefetch((const char*)&matrix[i][j + 4], _MM_HINT_T0);
              }
            transpose[j][i] = matrix[i][j];
        }
    }
}

// UNROLL RESTRICT
void matTransposeRestrict_Unroll(float* restrict * restrict matrix, float* restrict * restrict transpose, int N) {
    for (int i = 0; i < N; i++) {
        #pragma GCC unroll(8)
        for (int j = 0; j < N; j++) {
            transpose[j][i] = matrix[i][j];
        }
    }
}




//                                           EXPLICIT PARALLELISM FUNCTIONS
//------------------------------------------------------------------------------------------------------------------

// COLLAPSE
void matTransposeOMP_COLLAPSE(float** matrix, float** transpose, int N) {
    #pragma omp parallel
    {
    #pragma omp for collapse(2)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            transpose[j][i] = matrix[i][j];
        }
    }
    }
}


/////////////////////////////////////////////////////////////////////////////////////////////////////






//OTHER FUNCTIONS
/////////////////////////////////////////////////////////////////////////////////////////////////////

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

/////////////////////////////////////////////////////////////////////////////////////////////////////






int main() {
    

//conditional code for manual insertion of the size of the matrices or automatic insertion
//--------------------------------------------------------------------------------------------------
#ifdef Automatic
    int N = AUTOMATIC_MATRIX_SIZE; //automatic insertion
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
    




//TRANSPOSE EXECUTION FUNCTIONS
///////////////////////////////////////////////////////////////////////////////////////////
    
    printf("//////////////////////////////////////////////////////////\n");
    
  /*  
                    //PREPARATORY TEST
//---------------------------------------------------------------------------------------------------------------
    //used because the first run function is always the slowest due to the load of cache
    t1 = omp_get_wtime();
    matTranspose(matrix, transpose, N);
    t2 = omp_get_wtime();
    printf("FIRST TEST, DO NOT CONSIDER THIS TIME:    %f secondi\n\n", t2 - t1);
*/
    
    
    
    
                    //IMPLICIT PARALLELISM
//---------------------------------------------------------------------------------------------------------------
    //SEQUENTIAL
    t1 = omp_get_wtime();
    matTranspose(matrix, transpose, N);
    t2 = omp_get_wtime();
    double sequentialTime= t2-t1;
    printf("SEQUENTIAL:                    %f secondi\n \n", sequentialTime);
    printf("IMPLICIT PARALLELISM AND MEMORY ACCESS OPTIMIZATION: \n");
    printf("------------------------------------------------------------ \n");
    
    //SIMD
    t1 = omp_get_wtime();
    matTransposeSIMD(matrix, transpose, N);
    t2 = omp_get_wtime();
    printf("IMPLICIT P. WITH SIMD:         %f seconds  -----> ", t2 - t1);
    printf("speed up : %f \n", sequentialTime/(t2-t1));
    
    // PREFETCHING
    t1 = omp_get_wtime();
    matTransposePrefetching(matrix, transpose, N);
    t2 = omp_get_wtime();
    printf("PREFETCHING:                   %f seconds  -----> ", t2 - t1);
    printf("speed up : %f \n", sequentialTime/(t2-t1));
    
     //UNROLL
    t1 = omp_get_wtime();
    matTransposeRestrict_Unroll(matrix, transpose, N);
    t2 = omp_get_wtime();
    printf("RESTRICT WITH UNROLL:          %f seconds  -----> ", t2 - t1);
    printf("speed up : %f \n \n", sequentialTime/(t2-t1));
    
    
    
    
    
                    //EXPLICIT PARALLELISM
//---------------------------------------------------------------------------------------------------------------

    printf("EXPLICIT PARALLELISM WITH PRAGMA OMP FOR COLLAPSE(2) \n");
    printf("--------------------------------------------------------------------------------------\n");
    printf("  NUMBER OF THREADS  |  ELAPSED TIME (seconds) |     SPEEDUP   |     EFFICIENCY   \n");



    //PARALLEL WITH OMP FOR
    for(int g=1; g<MAX_NUM_THREAD; g+=4){   //loop that increases threads by 4 at each iteration
    omp_set_num_threads(g);      //set number of threads
    t1 = omp_get_wtime();
    matTransposeOMP_COLLAPSE(matrix, transpose, N);
    t2 = omp_get_wtime();
    float speedUp=sequentialTime/(t2-t1);
    printf("         %2d          |         %f        |    %f   |     %f     \n", g , t2 - t1, speedUp, (speedUp/g)*100);
    }
    
    
    printf("--------------------------------------------------------------------------------------\n");





    
//////////////////////////////////////////////////////////////////////////////////////////





    // Deallocazione della memoria
    for (int i = 0; i < N; i++) {
        free(matrix[i]);
        free(transpose[i]);
    }
    free(matrix);
    free(transpose);

    return 0;
}




