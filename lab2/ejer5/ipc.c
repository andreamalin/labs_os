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
    int SIZE = 4096;
    int currentPos = 0;
    // Pipes
    int b[2];
    pipe(b);

    // File descriptor
    int fd = shm_open("test", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    struct stat sb;
    if (fstat(fd, &sb) == -1)
    {
        printf("Error producido en file descriptor %s\n", strerror(errno));

        return 1;
    }

    // Checkeamos y asignamos un tamano si es 0
    printf("%d\n", sb.st_size);
    if (sb.st_size != 0) {
        SIZE = sb.st_size;
    }
    printf("SIZE %d\n", SIZE);

    // Asignamos el valor al objeto de la memoria compartida
    ftruncate(fd, SIZE);

    // Shared memory
    void *shared_memory = mmap(NULL, SIZE, PROT_READ | PROT_WRITE,
                                    MAP_SHARED,
                                    fd, 0 );

    // 
    int f1 = fork();
    if (f1 == 0) {
        // Hijo
        
        while(1) {
            // Leo el valor y guardo en x
            int x = 0;
            read(b[0], (char*)&x, sizeof(int));

            // Si es valor 1, significa que escribimos en la memoria
            if (x == 1) {
                sprintf(shared_memory, "%s", argv[2]);
                shared_memory += strlen(argv[2]);
                currentPos++;
            }
            sleep(1);
        }

        close(b[1]); // Cerramos escritura
    } else {
        // Padre
        for (int i = 0; i < 10; i++)
        {
            int value = 0;
            if (i % n == 0) {
                // Escribimos valor 1
                value = 1;
            }
    
            // Mando valor y espero al hijo
            write(b[1], (char*)&value, sizeof(int));
            sleep(2);
        }

        // Termina el ciclo
        printf("Valor final en la memoria compartida: %s\n", (char*)shared_memory);
        // Remover la memoria cmpartida
        shm_unlink("test");
    }

    return 0;
}
