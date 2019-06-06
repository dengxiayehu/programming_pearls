#include <stdio.h>
#include <stdlib.h>

#define NELEM(arr) (sizeof(arr)/sizeof(arr[0]))

int main(int argc, char const *argv[])
{
    int array[] = { 31, -41, 59, 26, -53, 58, 97, -93, -23, 84 };
    const int n = NELEM(array);
    int i;
    int j;
    int maxsofar = 0;
    int* sum_array, *cumarr;

    sum_array = (int*)malloc(sizeof(int)*(n+1));
    if (!sum_array) {
        perror("malloc failed");
        return 1;
    }

    cumarr = sum_array+1;
    cumarr[-1] = 0; // that is sum_array[0]
    for (i = 0; i < n; i++) {
        cumarr[i] = cumarr[i-1]+array[i];
    }

    printf("the cumarr is: ");
    for (i = 0; i < n; i++) {
        printf("%d ", cumarr[i]);
    }
    printf("\n");

    for (i = 0; i < n; i++) {
        for (j = i; j < n; j++) {
            // Get the sum of [i..j].
            int sum = cumarr[j]-cumarr[i-1];
            if (sum > maxsofar) {
                maxsofar = sum;
            }
        }
    }

    printf("maxsofar=%d\n", maxsofar);

    free(sum_array);

    return 0;
}
