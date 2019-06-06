#include <stdio.h>

#define NELEM(arr) (sizeof(arr)/sizeof(arr[0]))

#ifndef MAX
#define MAX(a, b) ((a)>(b)?(a):(b))
#endif

int max3(int a, int b, int c)
{
    return MAX(a, MAX(b, c));
}

int maximum_divide(int array[], int l, int r)
{
    int mid;
    int lmax;
    int rmax;
    int sum;
    int i;

    if (l > r) {
        // Empty array, return maximum 0.
        return 0;
    }

    if (l == r) {
        // Only 1 element, return maximum(0, array[l]).
        return MAX(0, array[l]);
    }

    mid = l + (r-l)/2;
    // Get the lmax of the subarray cross the middle.
    lmax = sum = 0;
    for (i = mid; i >= l; i--) { // index mid to l
        sum += array[i];
        if (sum > lmax) {
            lmax = sum;
        }
    }
    // Get the rmax of the subarray cross the middle.
    rmax = sum = 0;
    for (i = mid+1; i <= r; i++) { // index mid+1 to r
        sum += array[i];
        if (sum > rmax) {
            rmax = sum;
        }
    }

    // 为避免宏定义展开的副作用，这里使用max3函数来找三个数中的最大值。
    return max3(lmax+rmax, maximum_divide(array, l, mid-1), maximum_divide(array, mid+1, r));
}

int main(int argc, char const *argv[])
{
    int array[] = { 31, -41, 59, 26, -53, 58, 97, -93, -23, 84 };
    const int n = NELEM(array);
    int maxsofar = 0;

    maxsofar = maximum_divide(array, 0, n-1);
    printf("maxsofar=%d\n", maxsofar);

    return 0;
}
