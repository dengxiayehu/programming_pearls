#include <stdio.h>
#include <stdlib.h>

#define NELEM(arr) (sizeof(arr)/sizeof(arr[0]))

int gcd(int a, int b)
{
    while (a != b) {
        if (a > b) {
            a -= b;
        } else {
            b -= a;
        }
    }

    return a;
}

int main(int argc, const char* argv[])
{
    int arr[] = { 1, 2, 3, 4, 5, 6 };
    int arr_len = NELEM(arr);
    int left_num = 0;
    int i;

    if (argc != 2) {
        fprintf(stderr, "%s <left_num>\n", argv[0]);
        return 1;
    }

    left_num = atoi(argv[1]);

    // Preprocess left_num.
    left_num %= arr_len;

    if (left_num == 0) {
        printf("no need to left rotate\n");
        return 1;
    }

    for (i = 0; i < gcd(left_num, arr_len); i++) {
        int t = arr[i]; // backup current arr[i]
        int k = i + left_num; // next slot
        int j = i;

        // Take care this condition.
        while (k != i) {
            // Take next slot number into here.
            arr[j] = arr[k];
            // Update j to prepare for next slot.
            j = k;

            // Update k to the slot before current next slot.
            k += left_num;
            if (k >= arr_len) {
                // Wrap back.
                k -= arr_len;
            }
        }

        // Update the backup one.
        arr[j] = t;
    }

    for (i = 0; i < arr_len; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}
