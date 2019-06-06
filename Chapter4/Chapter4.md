### 编写正确的程序

前面三章已经介绍了在正式动手编程前需要进行的准备工作，这一章对编程提出了一些宝贵的方法，来确保所写的程序是正确的。

循环不变式的重要性：在循环开始前循环不变式为真，那么在过程中一直保持此循环不变式，那么必然在循环结束时，循环不变式仍为真。

停机证明：确保你所写的循环能够在运行有限次步骤后退出。

3x+1问题，伪代码如下：
```
while x != 1
    x is even
        x = x/2
    else
        x = 3x + 1
```
这个程序在运行有限次后肯定会停机。
```
#include <stdio.h>

int main(int argc, char const* argv[])
{
    int x;

    for ( ; ; ) {
        printf("Input x: ");
        if (scanf("%d", &x) == EOF) {
            break;
        }

        while (x != 1) {
            if (x&1) { // x is odd
                x = (x << 1) + x + 1;
            } else { // x is even
                x >>= 1;
            }
            printf("%d ", x);
        }

        printf("\n");
    }
    return 0;
}
```
> 输出结果：
Input x: 3  
10 5 16 8 4 2 1   
Input x: 4  
2 1   
Input x: 5  
16 8 4 2 1   
Input x: 6  
3 10 5 16 8 4 2 1   
Input x: 9  
28 14 7 22 11 34 17 52 26 13 40 20 10 5 16 8 4 2 1   

我们来书写一下那个吹毛求疵的二分搜索验证程序：
```
int bsearch(int t, int x[], int n)
/* precondition: x[0] <= x[1] <= x[2] ... <= x[n-1]
   postcondition: 
        result == -1    => t not present in x
        0 <= result < n => x[result] = t
*/
{ mustbe(0, n - 1) }    // 初始条件
l = 0; u = n - 1
{ mustbe(l, u) }        // 循环不变式
loop
    { mustbe(l, u) }    // 循环不变式
    if l > u
        { l > u && mustbe(l, u) }   // 范围为空，循环不变式不满足，需退出循环
        { t is not in the array }
        p = -1; break
    { mustbe(l, u) && l <= u }      // 此处，循环不变式必定满足
    m = l + (u - l)/2
    { mustbe(l, u) && l <= m <= u }
    case
        x[m] < t:
                { mustbe(l, u) && cantbe(0, m) }
                { mustbe(m+1, u) }  // 根据情况，更新循环不变式继续为真
                l = m + 1
                { mustbe(l, u) }    // 循环不变式保持
        x[m] == t:
                { x[m] == t }
                p = m; break
        x[m] > t:
                { mustbe(l, u) && cantbe(m, n-1) }
                { mustbe(l, m-1) }
                u = m - 1
                { mustbe(l, u) }    // 循环不变式保持
    { mustbe(l, u) }                // 迭代完成一次，循环不变式继续保持
```

### P1 二分查找第一次出现的数

分析：可以先进行二分查找，然后若是找到该数，再判断其左侧数字是否相同，若是，则索引往左移动，直到找到第一个。  
更进一步，持续进行二分搜索，直到找到的数左侧和其不同就返回。  
这个方法相对于第一个算法而言，可以对重复的数字也进行二分搜索。

```
#include <stdio.h>

#define NELEM(arr) sizeof(arr)/sizeof(arr[0])

int binary_search_1st(int arr[], int n, int key)
{
    int l;
    int u;

    if (!arr || n <= 0) { // Invalid param.
        return -1;
    }

    l = 0;
    u = n - 1;
    while (l <= u) {
        int mid = l + (u - l)/2;
        
        if (arr[mid] < key) { // key must be range [mid+1 .. u]
            l = mid + 1;
        } else if (arr[mid] > key) { // key must be range [l, mid-1]
            u = mid - 1;
        } else {
            if (mid == 0 || arr[mid-1] != key) { // arr[mid] equals key and is the first occurance.
                return mid;
            } else {
                u = mid - 1; // find left 1st element with value key
            }
        }
    }

    return 0;
}

int main(int argc, char const* argv[])
{
    // Note: arr must be in order.
    int arr[] = { 1, 2, 4, 4, 7, 7, 7, 8, 9, 9, 10, 11, 12 };
    int num;

    while (scanf("%d", &num) != EOF) {
        int idx = binary_search_1st(arr, NELEM(arr), num);

        if (idx >= 0) {
            printf("Found! Index is: %d\n", idx);
        } else {
            printf("Not found!\n");
        }
    }
    
    return 0;
}
```
> 运行结果：  
./binary_search_1st_run  
4  
Found! Index is: 2  
2  
Found! Index is: 1  
1  
Found! Index is: 0  
7  
Found! Index is: 4  


