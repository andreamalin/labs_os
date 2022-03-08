#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <omp.h>
#include <sys/syscall.h>

int sudoku[9][9];

// Columna
void *checkColumn(void *threadNumber) {
    int cells[9];
	unsigned int tid = 0;

	#pragma omp parallel for ordered private(tid)
    for (int i = 0; i < 9; i++)
    {
        tid = omp_get_thread_num();
        printf("Programa en paralelo: thread: %d %d\n", tid, syscall(SYS_gettid));

        // Se obtiene una columna
        #pragma omp ordered
        for (int j = 0; j < 9; j++)
        {
            cells[j] = sudoku[j][i];
        }
    
        int checkNumbers[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        
        // Por celda en la columna
        #pragma omp ordered
        for (int cell = 0; cell < 9; cell++)
        {
            // Por cada numero en los numeros
            for (int i = 0; i < 9; i++)
            {
                // Si el numero es igual al de la celda
                if (checkNumbers[i] == cells[cell]) {
                    // Colorcarlo en -1 (ya usado)
                    checkNumbers[i] = -1;
                    break;
                }
            }
        }

        // Por celda en la columna
        #pragma omp ordered
        for (int cell = 0; cell < 9; cell++)
        {
            if (checkNumbers[cell] != -1) {
                printf("\n------\nAlguna columna NO contiene todos los numeros 1-9\n------\n");
                // La columna NO contiene todos los numeros del 1 al 9 
                pthread_exit(NULL);
            }
        }
        
    }
    
    printf("\n------\nTodas las columnas contienen los numeros 1-9\n------\n");
    // Todas las columnas contienen todos los numeros del 1 al 9 
    pthread_exit(NULL);
}


// Fila
int checkRow() {
	unsigned int tid = 0;
    int cells[9];

	#pragma omp parallel for ordered private(tid)
    for (int i = 0; i < 9; i++)
    {
        tid = omp_get_thread_num();
        printf("Programa en paralelo: thread: %d %d\n", tid, syscall(SYS_gettid));

        // Se obtiene una fila
        #pragma omp ordered
        for (int j = 0; j < 9; j++)
        {
            cells[j] = sudoku[i][j];
        }

        int checkNumbers[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

        // Por celda en la fila
        #pragma omp ordered
        for (int cell = 0; cell < 9; cell++)
        {
            // Por cada numero en los numeros
            for (int i = 0; i < 9; i++)
            {
                // Si el numero es igual al de la celda
                if (checkNumbers[i] == cells[cell]) {
                    // Colorcarlo en -1 (ya usado)
                    checkNumbers[i] = -1;
                    break;
                }
            }
        }

        // Por celda en la fila
        #pragma omp ordered
        for (int cell = 0; cell < 9; cell++)
        {
            if (checkNumbers[cell] != -1) {
                printf("\n------\nAlguna fila NO contiene todos los numeros 1-9\n------\n");
            }
        }
        
    }

    printf("\n------\nTodas las filas contienen los numeros 1-9\n------\n");
}


// Cuadricula 3x3
int checkGrid() {
	unsigned int tid = 0;
    int cells[9];

	#pragma omp parallel for ordered private(tid)
    // Nos vamos moviendo por filas
    for (int contadorFilas = 0; contadorFilas < 9; contadorFilas+=3)
    {   
        tid = omp_get_thread_num();
        printf("Programa en paralelo: thread: %d %d\n", tid, syscall(SYS_gettid));

        // Nos vamos moviendo por columnas
        for (int contadorColumna = 0; contadorColumna < 9; contadorColumna+=3)
        {
            // Resetear el contador
            int contador = 0;
            
            // Se obtiene una fila
            #pragma omp ordered
            for (int i = 0; i < 3; i++)
            {
                // Se obtiene una columna
                for (int j = 0; j < 3; j++)
                {
                    // Se suman los contadores para el grid correcto
                    cells[contador] = sudoku[i+contadorFilas][j+contadorColumna];
                    contador++;
                }
            }

            // Todas las posiciones de las celdas
            #pragma omp ordered
            for (int i = 0; i < 9; i++) {
                int checkNumbers[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
                // Por celda en la cuadricula
                for (int cell = 0; cell < 9; cell++)
                {
                    // Por cada numero en los numeros
                    for (int i = 0; i < 9; i++)
                    {
                        // Si el numero es igual al de la celda
                        if (checkNumbers[i] == cells[cell]) {
                            // Colorcarlo en -1 (ya usado)
                            checkNumbers[i] = -1;
                            break;
                        }
                    }
                }

                // Por celda en la cuadricula
                for (int cell = 0; cell < 9; cell++)
                {
                    if (checkNumbers[cell] != -1) {
                        printf("\n------\nAlguna cuadricula NO contiene todos los numeros 1-9\n------\n");
                        // La cuadricula NO contiene todos los numeros del 1 al 9 
                    }
                }   
            }
        }
    }
   
    printf("\n------\nTodas las cuadriculas contienen los numeros 1-9\n------\n");
    // Todas las cuadriculas contienen todos los numeros del 1 al 9 
}

int main(int argc, char const *argv[])
{
    // Shared memory
    int SIZE = 4096;
    // File
    int fd = open(argv[1], O_RDONLY, 0666);

    struct stat sb;
    if (fstat(fd, &sb) == -1)
    {
        printf("Error producido en file %s\n", strerror(errno));

        return 1;
    }

    // Checkeamos y asignamos un tamano si es 0
    if (sb.st_size != 0) {
        SIZE = sb.st_size;
    }
    // Asignamos el valor al objeto de la memoria compartida
    ftruncate(fd, SIZE);

    // Shared memory
    char* opened_file = mmap(NULL, SIZE, PROT_READ, MAP_PRIVATE,fd, 0 );


    for (int i = 0; i < 81; i++)
    {
        sudoku[i / 9][i % 9] = opened_file[i] - '0';
    }

    
    printf("Numero del proceso padre %d\n", (int)getppid());

    int f1 = fork();

    if (f1 == 0) {
        char parentID[10];
        sprintf(parentID, "%d", (int)getppid());
        execlp("ps", "ps", "-p", parentID, "-lLf", (char*)NULL);
        printf("\n");
    } else {
        pthread_t tid;
        pthread_create(&tid, NULL, checkColumn, (void*)&tid);
        pthread_join(tid, NULL);
        printf("Num de thread %d\n", syscall(SYS_gettid));
        wait(NULL);
    }

    checkRow();
    checkGrid();

        
    printf("Numero del proceso padre %d\n", (int)getppid());

    int f2 = fork();

    if (f2 == 0) {
        char parentID[10];
        sprintf(parentID, "%d", (int)getppid());
        execlp("ps", "ps", "-p", parentID, "-lLf", (char*)NULL);
        printf("\n");
    } else {
        wait(NULL);
    }

    return 0;
}
