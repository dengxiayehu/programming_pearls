### 算法设计技术

这一章应该是比较重要的一章，开始讲怎么来设计高效、有些复杂的算法了。

开始抛出的一个问题，有如下数组：  
`31 -41 59 26 -53 58 97 -93 -23 84`  
求其最大子数组和。

### P1 O(n^3)

首先是最直观的算法，但效率低得离谱，时间复杂度是O(n^3)  
```
#include <stdio.h>

#define NELEM(arr) (sizeof(arr)/sizeof(arr[0]))

int main(int argc, char const *argv[])
{
    int array[] = { 31, -41, 59, 26, -53, 58, 97, -93, -23, 84 };
    const int n = NELEM(array);
    int i;
    int j;
    int k;
    int maxsofar = 0;

    // Iterate each range with index [i, j], find the maximum subarray.
    for (i = 0; i < n; i++) {
        for (j = i; j < n; j++) {
            int sum = 0;

            // Calculate the sum between [i, j].
            for (k = i; k <= j; k++) {
                sum += array[k];
            }

            if (sum > maxsofar) {
                maxsofar = sum;
            }
        }
    }

    printf("maxsofar=%d\n", maxsofar);

    return 0;
}
```
> 输出结果：  
/maximum_subarray_run  
maxsofar=187

### P2 O(n^2)
上面这个有个明显的优化点，就是在计算[i .. j]区间的和时，可以借助[i .. j-1]的结果加上array[j]的值即可。基于这个思想，我们可以写出时间复杂度为O(n^2)规模的算法。
```
#include <stdio.h>

#define NELEM(arr) (sizeof(arr)/sizeof(arr[0]))

int main(int argc, char const *argv[])
{
    int array[] = { 31, -41, 59, 26, -53, 58, 97, -93, -23, 84 };
    const int n = NELEM(array);
    int i;
    int j;
    int maxsofar = 0;

    // Iterate each range of index [i..j], find the maximum subarray.
    for (i = 0; i < n; i++) {
        int sum = 0;

        for (j = i; j < n; j++) {
            // Calculate the sum of index [i..j].
            // Sum of index [i..j] is equal to sum[i..j-1]+array[j].
            sum += array[j];

            if (sum > maxsofar) {
                maxsofar = sum;
            }
        }
    }

    printf("maxsofar=%d\n", maxsofar);

    return 0;
}
```

### P3 另一个O(n^2)级别算法

算法思想：建立一个临时数组，里面存储的值是sum[0 ..i ]元素的和。这样在计算子数组[i .. j]的和时，可以这么计算：cumarr[j]-cumarr[i-1]。但这里有个问题，就是在求子数组[0 .. j]范围的和时，使用cumarr[j]-cumarr[-1]。我们预先设置cumarr[-1]=0。

```
#include <stdio.h>
#include <stdlib.h>

#define NELEM(arr) (sizeof(arr)/sizeof(arr[0]))

int main(int argc, char const *argv[])
{
    int array[] = { 31, -41, 59, 26, -53, 58, 97, -93, -23, 84 };
    const int n = NELEM(array);
    int i;
    int j;
    int maxsofar = 0;
    int* sum_array, *cumarr;

    sum_array = (int*)malloc(sizeof(int)*(n+1));
    if (!sum_array) {
        perror("malloc failed");
        return 1;
    }

    cumarr = sum_array+1;
    cumarr[-1] = 0; // that is sum_array[0]
    for (i = 0; i < n; i++) {
        cumarr[i] = cumarr[i-1]+array[i];
    }

    printf("the cumarr is: ");
    for (i = 0; i < n; i++) {
        printf("%d ", cumarr[i]);
    }
    printf("\n");

    for (i = 0; i < n; i++) {
        for (j = i; j < n; j++) {
            // Get the sum of [i..j].
            int sum = cumarr[j]-cumarr[i-1];
            if (sum > maxsofar) {
                maxsofar = sum;
            }
        }
    }

    printf("maxsofar=%d\n", maxsofar);

    free(sum_array);

    return 0;
}
```
> 输出结果：  
./maximum_subarray_run2  
the cumarr is: 31 -10 49 75 22 80 177 84 61 145   
maxsofar=187  

