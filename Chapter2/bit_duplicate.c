#include <stdio.h>

typedef unsigned char DataType;

#define BITSPERDATA (4)
#define NELEM(arr) (sizeof(arr)/sizeof(arr[0]))

int main(int argc, const char* argv[])
{
    DataType arr[] = { 0, 6, 1, 4, 2, 7, 7, 7, 9, 5, 3, 10, 8 , 11, 12 ,13, 14, 15, 9, 14 }, *parr = arr;
    DataType arr0[NELEM(arr)] = { 0 }; // temp area for corresponding bit is 0
    DataType arr1[NELEM(arr)] = { 0 }; // temp area for corresponding bit is 1
    int arr_len = NELEM(arr);
    DataType check_mask = 0;
    DataType duplicate_data = 0;
    int i = 0;

    for (i = 0; i < BITSPERDATA; i++) {
        int arr0_len = 0;
        int arr1_len = 0;
        int j = 0;

        // Construct check_mask.
        // Note: !Not check_mask <<= i;
        // 1 << 0 = 1, 1 << 1 = 2, 2 << 2 = 8, 8 << 3 = 64, not what we want.
        check_mask = (1 << i);

        for (j = 0; j < arr_len; j++) {
            if (parr[j]&check_mask) { // add to arr1, corresponding bit is 1
                arr1[arr1_len++] = parr[j];
            } else { // add to arr0, corresponding bit is 0
                arr0[arr0_len++] = parr[j];
            }
        }

        if (arr1_len > arr0_len) { // bit 1 is more than bit 0, indicates duplicate
            duplicate_data += check_mask;
            parr = arr1;
            arr_len = arr1_len;
        } else {
            parr = arr0;
            arr_len = arr0_len;
        }
    }

    printf("The duplicated data is: %d\n", duplicate_data);
    
    return 0;
}
