#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/* Simulate workload by performing random computation */
static inline void tau1(void) {
    long long i = rand() % 10000;
    long long j = rand() % 10000;
    volatile long long result = i * j; /* Use volatile to prevent compiler optimization */
}

int main(void) {
    clock_t start, end;
    start = clock();

    tau1();

    end = clock();
    double time_taken = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Execution time: %f seconds\n", time_taken);

    return 0;
}

