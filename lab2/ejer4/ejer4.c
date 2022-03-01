#include <stdio.h>
#include <unistd.h>

int main() {
    int f1 = fork();

    if (f1 == 0) {
        printf("Entra en hijo\n", (int)getpid());
    } else {
        while (1) { }
    }
}