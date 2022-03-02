#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>

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
    int updateSharedMemory = 0;
    int currentPos = 0;

    // Obteniendo file descriptor
    int fd = open("file1.txt", O_RDONLY);
    if (fd == -1)
    {
        printf("File doesn't exists\n");
    } else {
        printf("%d\n", fd);
    }

    // Abrir espacio de memoria
    int *shared_memory = mmap(NULL, SIZE*sizeof(int), PROT_READ | PROT_WRITE,
                                    MAP_SHARED | MAP_ANONYMOUS,
                                    0, 0 );

    close(fd);

    // 
    int f1 = fork();
    if (f1 == 0) {
        // Hijo
        if (updateSharedMemory == 1) {
            shared_memory[currentPos] = x;
            currentPos++;
            updateSharedMemory = 0;
        }
    } else {
        // Padre
        for (int i = 0; i < SIZE; i++)
        {
            if (i % n == 0) {
                updateSharedMemory = 1;
            }
        }
        
    }

    return 0;
}
