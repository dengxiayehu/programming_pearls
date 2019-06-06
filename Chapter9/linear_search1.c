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
        for (i = 0; ; i+=8) {
            if (arr[i] == t)   {        break; }
            if (arr[i+1] == t) { i+= 1; break; }
            if (arr[i+2] == t) { i+= 2; break; }
            if (arr[i+3] == t) { i+= 3; break; }
            if (arr[i+4] == t) { i+= 4; break; }
            if (arr[i+5] == t) { i+= 5; break; }
            if (arr[i+6] == t) { i+= 6; break; }
            if (arr[i+7] == t) { i+= 7; break; }
        }
        arr[n] = hold; // recover the backup element, sometimes not necessary
        if (i == n) {
            printf("not found\n");
        } else {
            printf("found with index: %d\n", i);
        }
    }

    free(arr);

    return 0;
}