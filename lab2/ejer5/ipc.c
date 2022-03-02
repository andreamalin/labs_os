#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char*argv[])
{

    // Progra defensiva
    if (argc < 3) {
        printf("Faltan argumentos\n");
        return 1;
    }

    // Obteniendo los argumentos
    int n = strtol(argv[1], NULL, 0);
    // Shared memory
    int SIZE = 10;
    int currentPos = 0;
    // Pipes
    int b[2];
    pipe(b);
    // File descriptor
    int fd = open("./file1.txt", O_RDWR, S_IRUSR | S_IWUSR);
    struct stat sb;
    if (fstat(fd, &sb) == -1)
    {
        printf("Error producido en file descriptor %s\n", strerror(errno));

        return 1;
    }
    // Shared memory
    char *shared_memory = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE,
                                    MAP_SHARED,
                                    fd, 0 );

    // 
    int f1 = fork();
    if (f1 == 0) {
        // Hijo
        
        while(1) {
            int x = 0;
            read(b[0], (char*)&x, sizeof(int));

            // Si es valor 1, significa que escribimos en la memoria
            if (x == 1) {
                shared_memory[currentPos] = argv[2];
                currentPos++;
            }
            sleep(1);
        }

        close(b[1]); // Cerramos escritura
    } else {
        // Padre
        for (int i = 0; i < SIZE; i++)
        {
            int value = 0;
            if (i % n == 0) {
                // Escribimos valor 1
                value = 1;
            }
    
            write(b[1], (char*)&value, sizeof(int));
            sleep(2);
        }
        printf("Memoria compartida: %s\n", shared_memory);
    }

    return 0;
}
