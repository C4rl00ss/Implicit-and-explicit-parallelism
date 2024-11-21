## Implicit-and-explicit-parallelism
This repository contains NNN files: 3 for the sequential code, identified by the letter **S** at the beginning of the file name, and NNN for the parallel code, identified by the letter **P** at the beginning of the file name.

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
4. 

5. 