可以看出，这个平房级别的算法明显没有之前的那个好，要有一个预处理。

### P4 nlog(n)规模的算法

第一个次平方的算法，差不多是一个可以实用的算法了。分治思想的应用。将一个数组对半分，那么最大子数组和必然在左半部分、右半部分或者横跨左右两部分的中间部分。其中，对于中间部分我们可以单独拎出来求，而左右半部分中最大子数组和的求取又和大问题类似，可以应用相同的策略。

分治算法的基本思想是将一个规模为N的问题分解为K个规模较小的子问题，这些子问题相互独立且与原问题性质相同。求出子问题的解，就可得到原问题的解。即一种分目标完成程序算法，简单问题可用二分法完成。

注意要将分治思想与动态规划思想区分开，动态规划算法与分治法类似，其基本思想也是将待求解问题分解成若干个子问题，先求解子问题，然后从这些子问题的解得到原问题的解。与分治法不同的是，适合于用动态规划求解的问题，经分解得到子问题往往不是互相独立的。若用分治法来解这类问题，则分解得到的子问题数目太多，有些子问题被重复计算了很多次。如果我们能够保存已解决的子问题的答案，而在需要时再找出已求得的答案，这样就可以避免大量的重复计算，节省时间。我们可以用一个表来记录所有已解的子问题的答案。不管该子问题以后是否被用到，只要它被计算过，就将其结果填入表中。这就是动态规划法的基本思路。

```
#include <stdio.h>

#define NELEM(arr) (sizeof(arr)/sizeof(arr[0]))

#ifndef MAX
#define MAX(a, b) ((a)>(b)?(a):(b))
#endif

int max3(int a, int b, int c)
{
    return MAX(a, MAX(b, c));
}

int maximum_divide(int array[], int l, int r)
{
    int mid;
    int lmax;
    int rmax;
    int sum;
    int i;

    if (l > r) {
        // Empty array, return maximum 0.
        return 0;
    }

    if (l == r) {
        // Only 1 element, return maximum(0, array[l]).
        return MAX(0, array[l]);
    }

    mid = l + (r-l)/2;
    // Get the lmax of the subarray cross the middle.
    lmax = sum = 0;
    for (i = mid; i >= l; i--) { // index mid to l
        sum += array[i];
        if (sum > lmax) {
            lmax = sum;
        }
    }
    // Get the rmax of the subarray cross the middle.
    rmax = sum = 0;
    for (i = mid+1; i <= r; i++) { // index mid+1 to r
        sum += array[i];
        if (sum > rmax) {
            rmax = sum;
        }
    }

    // 为避免宏定义展开的副作用，这里使用max3函数来找三个数中的最大值。
    return max3(lmax+rmax, maximum_divide(array, l, mid-1), maximum_divide(array, mid+1, r));
}

int main(int argc, char const *argv[])
{
    int array[] = { 31, -41, 59, 26, -53, 58, 97, -93, -23, 84 };
    const int n = NELEM(array);
    int maxsofar = 0;

    maxsofar = maximum_divide(array, 0, n-1);
    printf("maxsofar=%d\n", maxsofar);

    return 0;
}
```
> 输出结果：  
maxsofar=187  

这道题和leetcode 53题类似，我们来看一下：  
给定一个整数数组 nums ，找到一个具有最大和的连续子数组（子数组最少包含一个元素），返回其最大和。

### Px **示例:**

> 输入: [-2,1,-3,4,-1,2,1,-5,4],  
输出: 6  
解释: 连续子数组 [4,-1,2,1] 的和最大，为 6。  

