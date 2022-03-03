#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int f1 = fork();

    if (f1 == 0) {
        execl("./ipc", "./ipc", "5", "a", (char*)NULL);
    } else {
        wait(NULL);
        execl("./ipc", "./ipc", "2", "b", (char*)NULL);
    }
}