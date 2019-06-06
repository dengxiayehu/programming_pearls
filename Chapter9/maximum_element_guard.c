#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define N 20

int main(int argc, char const *argv[])
{
    int arr[N+1];
    int i;
    int max;

    srand((unsigned)time(NULL));

    printf("the array is: ");
    for (i = 0; i < N; i++) {
        arr[i] = rand()%100;
        printf("%d ", arr[i]);
    }
    printf("\n");

    // The algorithm to find the maximum element in array, use guard.
    i = 0;
    while (i < N) {
        max = arr[i];
        arr[N] = max;
        i++;
        while (arr[i] < max) {
            i++;
        }
    }
    printf("the max element is: %d\n", max);

    return 0;
}
