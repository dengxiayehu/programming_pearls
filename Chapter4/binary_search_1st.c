#include <stdio.h>

#define NELEM(arr) sizeof(arr)/sizeof(arr[0])

int binary_search_1st(int arr[], int n, int key)
{
    int l;
    int u;

    if (!arr || n <= 0) { // Invalid param.
        return -1;
    }

    l = 0;
    u = n - 1;
    while (l <= u) {
        int mid = l + (u - l)/2;
        
        if (arr[mid] < key) { // key must be range [mid+1 .. u]
            l = mid + 1;
        } else if (arr[mid] > key) { // key must be range [l, mid-1]
            u = mid - 1;
        } else {
            if (mid == 0 || arr[mid-1] != key) { // arr[mid] equals key and is the first occurance.
                return mid;
            } else {
                u = mid - 1; // find left 1st element with value key
            }
        }
    }

    return 0;
}

int main(int argc, char const* argv[])
{
    // Note: arr must be in order.
    int arr[] = { 1, 2, 4, 4, 7, 7, 7, 8, 9, 9, 10, 11, 12 };
    int num;

    while (scanf("%d", &num) != EOF) {
        int idx = binary_search_1st(arr, NELEM(arr), num);

        if (idx >= 0) {
            printf("Found! Index is: %d\n", idx);
        } else {
            printf("Not found!\n");
        }
    }
    
    return 0;
}
