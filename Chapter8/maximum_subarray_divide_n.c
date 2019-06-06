#include <stdio.h>

#define NELEM(arr) (sizeof(arr)/sizeof(arr[0]))

int max(int a, int b)
{
    return a > b ? a : b;
}

int max3(int a, int b, int c)
{
    return max(a, max(b, c));
}

// 时间复杂度为O(n)的分治算法求取最大子数组问题。
int maximum_divide(int array[], int l, int r)
{
    int m;
    int lmax, lidx, rmax, ridx, sum;
    int i;

    if (l > r) { // array is empty
        return 0;
    } else if (l == r) {
        return max(array[l], 0);
    }

    m = l+(r-l)/2;
    lmax = sum = 0;
    lidx = m;
    for (i = m; i >= l; i--) {
        sum += array[i];
        if (sum > lmax) {
            lmax = sum;
            lidx = i;
        }
    }
    rmax = sum = 0;
    ridx = m+1;
    for (i = m+1; i <= r; i++) {
        sum += array[i];
        if (sum > rmax) {
            rmax = sum;
            ridx = i;
        }
    }

    return max3(lmax+rmax,
                maximum_divide(array, l, lidx-1),
                maximum_divide(array, ridx+1, r));
}

int main(int argc, char const *argv[])
{
    int array[] = { 31, -41, 59, 26, -53, 58, 97, -93, -23, 84 };
    const int n = NELEM(array);
    int maxsofar;

    maxsofar = maximum_divide(array, 0, n-1);
    printf("maxsofar=%d\n", maxsofar);

    return 0;
}
