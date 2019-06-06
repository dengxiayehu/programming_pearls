#include <stdio.h>
#include <stdlib.h>

#define NELEM(arr) (sizeof(arr)/sizeof(arr[0]))

void swap_array(int arr[], int start_idx1, int start_idx2, int num)
{
    int i = 0;
    int t = 0;

    for (i = 0; i < num; i++) {
        t = arr[start_idx1 + i];
        arr[start_idx1 + i] = arr[start_idx2 + i];
        arr[start_idx2 + i] = t;
    }
}

int main(int argc, char const* argv[])
{
    int arr[] = { 1, 2, 3, 4, 5, 6, 7 };
    int arr_len = NELEM(arr);
    int left_num = 0;
    int right_num = 0;
    int left_bound = 0;
    int right_bound = arr_len - 1;
    int i = 0;

    if (argc != 2) {
        fprintf(stderr, "usage: %s <left_rotate_num>\n", argv[0]);
        return 1;
    }

    left_num = atoi(argv[1]);
    left_num %= arr_len;
    if (left_num == 0) {
        printf("no need to left rotate\n");
        return 1;
    }

    right_num = arr_len - left_num;

    // 迭代法就是递归法的直接翻译。
    while (left_num != right_num) {
        if (left_num > right_num) {
            swap_array(arr, left_bound, right_bound - right_num + 1, right_num);
            left_num -= right_num;
            left_bound += right_num;
        } else {
            swap_array(arr, left_bound, right_bound - left_num + 1, left_num);
            right_num -= left_num;
            right_bound -= left_num;
        }
    }
    swap_array(arr, left_bound, right_bound - left_num, left_num);

    for (i = 0; i < arr_len; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}