我使用Java语言编写的答案如下：
``` java
public class Solution {
  public int maxSubArray(int[] nums) {
    if (nums.length == 0) {
      return 0;
    }
    return maxSubArray(nums, 0, nums.length - 1);
  }
  
  private static int maxSubArray(int[] nums, int left, int right) {
    if (left >= right) {
      return nums[left];
    }
    final int mid = (left + right) / 2;
    int maxLeftSubArray = maxSubArray(nums, left, mid - 1);
    int maxRightSubArray = maxSubArray(nums, mid + 1, right);
    int maxBorderSum = nums[mid];
    for (int i = mid - 1, tmp = maxBorderSum; i >= left; i--) {
      tmp += nums[i];
      maxBorderSum = Math.max(maxBorderSum, tmp);
    }
    for (int i = mid + 1, tmp = maxBorderSum; i <= right; i++) {
      tmp += nums[i];
      maxBorderSum = Math.max(maxBorderSum, tmp);
    }
    return max3sum(maxBorderSum, maxLeftSubArray, maxRightSubArray);
  }
  
  private static int max3sum(int a, int b, int c) {
    return Math.max(a, Math.max(b, c));
  }
}
```
这个算法可以求出子数组和最大就是负数的情况。而不像之前介绍的算法，只能求的最大子数组和为大于0的情况。

> *说明：使用分治算法来求最大子数组问题时，还有优化的空间，时间复杂度可以进一步减少到O(n)，具体参考习题8.8。*

### P5 动态规划思想的O(n)规模算法

这也是此类问题的最高效算法了。

思想：计算以当前索引i为结尾的最大子数组和，有两种情况：
1. 以当前索引i为结尾的子数组和是 以索引i-1为结尾的最大子数组和+array[i]
2. 以当前索引i为结尾的子数组和就是array[i]

```
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
```
> 输出结果：   
./maximum_subarray4_run  
maxendinghere i:0 is: 31  
maxendinghere i:1 is: -10  
maxendinghere i:2 is: 59  
maxendinghere i:3 is: 85  
maxendinghere i:4 is: 32  
maxendinghere i:5 is: 90  
maxendinghere i:6 is: 187  
maxendinghere i:7 is: 94  
maxendinghere i:8 is: 71  
maxendinghere i:9 is: 155  
maxsofar=187  

---

以下内容来自网页：<https://segmentfault.com/a/1190000008730320>。  
非常推荐这篇文章，介绍DP的思想，并以算法导论上的“切割刚条”的例子进行实际探索。

动态规划：dynamic programming，programming指的是一种表格法，而非指coding。

DP和分治的不同之处：
+ 分治步骤：（例如归并排序，求最大子数组和问题）
    + 将问题划分为互不相交的子问题
    + 递归地求解子问题
    + 组合子问题的解，求出原问题的解
+ DP：
    + 应用于子问题重叠的情况，即不同的子问题具有公共的子子问题（子问题的求解是递归的，将其划分为更小的子子问题）
    + 这种情况下分治会做很多不必要的工作，会反复求解那些公共子问题
    + 而DP对每个子子问题只求解一次，将其解保存在一个表格中，无需每次都重新计算，避免重复工作

DP通常用来求解最优化问题（optimization problem）
+ 这种问题可以有很多可行的解，每个解都有一个值，希望找到最优值（最大或者最小）的解。称这样的解为问题的一个最优解（an optimal solution），而不是最优解（the optimal solution），因为可能有多个解都达到最优。

DP的四个步骤：
1. 刻画一个最优解的结构特征
2. 递归地定义最优解的值
3. 计算最优解的值，通常采用自底向上法
4. 利用计算出的信息构造一个最优解

前三步是DP求解的基础。若仅需要一个最优解，而非解本身，可忽略第4步。若需要第4步，有时需要执行第3步的过程中维护一些额外的信息，以便构造一个最优解。

### P6 切割钢条版本1：最初的解法，穷举每一种切割方法。（分治思想）

```
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
```
> 输出结果：  
max_profit=30

基于上面的算法，有一个比较直观的优化就是，在每个切割点，保持左侧的钢条在切割完之后不再切割，而进一步递归切割右侧的钢条。
### P7 切割钢条 较P6稍微提升一点效率 （分治思想）

```
#include <stdio.h>

#define NELEM(arr) (sizeof(arr)/sizeof(arr[0]))

int max(int a, int b)
{
    return a > b ? a : b;
}

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
        // 依次判断钢条在每个切割点切割后，整体的收益=左侧固定不动的收益+右侧切割的收益和。
        max_profit = max(max_profit, p[i-1] + cut_rob(p, n-i));
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
```
> 输出结果：  
max_profit=30