### P2 二分搜索递归写法

我们先写一下伪代码，然后用C程序去验证。

```
binary_search(arr, l, r, key)
if (l > r)
    return -1
mid = l + (r - l)/2
if (arr[mid] == key)
    return mid
else if (arr[mid] > key)
    binary_search(arr, l, mid-1, key)
else
    binary_search(arr, mid+1, u, key)
```

```
#include <stdio.h>

#define NELEM(arr) (sizeof(arr)/sizeof(arr[0]))

int binary_search(int arr[], int l, int r, int key)
{
    int mid;

    if (l > r) {
        return -1;
    }

    mid = l + (r - l)/2;
    if (arr[mid] == key) { // Found
        return mid;
    } else if (arr[mid] > key) { // recursive call binary_search [l .. mid - 1]
        return binary_search(arr, l, mid - 1, key);
    } else { // recursive call binary_search [mid + 1 .. r]
        return binary_search(arr, mid + 1, r, key);
    }
}

int main(int argc, char const* argv[])
{
    // Note: arr must be in order.
    int arr[] = { 1, 2, 4, 4, 7, 7, 7, 8, 9, 9, 10, 11, 12 };
    int num;

    while (scanf("%d", &num) != EOF) {
        int idx = binary_search(arr, 0, NELEM(arr) - 1, num);

        if (idx >= 0) {
            printf("Found! Index is: %d\n", idx);
        } else {
            printf("Not found!\n");
        }
    }
    
    return 0;
}
```
> 输出结果：  
./recursive_binary_search_run  
1  
Found! Index is: 0  
2  
Found! Index is: 1  
3  
Not found!  


### P3 黑白球问题

我们用代码来模拟这一过程，看下是否真如结论所言这个样子：  
当瓶中白色球为奇数时，最后剩下的是白色球；否则就是黑色球。

```
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

enum { WB = 0, BB = 1, BC };

int foo(int balls[BC])
{
    int c[BC];
    int i;

    while (balls[WB] + balls[BB] > 1) { // loop until more than one ball left
        for (i = 0; i < BC; i++) {
            c[i] = rand()%BC;
            if (balls[c[i]] == 0) { // this color is empty, choose the other
                c[i] = !c[i];
            }
            balls[c[i]]--; // take it out
        }

        if (c[0] == c[1]) { // same color, put a black ball back
            balls[BB]++;
        } else {
            balls[WB]++; // not the same color, "recover" the white ball's number
        }
    }

    assert(balls[WB] >= 0 && balls[BB] >= 0 && balls[WB] + balls[BB] == 1);
    
    printf("WB:%d BB:%d\n", balls[WB], balls[BB]);

    return 0;
}

int main(int argc, char const* argv[])
{
    int balls[BC];

    srand((unsigned int)time(NULL));

    while (scanf("%d %d", &balls[WB], &balls[BB]) != EOF) {
        assert(balls[WB] >= 0 && balls[BB] >= 0 && balls[WB] + balls[BB] >= 1);
        assert(!foo(balls));
    }

    return 0;
}
```
> 运行结果：  
./white_black_ball_run   
1 1  
WB:1 BB:0  
2 2  
WB:0 BB:1  
1 1  
WB:1 BB:0  
12 44  
WB:0 BB:1  
13 44  


### P4 临近线段问题

形如$y = a_i*x + b_i$的直线。有两个数组，分别表示系数$a_i$和$b_i$。

