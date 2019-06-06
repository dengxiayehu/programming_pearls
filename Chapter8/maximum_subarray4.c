#include <stdio.h>

#define NELEM(arr) (sizeof(arr)/sizeof(arr[0]))

int max(int a, int b)
{
    return a > b ? a : b;
}

int main(int argc, char const *argv[])
{
    int array[] = { 31, -41, 59, 26, -53, 58, 97, -93, -23, 84 };
    const int n = NELEM(array);
    int maxsofar = 0;
    int maxendinghere = 0;
    int i;

    for (i = 0; i < n; i++) {
        // 从以索引i为子数组结尾的角度去考虑问题。
        maxendinghere = max(maxendinghere+array[i], array[i]);
        printf("maxendinghere i:%d is: %d\n", i, maxendinghere);
        if (maxendinghere > maxsofar) {
            maxsofar = maxendinghere;
        }
    }

    printf("maxsofar=%d\n", maxsofar);

    return 0;
}
