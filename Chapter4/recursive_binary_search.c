#include <stdio.h>

#define NELEM(arr) (sizeof(arr)/sizeof(arr[0]))

int binary_search(int arr[], int l, int r, int key)
{
    int mid;

    if (l > r) {
        return -1;
    }

    mid = l + (r - l)/2;
    if (arr[mid] == key) { // Found
        return mid;
    } else if (arr[mid] > key) { // recursive call binary_search [l .. mid - 1]
        return binary_search(arr, l, mid - 1, key);
    } else { // recursive call binary_search [mid + 1 .. r]
        return binary_search(arr, mid + 1, r, key);
    }
}

int main(int argc, char const* argv[])
{
    // Note: arr must be in order.
    int arr[] = { 1, 2, 4, 4, 7, 7, 7, 8, 9, 9, 10, 11, 12 };
    int num;

    while (scanf("%d", &num) != EOF) {
        int idx = binary_search(arr, 0, NELEM(arr) - 1, num);

        if (idx >= 0) {
            printf("Found! Index is: %d\n", idx);
        } else {
            printf("Not found!\n");
        }
    }
    
    return 0;
}
