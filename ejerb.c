#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
    pid_t fork1, fork2, fork3;
    clock_t start, end;
    int million = 1000000;

    start = clock()
    
    // hijo
    fork1 = fork()

    if (fork1 == 0) {
        // 2do hijo
        fork2 = fork()
        if (fork2 == 0) {
            // 3er hijo
            fork3 = fork()
            if (fork3 == 0) {
                for (int i = 0; i < million; i++) {}
            } else {
                for (int i = 0; i < million; i++) {}
                wait(NULL)
            } 
        } else {
            for (int i = 0; i < million; i++) {}
            wait(NULL)
        }
    } else {
        wait(NULL)
        end = clock()
    }

    time = (double)(end - start);
    printf("El tiempo final es de %f ms\n", time);

    return 0;
}
