#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
    pid_t fork1, fork2, fork3;
    clock_t start, end;
    double time;
    int million = 1000000;

    
    start = clock();
    printf("start %f s", (double)start);
    // hijo
    fork1 = fork();

    if (fork1 == 0) {
        // 2do hijo
        fork2 = fork();
        if (fork2 == 0) {
            // 3er hijo
            fork3 = fork();
            if (fork3 == 0) {
                for (int i = 0; i < million; i++) {
                    printf("%d", i);
                }
            } else {
                for (int i = 0; i < million; i++) {
                    printf("%d", i);
                }
                wait(NULL);
            } 
        } else {
            for (int i = 0; i < million; i++) {
                printf("%d", i);
            }
            wait(NULL);
        }
    } else {
        wait(NULL);
        end = clock();
        printf("end %f s\n", (double)end);
        time = (double)(end - start)/CLOCKS_PER_SEC;
    

        printf("El tiempo final es de %f s\n", time);
    
    }

    return 0;
}
