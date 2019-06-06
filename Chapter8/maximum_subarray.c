#include <stdio.h>

#define NELEM(arr) (sizeof(arr)/sizeof(arr[0]))

int main(int argc, char const *argv[])
{
    int array[] = { 31, -41, 59, 26, -53, 58, 97, -93, -23, 84 };
    const int n = NELEM(array);
    int i;
    int j;
    int k;
    int maxsofar = 0;

    // Iterate each range with index [i, j], find the maximum subarray.
    for (i = 0; i < n; i++) {
        for (j = i; j < n; j++) {
            int sum = 0;

            // Calculate the sum between [i, j].
            for (k = i; k <= j; k++) {
                sum += array[k];
            }

            if (sum > maxsofar) {
                maxsofar = sum;
            }
        }
    }

    printf("maxsofar=%d\n", maxsofar);

    return 0;
}
