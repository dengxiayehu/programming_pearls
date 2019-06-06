#include <stdio.h>

#define N 8

#define NELEM(arr) (sizeof(arr)/sizeof(arr[0]))

int main(int argc, char const *argv[])
{
    int x[N+1];
    int tmp[N+1], *cumarr = tmp+1;
    int i;

    for (i = 0; i < NELEM(x); i++) {
        x[i] = 0;
    }

    cumarr[-1] = 0;
    for (i = 0; i < N; i++) {
        cumarr[i] = cumarr[-1]+x[i];
    }

    for (i = 0; i < N; i++) {
        cumarr[N-1] += 1;
        cumarr[i-1] -= 1;
    }

    // Reconstruct the array x.
    for (i = N-1; i >= 0; i--) {
        x[i] = x[i+1]+cumarr[i];
    }

    for (i = 0; i < N; i++) {
        printf("%d ", x[i]);
    }
    printf("\n");

    return 0;
}
