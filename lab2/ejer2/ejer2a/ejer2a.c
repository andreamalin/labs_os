#include <stdio.h>
#include <time.h>

int main(int argc, char const *argv[])
{
    clock_t start, end;
    double time;
    int million = 1000000;
    int i = 0;

    start = clock();
    for (i = 0; i < million; i++) {}
    for (i = 0; i < million; i++) {}
    for (i = 0; i < million; i++) {}
    
    end = clock();
    time = (double)(end - start);

    printf("El tiempo final es de %f ms\n", time);
    return 0;
}
