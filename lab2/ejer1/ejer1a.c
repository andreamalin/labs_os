#include <stdio.h>
#include <unistd.h>

int main() {
    int f1 = fork();
    int f2 = fork();
    int f3 = fork();
    int f4 = fork();

    printf("%d\n", (int)getpid());
}