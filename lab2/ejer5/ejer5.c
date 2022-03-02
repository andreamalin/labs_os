#include <stdio.h>
#include <unistd.h>

int main() {
    int f1 = fork();

    if (f1 == 0) {
        execl("./ipc", "./ipc", "5", "a", (char*)NULL);
    } else {
        wait(NULL);
        execl("./ipc", "./ipc", "2", "b", (char*)NULL);
        shm_unlink("test");
    }
}