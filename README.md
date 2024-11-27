## IMPLICIT AND EXPLICIT PARALLELISM
#### Directory description 
In questa repository sono presenti due cartelle contenenti dei codici, un file pbs "ProgramRun.pbs" e il codice finale in c "Best_implicit_and_explicit_combinations.c" che verrà compilato ed eseguito dal file pds.
Le due cartelle contengono tutti i codici utilizzati per raccogliere i dati: la cartella IMPLICIT contiene i codici relativi alle tecniche di parallelizzazione implicita e di ottimizzazioni per l'accesso in memoria, mentre la cartella EXPLICIT contiene i codici relativi alle tecniche di programmazione parallela tramite la libreria OpenMP. 

#### Code execution
The PBS file contains all the commands required to compile and execute the "Best_implicit_and_explicit_combinations.c" file located in the same directory as the PBS file.  
All files are written in C, and the compiler used in the tests is GCC version 9.1.0.  
To run the PBS file, you need to:  
1. Copy the "ProgramRun.pbs" file and the "Best_implicit_and_explicit_combinations.c" file to the cluster.  
2. Open the PBS file and modify the line indicating the location of the C file to be compiled (comments in the PBS file guide this operation).  
3. Save the PBS file.  
4. Finally, submit the PBS file using the directive `qsub ProgramRun.pbs` in the cluster's bash terminal, and the file will be processed.  


#### What resources does the pbs file require:
Il file pbs richiede al cluster per 2 minuti di tempo un nodo con 1gb di ram e 96 cpu disponibili. Le cpu sono necessarie per testare la funzione che implementa il metodo di programmazione parallela con Openmp

#### Output files
Il file pbs "ProgramRun.pbs" genererà tre output, il codice compilato col nome "best_I_E_compilated", il file errore di nome "Error_file.e" e infine il file con gli output nominato "Output_file.o". 

#### what's in the output file and how to read it
Nel file "Output_file.o" sono presenti gli output di piu funzioni, esse sono le funzioni con le implementazioni che meglio hanno performato durante i test, ovviamente è presente anche la funzione sequenziale che serve da comparatore per lo speed-up e l'efficienza. Tutti i risultati sono stati eseguiti su una matrice quadrata N*N con N pari a 8192. La fu  






## Implicit-and-explicit-parallelism


Each file contains a different implementation. I decided to separate them into multiple files to make the code more readable and organized.

1. #### SEQUENTIAL CODE FILES:
   - **S_IMPLICIT_Matrix_Transposition.c**
   - **S_PREFETCHING_Matrix_Transposition.c**
   - **S_RESTRICT_Matrix_Transposition.c**

2. #### PARALLEL CODE (OMP) FILES:
che


### Struttura dei codici sequenziali:
1. ##### Dichiarazione funzioni
   I codici sequenziali sono strutturati in modo molto simile tra loro: all'inizio del file sono presenti le implemetazioni delle funzioni __matTranspose__, ognuna di esse ha una implementazione diversa che viene indicata nel nome stesso della funuzione per distinguerle(es. matTranspose_SIMD implementa il metodo SIMD al suo interno). Subito dopo le funzioni __matTranspose__ viene l'implementazione delle funzioni __checkSym__ per il controllo della simmetria della matrice, __initializeMatrix__ per l'inizializzazione con valori float random da 0 a 10.0 della matrice ed infine __printMatrix__ che stamba la matrice che gli viene passata in input.

2. ##### Main
   Il main inzia con un pezzo di codice condizionale che controlla se è presente la flag **Automatic** (che va aggiunta in fase di compilazione tramite la flag **-DAutomatic**). In caso la flag non fosse presente  il codice richiede all'utente di inserire la grandezza della Matrice che dovrà essere un numero dell'ordine della potenza di 2 e in caso questa condizione non fosse soddisfatta il codice ripropone la domanda finche non si inserisce un input valido. In caso la flag fosse presente il codice salta il processo di input manuale da parte dell'utente e quindi anche la sua verifica e assegna alla variabile **N** il valore definito in AUTOMATIC_MATRIX_SIZE che di default corrisponde a 2^14=16384 su tutti i codici.
Subito dopo viene l'allocazione della memoria della matrice, la sua inizializzazone random, la definizione delle varibili che verranno usate per calcolare il tempo e infine la verifica della simmetria della matrice.
Seguono le chiamate a tutte le funzioni implementate, per ognuna viene misurato il tempo di esecuzione e stampato tale valore a schermo.
Infine l'ultima parte del codice dealloca la memoria delle matrici.
4. 

5. 










