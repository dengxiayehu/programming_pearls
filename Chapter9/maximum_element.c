#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

int max(int a, int b)
{
    return a > b ? a : b;
}

int arrmax(int *arr, int n)
{
    if (n == 1) {
        return arr[0];
    }

    // return MAX(arr[n-1], arrmax(arr, n-1)); // BAD
    return max(arr[n-1], arrmax(arr, n-1)); // GOOD
}

int main(int argc, char const *argv[])
{
    int *arr;
    int n;
    int i;
    int max;

    for ( ; ; ) {
        printf("input array's size: ");
        if (scanf("%d", &n) == EOF) {
            break;
        }
        assert(n > 0);

        arr = (int*)malloc(n*sizeof(int));
        if (!arr) {
            perror("malloc");
            return 1;
        }

        for (i = 0; i < n; i++) {
            arr[i] = n-i;
        }

        max = arrmax(arr, n);
        printf("max is: %d\n", max);
    }

    return 0;
}