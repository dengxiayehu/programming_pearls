#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define NELEM(arr) (sizeof(arr)/sizeof(arr[0]))

typedef struct Item {
    int sum;
    int i;
} Item;

int compar(const void *arg1, const void *arg2)
{
    return ((const Item *)arg1)->sum - ((const Item *)arg2)->sum;
}

int min(int a, int b)
{
    return a < b ? a : b;
}

int max(int a, int b)
{
    return a > b ? a : b;
}

int main(int argc, char const *argv[])
{
    //int array[] = { 31, -41, 59, 26, -53, 58, 97, -93, -23, 84 };
    int array[] = { -3, 1, 1, -3, 5 };
    //int array[] = { -1, -1, 2 };
    //int array[] = { -99 };
    const int n = NELEM(array);
    Item tmp[n+1], *curmum = tmp+1;
    int i;
    int nearest;
    int l;
    int r;

    curmum[-1].sum = 0;
    for (i = 0; i < n; i++) {
        // curmum[i] is the sum of elements with index -1 to i.
        curmum[i].sum = curmum[i-1].sum+array[i];
        curmum[i].i = i;
    }

    // Sort curmum array. nlogn
    qsort(curmum, n, sizeof(Item), compar);

    // Print the sorted curmum.
    for (i = 0; i < n; i++) {
        printf("%d,%d ", curmum[i].sum, curmum[i].i);
    }
    printf("\n");

    // n
    nearest = abs(curmum[0].sum);
    l = r = 0;
    for (i = 0; i < n; i++) {
        if (curmum[i].sum == 0) { // find the exact value
            nearest = 0;
            l = 0;
            r = curmum[i].i;
            break;
        } else {
            if (i < n - 1) {
                int diff = abs(curmum[i].sum - curmum[i+1].sum);

                if (diff < nearest) {
                    nearest = diff;
                    l = min(curmum[i].i, curmum[i+1].i)+1; // note, add 1
                    r = max(curmum[i].i, curmum[i+1].i);
                }
            }
        }
    }

    printf("the nearest abs value is: %d, %d..%d\n", nearest, l, r);

    return 0;
}
