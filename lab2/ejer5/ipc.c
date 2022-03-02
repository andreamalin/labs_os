#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{

    // Progra defensiva
    if (argc < 3) {
        printf("Faltan argumentos\n");
        exit(1);
    }

    // Obteniendo los argumentos
    int n = (int)argv[1];
    const char *x = argv[2];
    // Shared memory
    int SIZE = 20;
    int currentPos = 0;
    // Pipes
    int b[2];
    pipe(b);

    // File descriptor
    int fd = shm_open("file1.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        printf("Error producido en file descriptor %s\n", strerror(errno));
        exit(1);
    } else {
        printf("%d\n", fd);
    }

    // Shared memory
    char *shared_memory = mmap(NULL, SIZE*sizeof(int), PROT_READ | PROT_WRITE,
                                    MAP_SHARED | MAP_ANONYMOUS,
                                    fd, 0 );

    // 
    int f1 = fork();
    if (f1 == 0) {
        // Hijo
        close(b[1]); // Cerramos escritura
        // Si es valor 1, significa que escribimos en la memoria
        if (read(b[1], 1, 1) == 1) {
            shared_memory[currentPos] = x;
            currentPos++;
        }
    } else {
        wait(NULL);
        // Padre
        for (int i = 0; i < SIZE; i++)
        {
            close(b[0]); // Cerramos lectura

            if (i % n == 0) {
                write(b[1], 1, 1); // Escribimos valor 1
            } else {
                write(b[1], 0, 1); // Escribimos valor 0
            }
            
            close(b[1]); // Cerramos escritura
        }
        
    }

    return 0;
}