上面的两个处理钢条切割的算法效率太低了，因为它在把大问题分为小问题的时候，重复求解了很多相同的小问题。我们可以将这些中间计算的小问题的解保存下来，下次再来求解时，直接从“表格”中拿出来即可。  
拿空间换时间的典型应用。  
DP有两种等价的实现方式：带备忘录的自顶向下法（top-down with memoization） & 自底向上法（bottom-up method）。

### P8 带备忘录的钢条切割算法（动态规划思想）自顶向下法
+ 此方法按照自然的递归形式编写过程，但是过程会保存每个子问题的解（通常保存在一个数组或者散列表中）
+ 当需要一个子问题的解时，过程首先检查是否已经保存过此解
    + 如果是，则直接返回保存的值，从而节省了计算时间
    + 否则，按照通常方式计算这个子问题

```
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
```
> 输出结果：  
max_profit=30

### P9 带备忘录的钢条切割算法（动态规划思想）自底向上法

+ 该方法一般需要恰当定义子问题“规模”的概念，使得任何子问题的求解都只依赖于“更小的”子问题的求解。
+ 因而可以将子问题规模排序，按从小到大的顺序求解。
+ 当求解某个子问题时，所依赖的那些更小的子问题都已经求解完毕了，结果已经保存
+ 每个子问题只求解一次，当求解它时（也是第一次遇见它），所有的前提子问题都已经求解完成

```
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
```
> 输出结果：  
max_profit=30

两种方法得到的算法具有相同的渐进运行时间
+ 仅有的差异是在某些特殊情况下，自顶向下方法并未真正递归地考察所有可能的子问题。
+ 由于没有频繁的递归调用开销，自底向上的复杂度函数通常具有更小的系数。

---

### 习题

### 8.5
分析：要想访问cumarr[-1]，即访问cumarr[0]元素之前的那个元素。由于数组的下标是从0开始，所以我们需要将cumarr[-1]指向某个C语言数组中索引>=0的位置。

    int x[4] = { 1, 2, 3, 4};
    int *cumarr = x + 1;
    cumarr[-1] = x[0]

具体参考程序P3。

### 8.6
分析：我一直弄不清Saxe算法相对于位算法更优的地方。

假设一个大文件里只有100万个数字，但均在范围0~9中。Saxe算法指出，要找到这个大文件里的任意一个重复的数字，我们只要准备一个11个元素的数组，然后只要处理这个大文件的前11个数字即可，后面的数字都不用考虑了。因为在处理过的11个数字中，肯定有一个重复的数字。

但是这个过程应该还可以更短，在使用位算法判断某个数已经出现了时，就直接报告找到了重复的数字，兴许都用不着取11个数字了。

### 8.8
分析：我觉得这个题非常好，能够进一步发觉分治算法在求解最大子数组问题中的信息，尽可能地将后一次分治多利用前面已经求得出来的结果。具体是：  
在求最大子数组横跨于左右子区间情况时，找到中间的最大子数组两端的位置。在紧接着分治，可分别在这个最大子数组的前部和后部进行，而不用和当前已经计算的中间最大子数组重叠。

```
#include <stdio.h>

#define NELEM(arr) (sizeof(arr)/sizeof(arr[0]))

int max(int a, int b)
{
    return a > b ? a : b;
}

int max3(int a, int b, int c)
{
    return max(a, max(b, c));
}

// 时间复杂度为O(n)的分治算法求取最大子数组问题。
int maximum_divide(int array[], int l, int r)
{
    int m;
    int lmax, lidx, rmax, ridx, sum;
    int i;

    if (l > r) { // array is empty
        return 0;
    } else if (l == r) {
        return max(array[l], 0);
    }

    m = l+(r-l)/2;
    lmax = sum = 0;
    lidx = m;
    for (i = m; i >= l; i--) {
        sum += array[i];
        if (sum > lmax) {
            lmax = sum;
            lidx = i;
        }
    }
    rmax = sum = 0;
    ridx = m+1;
    for (i = m+1; i <= r; i++) {
        sum += array[i];
        if (sum > rmax) {
            rmax = sum;
            ridx = i;
        }
    }

    return max3(lmax+rmax,
                maximum_divide(array, l, lidx-1),
                maximum_divide(array, ridx+1, r));
}

int main(int argc, char const *argv[])
{
    int array[] = { 31, -41, 59, 26, -53, 58, 97, -93, -23, 84 };
    const int n = NELEM(array);
    int maxsofar;

    maxsofar = maximum_divide(array, 0, n-1);
    printf("maxsofar=%d\n", maxsofar);

    return 0;
}
```
> 输出结果：  
maxsofar=187

