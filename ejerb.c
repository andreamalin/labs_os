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
    // 2do hijo
    fork2 = fork()
    // 3er hijo
    fork3 = fork()

    if (fork1 == 0 && fork2 !== 0 && fork3 !== 0) {
        for (int i = 0; i < million; i++) {}
    } else if (fork1 !== 0 && fork2 == 0 && fork3 !== 0) {
        for (int i = 0; i < million; i++) {}
    } else if (fork1 !== 0 && fork2 !== 0 && fork3 == 0) {
        for (int i = 0; i < million; i++) {}
    } else {
        wait(NULL)
    }
    

    return 0;
}
