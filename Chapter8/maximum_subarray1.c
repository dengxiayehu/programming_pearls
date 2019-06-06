#include <stdio.h>

#define NELEM(arr) (sizeof(arr)/sizeof(arr[0]))

int main(int argc, char const *argv[])
{
    int array[] = { 31, -41, 59, 26, -53, 58, 97, -93, -23, 84 };
    const int n = NELEM(array);
    int i;
    int j;
    int maxsofar = 0;

    // Iterate each range of index [i..j], find the maximum subarray.
    for (i = 0; i < n; i++) {
        int sum = 0;

        for (j = i; j < n; j++) {
            // Calculate the sum of index [i..j].
            // Sum of index [i..j] is equal to sum[i..j-1]+array[j].
            sum += array[j];

            if (sum > maxsofar) {
                maxsofar = sum;
            }
        }
    }

    printf("maxsofar=%d\n", maxsofar);

    return 0;
}
