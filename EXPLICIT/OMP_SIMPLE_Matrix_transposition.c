#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <omp.h>


#define AUTOMATIC_MATRIX_SIZE 8192
#define MAX_NUM_THREAD 98   //select max number of threads


//TRANSPOSE FUNCTIONS
/////////////////////////////////////////////////////////////////////////////////////////



// SEQUENTIAL 
void matTranspose(float** matrix, float** transpose, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            transpose[j][i] = matrix[i][j];
        }
    }
}

// PARALLEL WITH OMP FOR
void matTransposeOMP(float** matrix, float** transpose, int N) {
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            transpose[j][i] = matrix[i][j];
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////




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




//EXECUTION OF FUNCTIONS
///////////////////////////////////////////////////////////////////////////////////////////
    
    printf("Matrix size : %d \n ", N);
    
    
    
    // SEQUENTIAL
    t1 = omp_get_wtime();
    matTranspose(matrix, transpose, N);
    t2 = omp_get_wtime();
    double sequentialTime= t2-t1;
    printf("--------------------------------------------------------------------------------------\n");
    printf("SEQUENTIAL: %f secondi\n", t2 - t1);


    printf("--------------------------------------------------------------------------------------\n");
    printf("  NUMBER OF THREADS  |  ELAPSED TIME (seconds) |     SPEEDUP   |     EFFICIENCY   \n");



    //PARALLEL WITH OMP FOR
    for(int g=1; g<MAX_NUM_THREAD; g+=4){   //loop that increases threads by 4 at each iteration
    omp_set_num_threads(g);      //set number of threads
    t1 = omp_get_wtime();
    matTransposeOMP(matrix, transpose, N);
    t2 = omp_get_wtime();
    float speedUp=sequentialTime/(t2-t1);
    printf("         %2d          |         %f        |    %f   |     %f     \n", g , t2 - t1, speedUp, (speedUp/g)*100);
    }
    
    
    printf("--------------------------------------------------------------------------------------\n");
    
///////////////////////////////////////////////////////////////////////////////////////////    
    
    
    
    

    // Memory deallocation
    for (int i = 0; i < N; i++) {
        free(matrix[i]);
        free(transpose[i]);
    }
    free(matrix);
    free(transpose);

    return 0;
}
