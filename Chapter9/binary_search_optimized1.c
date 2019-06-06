#include <stdio.h>

#define N 1000

int main(int argc, char const *argv[])
{
    int arr[N];
    int l;
    int i;
    int t;
    int p;

    for (i = 0; i < N; i++) {
        arr[i] = i+1;
    }

    for ( ; ; ) {
        printf("Input t: ");
        if (scanf("%d", &t) == EOF) {
            break;
        }

        // Check t is in (-1, 511) or (488, 1000).
        l = -1;
        i = 512;
        if (arr[511] < t) {
            l = 1000 - 512;
        }

        // Binary search algorithm.
        while (i > 1) {
            i >>= 1;
            if (arr[l+i] < t) {
                l += i;
            }
        }
        p = l+1;
        if (p >= N || arr[p] != t) {
            p = -1;
        }
        printf("index: %d\n", p);
    }

    return 0;
}