### 8.9
分析：最大子数组问题，若整个数组里面都是负数，那么现在不是返回0了， 而是返回数值最大的那个负数。这个算法可以参考之前的Px程序。

也可以修改之前的P5程序，将 maxsofar = INT_MIN 或 maxsofar = array[0]，具体如下：
```
#include <stdio.h>
#include <limits.h>

#define NELEM(arr) (sizeof(arr)/sizeof(arr[0]))

int max(int a, int b)
{
    return a > b ? a : b;
}

int main(int argc, char const *argv[])
{
    // int array[] = { 31, -41, 59, 26, -53, 58, 97, -93, -23, 84 };
    int array[] = { -9, -1, -8, -2, -3 };
    const int n = NELEM(array);
    int maxsofar = array[0]; // or maxsofar = INT_MIN;
    int maxendinghere = 0;
    int i;

    for (i = 1 /* or i = 0 when maxsofar = INT_MIN*/; i < n; i++) {
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
```
> 输出结果：（输入数组为{ -9, -1, -8, -2, -3 }）  
./maximum_subarray_dp1_run  
maxendinghere i:1 is: -1  
maxendinghere i:2 is: -8  
maxendinghere i:3 is: -2  
maxendinghere i:4 is: -3  
maxsofar=-1  

确实如预期的那样，若数组都是负数的情况，返回的是最大元素的值。

### 8.10 查找总和最接近某个数t的子向量

基于程序P3，我们将遍历的所有子数组和与t进行比较，求得最接近的子数组，时间复杂度是O(n)。

```
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define FLT_MAX 3.402823466e+38F

#define NELEM(arr) (sizeof(arr)/sizeof(arr[0]))

int main(int argc, char const *argv[])
{
    float array[] = { 31, -41, 59, 26, -53, 58, 97, -93, -23, 84 };
    const int n = NELEM(array);
    float tmp[n+1], *curmum = tmp+1;
    float t;
    float nearest = FLT_MAX;
    int i;
    int j;
    int l;
    int r;

    for ( ; ; ) {
        printf("input t: ");
        if (scanf("%f", &t) == EOF) {
            break;
        }

        curmum[-1] = 0;
        for (i = 0; i < n; i++) {
            // curmum[i] is the sum of elements with index -1 to i.
            curmum[i] = curmum[i-1]+array[i];
        }

        for (i = 0; i < n; i++) {
            for (j = i; j < n; j++) {
                float sum = curmum[j]-curmum[i-1];
                
                if (fabs(sum-t) < fabs(nearest-t)) {
                    nearest = sum;
                    l = i;
                    r = j;
                }
            }
        }

        printf("the nearest sum is: %f [%d .. %d]\n", nearest, l, r);
    }

    return 0;
}
```
> 输出结果：  
./nearest_run   
input t: 10  
the nearest sum is: 9.000000 [4 .. 7]  
input t: -11  
the nearest sum is: -10.000000 [0 .. 1]  

若求子数组和最接近0的子向量，我们可以将时间复杂度降低到nlogn级别。

给出[-3, 1, 1, -3, 5]，返回[0, 2]，[1, 3]， [1, 1]， [2, 2] 或者 [0, 4]。  
假设[0-2]和[0-4]的sum值的差值最小，那么结果为index[3,4]。

看到nlogn级别的时间复杂度，直觉上应该用二分之类的算法，但这里没有看出来二分的意义，所以想只能想排序了，我们知道连续数组的和其实就是前缀和之间的差，而要求和最接近于0，也就是说，两个前缀和要最接近，那么直接前缀和排序，相邻两项比较即可。

