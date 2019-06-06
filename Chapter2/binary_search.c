#include <stdio.h>
#include <stdlib.h>

#define NELEM(arr) (sizeof(arr)/sizeof(arr[0]))

static int binary_search(int arr[], int len, int key)
{
    int l = 0;
    int r = len - 1;
    int mid = 0;

    // In case a null array, avoid crash.
    if (arr == NULL) {
        return -1;
    }

    while (l <= r) {
        // locate the middle of index l and r
        // NOTE: mid = (r - l)/2; is wrong!
        mid = l + (r - l)/2;
        if (arr[mid] == key) { // found it
            return mid;
        } else if (arr[mid] > key) { // key must in arr[l .. mid-1]
            r = mid - 1;
        } else { // key must in arr[mid+1 .. r]
            l = mid + 1;
        }
    }

    // Not found case.
    return -1;
}

int main(int argc, const char *argv[])
{
    int arr[] = { 1, 3, 4, 6, 9, 10 };
    int len = NELEM(arr);
    int key = 0;
    int i = 0;

    if (argc != 2) {
        fprintf(stderr, "usage: %s <key>\n", argv[0]);
        return 1;
    }

    key = atoi(argv[1]);

    i = binary_search(arr, len, key);
    if (i >= 0) {
        printf("Found key:%d with index:%d\n", key, i);
    } else {
        printf("Not found!\n");
    }

    return 0;
}