```
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// 这个算法和第3章计算税率的有点像。
int binarysearch(float a[], float b[], int n, float x, float y)
{
    int l, u;

    l = 0;
    u = n - 1;
    while (l <= u) {
        int mid = l + (u - l)/2;
        float y_mid = a[mid]*x + b[mid];
        if (y <= y_mid) { // the point is under this "mid line"
            if (mid == 0) { // the point is not between two lines
                return -1;
            } else {
                float y_mid_minus_1 = a[mid-1]*x + b[mid-1];
                if (y >= y_mid_minus_1) { // the line under this "mid line" is under this point
                    return mid; // find the lines wrap this point
                } else {
                    u = mid - 1;
                }
            }
        } else {
            l = mid + 1;
        }
    }

    return -1;
}

int main(int argc, char const *argv[])
{
    float *a, *b;
    int n;
    float x, y;
    int i;
    int ret;

    for ( ; ; ) {
        printf("Input line number: ");
        if (scanf("%d", &n) == EOF) {
            break;
        }
        assert(n >= 2);
        
        printf("Input x y: ");
        if (scanf("%f %f", &x, &y) == EOF) {
            break;
        }
        assert(x >= 0 && y >= 0 && x <= 1);

        a = (float*)malloc(n*sizeof(float));
        b = (float*)malloc(n*sizeof(float));
        if (!a || !b) {
            perror("malloc failed");
            return 1;
        }

        for (i = 0; i < n; i++) {
            a[i] = 1.0f; // lines with the same gradient
            b[i] = i;
        }

        ret = binarysearch(a, b, n, x, y);
        printf("ret = %d\n", ret);

        free(a);
        free(b);
    }

    return 0;
}
```
> 输出结果：  
Input line number: 3  
Input x y: 0.5 1.5  
ret = 1  
Input line number: 3  
Input x y: 0.5 1.3  
ret = 1  
Input line number: 3  
Input x y: 0.5 1.6  
ret = 2  
Input line number: 3  
Input x y: 0.5 2.5  
ret = 2  
Input line number: 3  
Input x y: 0.5 2.6  
ret = -1  
Input line number: 3  
Input x y: 0.5 0.5  
ret = 1  


### P5 乘方算法

这个算法有迭代法和递归法，时间复杂度是nlog(n)。普通的算法时间复杂度是O(n)。

```
#include <stdio.h>
#include <assert.h>

long square(long x)
{
    return x * x;
}

// Use iterate O(n) algorithm.
long myexp1(long num, int n)
{
    long res = 1;
    int i;

    // If n is 0, the following loop skipped, result is 1.
    // That is x^0 = 1.
    for (i = 1; i <= n; i++) {
        res *= num;
    }

    return res;
}

// Recursive algorithm.
long myexp2(long num, int n)
{
    if (n == 0) {
        return 1;
    } else {
        if (n&1) { // n is odd
            return num*myexp2(num, n-1);
        } else { // n is even
            return square(myexp2(num, n>>1));
        }
    }
}

// Recursive algorithm, another perspective.
long myexp3(long num, int n)
{
    if (n == 0) { // num^0 = 1
        return 1;
    } else {
        if (n&1) { // n is odd
            return num*myexp3(num*num, n>>1);
        } else { // n is even
            return myexp3(num*num, n>>1);
        }
    }
}

// Iterate algorithm.
long myexp4(long num, int n)
{
    long res = 1;

    while (n) {
        if (n&1) { // n is odd
            res *= num;
        }
        num *= num;
        n >>= 1;
    }

    return res;
}

int main(int argc, char const *argv[])
{
    long num;
    int n;

    for ( ; ; ) {
        printf("Input num n: ");
        if (scanf("%ld %d", &num, &n) == EOF) {
            break;
        }
        assert(num >= 0 && n >= 0);

        printf("myexp1: %ld\n", myexp1(num, n));
        printf("myexp2: %ld\n", myexp2(num, n));
        printf("myexp3: %ld\n", myexp3(num, n));
        printf("myexp4: %ld\n", myexp4(num, n));
    }

    return 0;
}
```
> 运行结果：  
Input num n: 4 5  
myexp1: 1024  
myexp2: 1024  
myexp3: 1024  
myexp4: 1024  
Input num n: 4 12  
myexp1: 16777216  
myexp2: 16777216  
myexp3: 16777216  
myexp4: 16777216  
Input num n: 4 0  
myexp1: 1  
myexp2: 1  
myexp3: 1  
myexp4: 1  
Input num n: 0 4  
myexp1: 0  
myexp2: 0  
myexp3: 0  
myexp4: 0  