```
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define NELEM(arr) (sizeof(arr)/sizeof(arr[0]))

typedef struct Item {
    int sum;
    int i;
} Item;

int compar(const void *arg1, const void *arg2)
{
    return ((const Item *)arg1)->sum - ((const Item *)arg2)->sum;
}

int min(int a, int b)
{
    return a < b ? a : b;
}

int max(int a, int b)
{
    return a > b ? a : b;
}

int main(int argc, char const *argv[])
{
    //int array[] = { 31, -41, 59, 26, -53, 58, 97, -93, -23, 84 };
    int array[] = { -3, 1, 1, -3, 5 };
    //int array[] = { -1, -1, 2 };
    //int array[] = { -99 };
    const int n = NELEM(array);
    Item tmp[n+1], *curmum = tmp+1;
    int i;
    int nearest;
    int l;
    int r;

    curmum[-1].sum = 0;
    for (i = 0; i < n; i++) {
        // curmum[i] is the sum of elements with index -1 to i.
        curmum[i].sum = curmum[i-1].sum+array[i];
        curmum[i].i = i;
    }

    // Sort curmum array. nlogn
    qsort(curmum, n, sizeof(Item), compar);

    // Print the sorted curmum.
    for (i = 0; i < n; i++) {
        printf("%d,%d ", curmum[i].sum, curmum[i].i);
    }
    printf("\n");

    // n
    nearest = abs(curmum[0].sum);
    l = r = 0;
    for (i = 0; i < n; i++) {
        if (curmum[i].sum == 0) { // find the exact value
            nearest = 0;
            l = 0;
            r = curmum[i].i;
            break;
        } else {
            if (i < n - 1) {
                int diff = abs(curmum[i].sum - curmum[i+1].sum);

                if (diff < nearest) {
                    nearest = diff;
                    l = min(curmum[i].i, curmum[i+1].i)+1; // note, add 1
                    r = max(curmum[i].i, curmum[i+1].i);
                }
            }
        }
    }

    printf("the nearest abs value is: %d, %d..%d\n", nearest, l, r);

    return 0;
}
```
> 输出结果：  
./nearest_0_run  
-4,3 -3,0 -2,1 -1,2 1,4   
the nearest abs value is: 1, 1..3  

### 8.11

这道题有两种解法：  
1. 建立每个收费站两两之间的费用表，需要使用二维数组。空间复杂度O(n^2)。
2. 通过累加和数组，收费站i和j之间的费用就是cumarr[j]-cumarr[i]。空间复杂度O(n)。

| 收费站0（入口） | 收费站1 | 收费站2 | 收费站3 |
| :----: | :----: | :----: | :----: |
| 0 | 7 | 6 | 8 |

7个收费站，共6段公路，每一段的费用如下：  
第1段路程的费用：7  
第2段路程的费用：6  
第3段路程的费用：8  

另i为任意路段的起始收费站，j为任意路段的终止收费站。我们可以根据上表，手动建立所有[i..j]路段的费用表，然后根据输入的路段自动查表得出费用；也可以使用cumarr数组。

```
#include <stdio.h>

#define N 4

int main(int argc, char const *argv[])
{
    const int fee[N][N] = { // i <= j
        { 0, 7, 13, 21 }, // i = 0
        { 0, 0,  6, 14 }, // i = 1
        { 0, 0,  0,  8 }, // i = 2
        { 0, 0,  0,  0 }  // i = 3
    };
    int i;
    int j;

    for ( ; ; ) {
        printf("Input i j: ");
        if (scanf("%d %d", &i, &j) == EOF) {
            break;
        }

        if (i > j) {
            int t = i;
            i = j;
            j = t;
        }

        printf("the fee is: %d\n", fee[i][j]);
    }

    return 0;
}
```
> 运行结果：  
Input i j: 1 1  
the fee is: 0  
Input i j: 1 3  
the fee is: 14  
Input i j: 3 1  
the fee is: 14  

我们再使用前缀和数组来求解。

