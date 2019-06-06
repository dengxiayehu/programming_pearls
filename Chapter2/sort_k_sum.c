#include <stdio.h>
#include <stdlib.h>

#define NELEM(arr) (sizeof(arr)/sizeof(arr[0]))

// The most foundamental one.
int partition(int arr[], int left, int right)
{
    // Take arr[right] as the standard element.
    int t = arr[right];

    while (left < right) {
        // Left to right, find the element bigger than t.
        while (left < right && arr[left] <= t) left++;
        // Take arr[left] (the bigger one) to arr[right].
        arr[right] = arr[left];

        // Right to left, find the element smaller than t.
        while (right > left && arr[right] >= t) right--;
        // Take arr[right] (the smaller one) to arr[left].
        arr[left] = arr[right];
    }

    // Take t into arr[left].
    arr[left] = t;

    return left;
}

int median3(int a[], int i, int j, int k)
{
    return (a[i] < a[j]) ?
            (a[j] < a[k] ? j : a[i] < a[k] ? k : i) : (a[k] < a[j] ? j : a[k] < a[i] ? k : i);
}

#define SWAP(arr, i, j) do { \
    arr[i] ^= arr[j]; arr[j] ^= arr[i]; arr[i] ^= arr[j]; \
} while (0)

// 三取样切分。
int partition1(int arr[], int left, int right)
{
    // Found the medium number of arr[left] arr[(right+left)/2] arr[right].
    int m = median3(arr, left, (right+left)/2, right);
    int i, j, t;

    // If use xor method to exchange two elements, must not the same value, or the swapped value is 0!
    if (left != m) {
        // Exchange arr[left] with arr[m], and take the new arr[left] as standard element.
        SWAP(arr, left, m);
    }

    i = left;
    j = right;
    t = arr[left]; // the standard element

    // Special case 1.
    while (arr[++i] <= t) {
        if (i == right) {
            // The right most element is still smaller than t, swap arr[left] and arr[right].
            SWAP(arr, left, right);
            return right;
        }
    }

    // Special case 2.
    while (arr[j] >= t) {
        if (j == left) {
            // The left most element is still bigger than t, already in "order" state.
            return left;
        }
        --j;
    }

    // Currently arr[i] > t and arr[j] < t.

    while (i < j) {
        // Exchange arr[i] with arr[j].
        SWAP(arr, i, j);

        // Try to find next i and j.
        while (arr[++i] <= t) ;
        while (arr[--j] >= t) ;
    }

    // Take the standard element into the right place.
    // Current arr[j] is smaller than t.
    SWAP(arr, j, left);

    return j;
}

int partition2(int arr[], int left, int right)
{
    // Found the medium number of arr[left] arr[(right+left)/2] arr[right].
    int m = median3(arr, left, (right+left)/2, right);
    int i, j, t;

    // If use xor method to exchange two elements, must not the same value, or the swapped value is 0!
    if (left != m) {
        // Exchange arr[left] with arr[m], and take the new arr[left] as standard element.
        SWAP(arr, left, m);
    }

    i = left;
    j = right;
    t = arr[left]; // the standard element

    // Special case 1.
    while (arr[++i] <= t) {
        if (i == right) {
            // The right most element is still smaller than t, swap arr[left] and arr[right].
            SWAP(arr, left, right);
            return right - left + 1;
        }
    }

    // Special case 2.
    while (arr[j] >= t) {
        if (j == left) {
            // The left most element is still bigger than t, already in "order" state.
            return 1;
        }
        --j;
    }

    // Currently arr[i] > t and arr[j] < t.

    while (i < j) {
        // Exchange arr[i] with arr[j].
        SWAP(arr, i, j);

        // Try to find next i and j.
        while (arr[++i] <= t) ;
        while (arr[--j] >= t) ;
    }

    // Take the standard element into the right place.
    // Current arr[j] is smaller than t.
    SWAP(arr, j, left);

    return j - left + 1;
}

void my_qsort(int arr[], int left, int right)
{
    int idx = 0;

    if (left >= right) {
        return;
    }

    idx = partition1(arr, left, right);
    my_qsort(arr, left, idx - 1);
    my_qsort(arr, idx + 1, right);
}

int main(int argc, char const* argv[])
{
    int arr[] = { 4, 5, 9, 1, 2, 10, 6, 3, 99, 46, 23, 11, -1, -3, -1 };
    /*int arr[] = { 1, 2, 3, 4, 5 };*/
    /*int arr[] = { 5, 4, 3, 2, 1 };*/
    int k = 0;
    int t = 0;
    int i = 0;
    int left = 0;
    int right = NELEM(arr) - 1;
    int n = 0;
    int k_bak = 0;
    int sum = 0;

    /*my_qsort(arr, 0, right);*/

    if (argc != 3) {
        fprintf(stderr, "%s <k> <t>\n", argv[0]);
        return 1;
    }

    k = k_bak = atoi(argv[1]);
    t = atoi(argv[2]);

    printf("original array is: \n");
    for (i = 0; i < NELEM(arr); i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    for ( ; ; ) {
        n = partition2(arr, left, right);
        if (n == k) {
            break;
        } else if (n < k) {
            k -= n;
            left += n;
        } else {
            right = left + n - 1 - 1;
        }
    }

    k = k_bak;
    printf("k = %d, t = %d\n", k, t);
    for (i = 0; i < NELEM(arr); i++) {
        printf("%d ", arr[i]);
        if (i < k) {
            sum += arr[i];
            if (i == k - 1) {
                printf("| ");
            }
        }
    }
    printf("\n");

    if (sum < t) {
        printf("Found\n");
    } else {
        printf("Not found\n");
    }

    return 0;
}
