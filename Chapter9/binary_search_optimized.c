#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main(int argc, char const *argv[])
{
    int *arr;
    int i;
    int n;
    int t;
    int l;
    int u;
    int m;
    int p;

    printf("input n: ");
    if (scanf("%d", &n) == EOF) {
        return 0;
    }
    assert(n > 0);

    arr = (int*)malloc(n*sizeof(int));
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

        l = -1;
        u = n;
        while (l+1 != u) {
            m = l+(u-l)/2; // locate middle of l..u
            if (arr[m] < t) { // t maybe in right half
                l = m;
            } else { // t maybe in left half
                u = m;
            }
        }

        // assert l+1 = u && arr[l] < t && arr[u] >= t
        p = u;
        if (p >= n || arr[p] != t) {
            p = -1;
        }
        printf("index: %d\n", p);
    }

    free(arr);

    return 0;
}