```
#include <stdio.h>

#define N 3

int main(int argc, char const *argv[])
{
    const int fee[N] = { 7, 6, 8 };
    int tmp[N+1], *cumarr = tmp+1;
    int i;
    int j;
    int res;

    cumarr[-1] = 0;
    for (i = 0; i < N; i++) {
        cumarr[i] = cumarr[i-1]+fee[i];
        printf("%d ", cumarr[i]);
    }
    printf("\n");

    for ( ; ; ) {
        printf("Input i j: ");
        if (scanf("%d %d", &i, &j) == EOF) {
            break;
        }

        if (i == j) {
            res = 0;
        } else {
            if (i > j) {
                int t = i;
                i = j;
                j = t;
            }

            i--;
            j--;

            res = cumarr[j]-cumarr[i];
        }

        printf("the fee is: %d\n", res);
    }

    return 0;
}
```
> 输出结果：  
./highway_run1   
7 13 21   
Input i j: 1 1  
the fee is: 0  
Input i j: 1 3  
the fee is: 14  
Input i j: 3 1  
the fee is: 14  
Input i j: 0 3  
the fee is: 21  

### 8.12

这题很有启发性。注意前提：原始数组都被初始化了0。  
设数组大小为N，那么我们需要N+1的空间。cumarr需要N的空间。

下面程序外循环总共循环N次，每次将i~n-1的区间里的元素值加1。O(n^2)的算法如下：

    for (i = 0; i < N; i++) {
        for (j = i; j < N; j++) {
            x[j] += 1;
        }
    }

下面使用O(n)级别的算法来解决：
```
#include <stdio.h>

#define N 8

#define NELEM(arr) (sizeof(arr)/sizeof(arr[0]))

int main(int argc, char const *argv[])
{
    int x[N+1];
    int tmp[N+1], *cumarr = tmp+1;
    int i;

    for (i = 0; i < NELEM(x); i++) {
        x[i] = 0;
    }

    cumarr[-1] = 0;
    for (i = 0; i < N; i++) {
        cumarr[i] = cumarr[-1]+x[i];
    }

    for (i = 0; i < N; i++) {
        cumarr[N-1] += 1;
        cumarr[i-1] -= 1;
    }

    // Reconstruct the array x.
    for (i = N-1; i >= 0; i--) {
        x[i] = x[i+1]+cumarr[i];
    }

    for (i = 0; i < N; i++) {
        printf("%d ", x[i]);
    }
    printf("\n");

    return 0;
}
```
> 输出结果：  
1 2 3 4 5 6 7 8  
输出结果与预期一致。

### 8.13

二维数组最大子矩形数组问题。

```
#include <stdio.h>
#include <limits.h>

#define ROW 3
#define COL 4

int max(int a, int b)
{
    return a > b ? a : b;
}

int main(int argc, char const *argv[])
{
    // const int array[ROW][COL] = {
    //     { 0,  1,  2,  3 },
    //     { 4,  5,  6,  7 },
    //     { 8,  9, 10, 11 },
    // };
    const int array[ROW][COL] = {
        { 0,  -1,  -2,  -3 },
        { -4,  5,  6,  -7 },
        { -8,  -9, -10, -11 },
    };
    int tmp[ROW+1][COL] = {{1, 2, 3, 4}};
    int (*cumarr)[COL] = &tmp[1];
    int row_sum[COL];
    int i;
    int ii;
    int j;
    int maxsofar;
    int maxendinghere;
    int max_rect = INT_MIN;

    // Init cumarr.
    for (j = 0; j < COL; j++) {
        cumarr[-1][j] = 0;
    }
    for (j = 0; j < COL; j++) {
        for (i = 0; i < ROW; i++) {
            cumarr[i][j] = cumarr[i-1][j]+array[i][j];
        }
    }

    // Print the cumarr.
    printf("the cumarr is: \n");
    for (i = 0; i < ROW; i++) {
        for (j = 0; j < COL; j++) {
            printf("%d ", cumarr[i][j]);
        }
        printf("\n");
    }

    // Find the max sub rect.
    for (i = 0; i < ROW; i++) {
        for (ii = i; ii < ROW; ii++) {
            for (j = 0; j < COL; j++) {
                row_sum[j] = cumarr[ii][j]-cumarr[i-1][j];
            }

            maxsofar = row_sum[0];
            maxendinghere = 0;
            for (j = 0; j < COL; j++) {
                maxendinghere = max(maxendinghere+row_sum[j], row_sum[j]);
                maxsofar = max(maxsofar, maxendinghere);
            }

            max_rect = max(maxsofar, max_rect);
        }
    }

    printf("max_rect is: %d\n", max_rect);

    return 0;
}
```
> 输出结果：  
the cumarr is:  
0 -1 -2 -3  
-4 4 4 -10  
-12 -5 -6 -21  
max_rect is: 11 

