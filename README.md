## IMPLICIT AND EXPLICIT PARALLELISM
### Directory description 
In this repository there is a pbs file "ProgramRun.pbs", the final c code "Best_implicit_and_explicit_combinations.c" which will be compiled and run from the pds file and two folders containing codes. The two folders contain all the codes used to collect the data: the IMPLICIT folder contains the codes relating to the implicit parallelization techniques and optimizations for memory access, while the EXPLICIT folder contains the codes relating to the parallel programming techniques through the OpenMP library.

### How to execute the main code with pbs
- __It is recommended to first read the flag session regarding the -DAutomatic flag for setting the matrix size.__   
- The PBS file contains all the commands required to compile and execute the "Best_implicit_and_explicit_combinations.c".  
- All files are written in C, and the compiler used in the tests is the GCC in the version 9.1.0.  

To run the PBS file, you need to:  
1. Copy the "ProgramRun.pbs" file and the "Best_implicit_and_explicit_combinations.c" file to the cluster.  
2. Open the PBS file and modify the line indicating the location of the C file to be compiled (comments in the PBS file guide this operation).  
3. Save the PBS file.  
4. Finally, submit the PBS file using the directive `qsub ProgramRun.pbs` in the cluster's bash terminal, and the file will be processed.

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

### what's in the output file and how to read it
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
Gli altri file contengono tutte le implementazioni fatte e testate durante lo svolgimento del progetto. La struttura di ogni codice è pressoche identica (guardare paragrafo *General structure of the codes*) quindi le differenze sostanziali da un codice all'altro sono le implementazioni delle funzioni. In caso si voglia eseguire uno di questi file 







### General structure of the codes:
1. ##### Dichiarazione funzioni
   I codici sequenziali sono strutturati in modo molto simile tra loro: all'inizio del file sono presenti le implemetazioni delle funzioni __matTranspose__, ognuna di esse ha una implementazione diversa che viene indicata nel nome stesso della funuzione per distinguerle(es. matTranspose_SIMD implementa il metodo SIMD al suo interno). Subito dopo le funzioni __matTranspose__ viene l'implementazione delle funzioni __checkSym__ per il controllo della simmetria della matrice, __initializeMatrix__ per l'inizializzazione con valori float random da 0 a 10.0 della matrice ed infine __printMatrix__ che stamba la matrice che gli viene passata in input.

2. ##### Main
   Il main inzia con un pezzo di codice condizionale che controlla se è presente la flag **Automatic** (che va aggiunta in fase di compilazione tramite la flag **-DAutomatic**). In caso la flag non fosse presente  il codice richiede all'utente di inserire la grandezza della Matrice che dovrà essere un numero dell'ordine della potenza di 2 e in caso questa condizione non fosse soddisfatta il codice ripropone la domanda finche non si inserisce un input valido. In caso la flag fosse presente il codice salta il processo di input manuale da parte dell'utente e quindi anche la sua verifica e assegna alla variabile **N** il valore definito in AUTOMATIC_MATRIX_SIZE che di default corrisponde a 2^14=16384 su tutti i codici.
Subito dopo viene l'allocazione della memoria della matrice, la sua inizializzazone random, la definizione delle varibili che verranno usate per calcolare il tempo e infine la verifica della simmetria della matrice.
Seguono le chiamate a tutte le funzioni implementate, per ognuna viene misurato il tempo di esecuzione e stampato tale valore a schermo.
Infine l'ultima parte del codice dealloca la memoria delle matrici.











