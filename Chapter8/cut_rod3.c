#include <stdio.h>

#define NELEM(arr) (sizeof(arr)/sizeof(arr[0]))

int max(int a, int b)
{
    return a > b ? a : b;
}

int main(int argc, char const *argv[])
{
    int p[] = { 1, 5, 8, 9, 10, 17, 17, 20, 24, 30};
    int r[NELEM(p)] = { 0 };
    const int n = NELEM(p);
    int max_profit;
    int i;
    int j;

    // 从短到长处理钢条切割。
    for (j = 1; j <= n; j++) {
        // 处理每种情况下的钢条切割。
        max_profit = p[j-1]; // 先处理长度为j的钢条都不切割的情况
        for (i = 1; i < j; i++) { // 再次考虑切割长度为j钢条的情况
            max_profit = max(max_profit, p[i-1]+r[j-i-1]);
        }
        // 保存长度为j的钢条的最大收益情况。
        r[j-1] = max_profit;
    }
    printf("max_profit=%d\n", max_profit);

    return 0;
}