上面这个程序，出发点是好的，想预先计算出来每行数据在行上面的累加和。使用到了O(n^2)的零时空间，避免了每次的计算。但是整个程序的时间复杂度是O(n^3)的，省的这点时间没有多大效益。

```
#include <stdio.h>
#include <limits.h>

#define ROW 3
#define COL 4

int max(int a, int b)
{
    return a > b ? a : b;
}

int main(int argc, char const *argv[])
{
    // const int array[ROW][COL] = {
    //     { 0,  1,  2,  3 },
    //     { 4,  5,  6,  7 },
    //     { 8,  9, 10, 11 },
    // };
    const int array[ROW][COL] = {
        { 0,  -1,  -2,  -3 },
        { -4,  5,  6,  -7 },
        { -8,  -9, -10, -11 },
    };
    int row_sum[COL];
    int i;
    int ii;
    int j;
    int maxsofar;
    int maxendinghere;
    int max_rect = INT_MIN;

    // Find the max sub rect.
    for (i = 0; i < ROW; i++) {
        for (j = 0; j < COL; j++) {
            row_sum[j] = 0;
        }

        for (ii = i; ii < ROW; ii++) {
            for (j = 0; j < COL; j++) {
                row_sum[j] += array[ii][j];
            }

            maxsofar = row_sum[0];
            maxendinghere = 0;
            for (j = 0; j < COL; j++) {
                maxendinghere = max(maxendinghere+row_sum[j], row_sum[j]);
                maxsofar = max(maxsofar, maxendinghere);
            }

            max_rect = max(maxsofar, max_rect);
        }
    }

    printf("max_rect is: %d\n", max_rect);

    return 0;
}
```
> 输出结果：  
./max_rect_array_run1  
max_rect is: 11  

### 8.14

求m个元素总和最接近0的和。x[i]+...+x[i+m]最接近0的整数i（0 <= i < n-m）。

```
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <assert.h>

#define NELEM(arr) (sizeof(arr)/sizeof(arr[0]))

int main(int argc, const char *argv[])
{
    int array[] = { -3, 1, 1, -3, 5 };
    const int n = NELEM(array);
    int tmp[n+1], *curmum = tmp+1;
    int i;
    int m;
    int min_gap;
    int min_i;

    printf("array: ");
    for (i = 0; i < n; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");

    // Init curmum array.
    printf("curmum: ");
    curmum[-1] = 0;
    for (i = 0; i < n; i++) {
        curmum[i] = curmum[i-1]+array[i];
        printf("%d ", curmum[i]);
    }
    printf("\n");

    for ( ; ; ) {
        printf("input m: "); // m stands for m numbers
        if (scanf("%d", &m) == EOF) {
            break;
        }
        assert(m > 0 && m <= n);

        m--;

        min_gap = INT_MAX;
        for (i = 0; i < n-m; i++) {
            int abs_sum = abs(curmum[i+m]-curmum[i-1]);
            if (abs_sum < min_gap) {
                min_gap = abs_sum;
                min_i = i;
            }
        }
        printf("min_gap=%d, min_i=%d\n", min_gap, min_i);
    }

    return 0;
}
```
> 输出结果：
./nearest_m_0_run  
array: -3 1 1 -3 5  
curmum: -3 -2 -1 -4 1  
input m: 5  
min_gap=1, min_i=0  
input m: 4  
min_gap=4, min_i=0  
input m: 3  
min_gap=1, min_i=0  
input m: 2  
min_gap=2, min_i=0  
input m: 1  
min_gap=1, min_i=1  






