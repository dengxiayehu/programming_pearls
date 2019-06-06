#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main(int argc, char const *argv[])
{
    int *arr;
    int i;
    int n;
    int t;
    int hold;

    printf("input n: ");
    if (scanf("%d", &n) == EOF) {
        return 0;
    }
    assert(n > 0);

    arr = (int*)malloc((n+1)*sizeof(int)); // +1 for the guard
    assert(arr);
    // Init the arr, 1, 2, 3, ... n.
    for (i = 0; i < n; i++) {
        arr[i] = i+1;
    }

    for ( ; ; ) {
        printf("input t: ");
        if (scanf("%d", &t) == EOF) {
            break;
        }

        hold = arr[n]; // backup the element which will be replaced by the guard
        arr[n] = t; // set the guard
        for (i = 0; ; i++) {
            if (arr[i] == t) {
                break;
            }
        }
        arr[n] = hold; // recover the backup element
        if (i == n) {
            printf("not found\n");
        } else {
            printf("found with index: %d\n", i);
        }
    }

    free(arr);

    return 0;
}
