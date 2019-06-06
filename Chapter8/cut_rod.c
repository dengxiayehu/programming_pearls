#include <stdio.h>

#define NELEM(arr) (sizeof(arr)/sizeof(arr[0]))

int max(int a, int b)
{
    return a > b ? a : b;
}

// 这个算法特别低效，时间复杂度是O(2^n)指数阶规模的算法。
// 在每个切割点，切割了好几次。
int cut_rob(int p[], int n)
{
    int i;
    int max_profit;

    if (n == 0) {
        return 0;
    }

    // 先假设钢条在不切割的时候收益是最大的。
    max_profit = p[n-1];
    for (i = 1; i < n; i++) {
        // 依次判断钢条在每个切割点切割后，整体的收益=左右两端的收益和。
        max_profit = max(max_profit, cut_rob(p, i) + cut_rob(p, n-i));
    }

    return max_profit;
}

int main(int argc, char const *argv[])
{
    int p[] = { 1, 5, 8, 9, 10, 17, 17, 20, 24, 30};
    const int n = NELEM(p);
    int max_profit;

    max_profit = cut_rob(p, n);
    printf("max_profit=%d\n", max_profit);

    return 0;
}
