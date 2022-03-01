#include <stdio.h>
#include <time.h>

int main(int argc, char const *argv[])
{
    clock_t start, end;
    double time;
    int million = 1000000;
    int i = 0;

    start = clock();
    for (i = 0; i < million; i++) {
        printf("%d", i);
    }
    for (i = 0; i < million; i++) {
        printf("%d", i);
    }
    for (i = 0; i < million; i++) {
        printf("%d", i);
    }
    
    end = clock();
    time = (double)(end - start)/CLOCKS_PER_SEC;

    printf("El tiempo final es de %f s\n", time);
    return 0;
}
