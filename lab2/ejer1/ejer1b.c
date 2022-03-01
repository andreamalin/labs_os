#include <stdio.h>
#include <unistd.h>

int main() {
    int i = 0;
    while (i < 4) {
        int f = fork();
        printf("%d\n", (int)getpid());
        ++i;
    }
}