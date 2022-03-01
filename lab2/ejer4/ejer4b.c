#include <stdio.h>
#include <unistd.h>

int main() {
    int f1 = fork();

    if (f1 == 0) {
        for (int i = 0; i < 4000000; i++)
        {
            printf("%d", i);
        }
    } else {
        while (1) { }
    }
}