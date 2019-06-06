#include <stdio.h>

#define NELEM(arr) (sizeof(arr)/sizeof(arr[0]))

int max(int a, int b)
{
    return a > b ? a : b;
}

int cut_rob(int p[], int n, int r[])
{
    int i;
    int max_profit;

    if (n == 0) {
        return 0;
    }

    // 假如长度为n的钢条的最大收益已经计算过了，则直接取出之前的计算结果。
    if (r[n-1] > 0) {
        return r[n-1];
    }

    // 先假设钢条在不切割的时候收益是最大的。
    max_profit = p[n-1];
    for (i = 1; i < n; i++) {
        // 依次判断钢条在每个切割点切割后，整体的收益=左侧固定不动的收益+右侧切割的收益和。
        max_profit = max(max_profit, p[i-1] + cut_rob(p, n-i, r));
    }

    // 将长度为n的钢条的最大收益结果保存到r中的对应位置。
    r[n-1] = max_profit;

    return max_profit;
}

int main(int argc, char const *argv[])
{
    int p[] = { 1, 5, 8, 9, 10, 17, 17, 20, 24, 30};
    int r[NELEM(p)] = { 0 };
    const int n = NELEM(p);
    int max_profit;

    max_profit = cut_rob(p, n, r);
    printf("max_profit=%d\n", max_profit);

    return 0;
}
