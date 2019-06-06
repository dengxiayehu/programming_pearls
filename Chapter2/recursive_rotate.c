#include <stdio.h>
#include <stdlib.h>

#define NELEM(arr) (sizeof(arr)/sizeof(arr[0]))

void swap_array(int arr[], int start_idx1, int start_idx2, int num)
{
    int i, t;

    for (i = 0; i < num; i++) {
        t = arr[start_idx1 + i];
        arr[start_idx1 + i] = arr[start_idx2 + i];
        arr[start_idx2 + i] = t;
    }
}

void foo(int* arr, int arr_len, int left_num)
{
    int right_num = arr_len - left_num;

    if (right_num == left_num) { // left part equals to right part, swap directly
        swap_array(arr, 0, left_num, left_num);
    } else if (left_num < right_num) { // left part is less that right part
        // Swap arr[0 .. left_num) with arr[arr_len - left_num, arr_len).
        swap_array(arr, 0, arr_len - left_num, left_num);
        // arr[arr_len - left_num .. arr_len) is done.
        foo(arr, arr_len - left_num, left_num);
    } else { // less part is bigger than right part
        // Swap arr[0 .. arr_len - right_num) with arr[arr_len - right_num, arr_len).
        swap_array(arr, 0, arr_len - right_num, right_num);
        // arr[0 .. right_num) is done.
        foo(arr + right_num, arr_len - right_num, left_num - right_num);
    }
}

int main(int argc, const char* argv[])
{
    int arr[] = { 1, 2, 3, 4, 5, 6, 7 };
    int arr_len = NELEM(arr);
    int left_num = 0;
    int i = 0;

    left_num = atoi(argv[1]);
    left_num %= arr_len;
    if (left_num == 0) {
        printf("no need to left rotate\n");
        return 1;
    }

    foo(arr, arr_len, left_num);

    for (i = 0; i < arr_len; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}
