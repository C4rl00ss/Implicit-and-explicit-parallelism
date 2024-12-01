## IMPLICIT AND EXPLICIT PARALLELISM
### Directory description 
In this repository there is a pbs file "ProgramRun.pbs", the final c code "Best_implicit_and_explicit_combinations.c" which will be compiled and run from the pds file and finally two folders containing codes. The two folders contain all the codes used to collect the data: the IMPLICIT folder contains the codes relating to the implicit parallelization techniques and optimizations for memory access, while the EXPLICIT folder contains the codes relating to the parallel programming techniques through the OpenMP library.
To run the files in these two folders go to the "OTHER FILES" paragraph.

### How to execute the main code with pbs
- __It is recommended to first read the flag session regarding the -DAutomatic flag for setting the matrix size.__   
- The PBS file contains all the commands required to compile and execute the "Best_implicit_and_explicit_combinations.c".  
- All files are written in C, and the compiler used in the tests is the GCC in the version 9.1.0.  

To run the PBS file, you need to:  
1. Copy the "ProgramRun.pbs" file and the "Best_implicit_and_explicit_combinations.c" file to the cluster.  
2. Open the PBS file and modify the line indicating the location of the C file to be compiled (comments in the PBS file guide this operation).  
3. Save the PBS file.
4. Use the "export OMP_NUM_THREADS=N" command into the bash to set the number of threads you want to use to perform the functions implemented with omp parallel
5. Finally, submit the PBS file using the directive `qsub ProgramRun.pbs` in the cluster's bash terminal, and the file will be processed.

### How to execute the main code in a interactive session
- __It is recommended to first read the flag session regarding the -DAutomatic flag for setting the matrix size__       
1. Copy the "Best_implicit_and_explicit_combinations.c" file to the cluster.
2. Open an interactive session with 1 node, 96 cpus , 1 gb of RAM
3. load the module gcc91 for the c compiler GCC 9.1.0
4. Use this to compile the code: "gcc Best_implicit_and_explicit_combinations.c -o best_I_E_compilated -fopenmp -DAutomatic"
5. run the output file "best_I_E_compilated"
6. If you want to manually enter the size of the matrix, remove the flag "-DAutomatic", compile and then execute the output file. The program will ask you the matrix sixe. The only accepted values are numbers ​​of the power of two.

### What resources does the pbs file require:
The pbs file requests a node with 1GB of RAM and 96 available CPUs from the cluster for 2 minutes. The CPUs are needed to test the function that implements the parallel programming method with OpenMP

### Output files
The PBS file "ProgramRun.pbs" will generate three outputs:  
- The compiled code, named "best_I_E_compilated",  
- The error file, named "Error_file.e",  
- And the output file, named "Output_file.o".  

### What's in the output file and how to read it
In the file "Output_file.o", you will find the outputs of multiple functions. These include some of the best-performing implementations based on the tests, as well as the sequential function, which serves as a baseline for comparing speed-up and efficiency.  

The codes execute the matrix transposition using, by default, a square matrix of size **N x N**, where **N = 8192**, __thanks to the `-DAutomatic` flag__ (see the "Flags" section for a detailed explanation of the flags used).  

The generated output will contain the execution times for the following functions:  
- The sequential function, __matTranspose__,  
- The SIMD-optimized function, __matTransposeSIMD__ (implemented with OpenMP),  
- The prefetching-optimized function, __matTransposePrefetching__,  
- The function __matTransposeRestrict_Unroll__ (implemented using the restrict keyword and loop unrolling),  
- And finally, the function __matTransposeOMP_COLLAPSE__ (implemented with #pragma omp parallel and #pragma omp for collapse(2)).  



### Flags 
The only two required flags are -fopenmp and -DAutomatic:
- ##### fopenm
This flag enables the OpenMP library, which is used in the project for three key purposes:
1. Measuring execution time with the omp_get_wtime function.
2. Using directives like #pragma omp simd to optimize the code through vectorization.
3. Enabling parallel programming with directives such as #pragma omp parallel.

- ##### DAutomatic:
This flag eliminates the need to manually specify the matrix size at runtime. When the flag is used, the program automatically sets the matrix size to 8192. This is defined in the header of the C file "Best_implicit_and_explicit_combinations.c" by the line *#define AUTOMATIC_MATRIX_SIZE 8192*, which assigns the value 8192 to the matrix size **N**. This flag can be removed during compilation, but __NOT IN THE PBS FILE__, as the code will then prompt the user for the matrix size, which cannot be provided in a PBS batch script, leading to execution failure.   
__IF YOU WANT TO REMOVE THE -DAutomatic FLAG IN ORDER TO SET THE MATRIX SIZE  (ONLY VALUES THAT ARE POWERS OF 2), YOU MUST OPEN AN INTERACTIVE SESSION.__   

- Alternatively, to change the matrix size, you can edit the C code "Best_implicit_and_explicit_combinations.c" and replace the line *#define AUTOMATIC_MATRIX_SIZE 8192* with a desired value, while keeping the -DAutomatic flag so the pbs file with a different matrix size can be used.



### OTHER FILES
The other files contain all the implementations used in tests during the project. The structure of each code is almost identical (see paragraph *General structure of the codes*) therefore the substantial differences from one code to another are the implementations of the functions. If you want to execute these files, just repeat the procedure described in the paragraph "__How to execute the main code with pbs__" as in each folder there is a pbs file that executes ALL the codes present in the folder, printing the results in a single otput file generated by each code. __In the pbs file present in the EXPLICIT folder you must also change the "export OMP_NUM_THREADS=N" line which indicates the number of threads you want to use for the execution of the "OMP_WITH_IMPLICIT_Matrix_Transposition.c" file__




### General structure of the codes:
1. ##### Function declaration
   The codes are structured in a very similar way to each other: at the beginning of the file there are the functions that perform the transpose of the matrix and they are all derived from the modified sequential __matTranspose__ function. Each new function thus created has been renamed so as to include the method with which it was implemented in the name (e.g. matTranspose_SIMD implements the SIMD method internally). Immediately after the transpose functions comes the implementation of the __checkSym__ functions for checking the symmetry of the matrix, __initializeMatrix__ for the initialization with random float values ​​from 0 to 10.0 of the matrix and finally __printMatrix__ which prints the matrix that is passed to it as input.

2. ##### Main
The main starts with a piece of conditional code that checks whether the **Automatic** flag is present (which must be added at compile time via the **-DAutomatic** flag). If the flag is not present the code asks the user to enter the size of the Matrix which must be a number of the order of the power of 2 and if this condition is not satisfied the code asks the question again until an input is inserted valid. If the flag is present, the code skips the manual input process by the user, and therefore also its verification, and assigns to the variable **N** the value defined in AUTOMATIC_MATRIX_SIZE which by default corresponds to 2^13= 8192 on all codes.
Immediately after comes the allocation of the matrix memory in the heap, its random initialization, the definition of the variables that will be used to calculate the time and finally the verification of the symmetry of the matrix.
This is followed by calls to all the implemented functions, for each one the execution time is measured and this value is printed on the screen.
Finally, the last part of the code deallocates the matrix memory.











