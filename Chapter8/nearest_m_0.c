#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <assert.h>

#define NELEM(arr) (sizeof(arr)/sizeof(arr[0]))

int main(int argc, const char *argv[])
{
    int array[] = { -3, 1, 1, -3, 5 };
    const int n = NELEM(array);
    int tmp[n+1], *curmum = tmp+1;
    int i;
    int m;
    int min_gap;
    int min_i;

    printf("array: ");
    for (i = 0; i < n; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");

    // Init curmum array.
    printf("curmum: ");
    curmum[-1] = 0;
    for (i = 0; i < n; i++) {
        curmum[i] = curmum[i-1]+array[i];
        printf("%d ", curmum[i]);
    }
    printf("\n");

    for ( ; ; ) {
        printf("input m: "); // m stands for m numbers
        if (scanf("%d", &m) == EOF) {
            break;
        }
        assert(m > 0 && m <= n);

        m--;

        min_gap = INT_MAX;
        for (i = 0; i < n-m; i++) {
            int abs_sum = abs(curmum[i+m]-curmum[i-1]);
            if (abs_sum < min_gap) {
                min_gap = abs_sum;
                min_i = i;
            }
        }
        printf("min_gap=%d, min_i=%d\n", min_gap, min_i);
    }

    return 0;
}
