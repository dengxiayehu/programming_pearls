灵机一动的算法大部分存在于不要盲目着急开始问题，而要先进行一定时间的分析再加上适时地动手验证。欲速则不达的道理在哪里都是相通的。

二分查找的威力是巨大的，但它是基于有序序列的查找算法。有时耗费一定的资源先进行排序然后进行二分查找的行为是值得的。

### P1 二分查找算法
据说写一个bug free的二分查找算法很难，先小试牛刀，等后面的学习深入了，再来批判这个程序。
```
#include <stdio.h>
#include <stdlib.h>

#define NELEM(arr) (sizeof(arr)/sizeof(arr[0]))

static int binary_search(int arr[], int len, int key)
{
    int l = 0;
    int r = len - 1;
    int mid = 0;

    // In case a null array, avoid crash.
    if (arr == NULL) {
        return -1;
    }

    while (l <= r) {
        // Locate the middle of index l and r
        // NOTE: mid = (r - l)/2; is wrong!
        mid = l + (r - l)/2;
        if (arr[mid] == key) { // found it
            return mid;
        } else if (arr[mid] > key) { // key must in arr[l .. mid-1]
            r = mid - 1;
        } else { // key must in arr[mid+1 .. r]
            l = mid + 1;
        }
    }

    // Not found case.
    return -1;
}

int main(int argc, const char *argv[])
{
    int arr[] = { 1, 3, 4, 6, 9, 10 };
    int len = NELEM(arr);
    int key = 0;
    int i = 0;

    if (argc != 2) {
        fprintf(stderr, "usage: %s <key>\n", argv[0]);
        return 1;
    }

    key = atoi(argv[1]);

    i = binary_search(arr, len, key);
    if (i >= 0) {
        printf("Found key:%d with index:%d\n", key, i);
    } else {
        printf("Not found!\n");
    }

    return 0;
}
```
> 输出：  
[root@localhost Chapter2]# ./p1_binary_search_run 5  
Not found!  
[root@localhost Chapter2]# ./p1_binary_search_run 4  
Found key:4 with index:2

### P2 大文件定位空缺数字 
描述：有一个大文件，里面顺序存储了整型数字，大概有40亿个，找出其中缺失的数字。  
分析：
+ int表示的范围是-2147483648～2147483647，总共有42亿+个数字，而文件中大概有
40亿的数字，所以肯定有缺失的数字。假设内存足够，我们可以借鉴之前的位图排序算法，
使用2^32个整型数字（4字节，因为每个数都有可能重复40亿次）来表示位向量。假设每个
数最多重复2^8次的话，那么只需要2^32个字节就可以了。
+ 将int型从位的角度去观察，进行*二分查找*。
```
#include <stdio.h>

typedef unsigned char DataType;

#define BITSPERDATA (4)
#define NELEM(arr) (sizeof(arr)/sizeof(arr[0]))

int main(int argc, const char* argv[])
{
    DataType arr[] = { 0, 6, 1, 4, 2, 7, /*9,*/ 5, 3, 10, 8, 11, 12, 13, 14, 15 }, *parr = arr;
    DataType arr0[NELEM(arr)] = { 0 }; // temp area for corresponding bit is 0
    DataType arr1[NELEM(arr)] = { 0 }; // temp area for corresponding bit is 1
    int arr_len = NELEM(arr);
    DataType check_mask = 0;
    DataType lost_data = 0;
    int i = 0;
    int lost = 0;

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

        if (arr1_len < arr0_len) { // bit 1 is less than bit 0, indicates missing this check_mask.
            lost_data += check_mask;
            parr = arr1;
            arr_len = arr1_len;
            lost = 1; // we have a lost number
        } else {
            if (arr1_len != arr0_len) { // if not even, we have a lost number
                lost = 1;
            }
            parr = arr0;
            arr_len = arr0_len;
        }
    }

    if (lost) {
        printf("lost_data = %d\n", lost_data);
    } else {
        printf("nothing lost\n");
    }
    
    return 0;
}
```
> 输出：  
lost_data = 9  

> 若输入数据是`0, 6, 1, 4, 2, 7, 9, 5, 3, 10, 8 /*, 11, 12*/ ,13, 14, 15`的话，输出的结果是：  
lost_data = 12

这个算法可以保证找到一个不存在的数字。

**上述算法若是出现重复数字就失效了，进一步考虑在有重复数字的情况下该如何解决。**
> 我自己还在学习笔记里记下：二分查找法之前需要对表进行排序。现在看了这个问题以后，完全推翻了笔记中的结论。作者之前说明，只要给定一个范围，并且能将这个范围二分，并且保证答案就在这个更小的范围里头，或者原本就不存在就可以了。  
实际的答案意思是（如同大学里某些牛人教授，跳过了关键解题步骤，让我这种一般人情和以堪）：算法读取所有记录，将他们分为高位为1，以及高位为0两类放到不同文件里（用低位也可以），这个过程不需要多少工作内存，几十个byte足够。  
<https://book.douban.com/subject/1230206/discussion/25685970/>

继续参考如下这篇文章，讲得比较清楚：（但貌似还是讲的非重复数字检漏的算法）<http://www.voidcn.com/article/p-sguykuzd-xz.html>
```
解：设x是不存的数。

可以从最低位开始进行探测，根据0或者1进行分组，分成两组之后，比较哪组的数目较少，比如位上是0的较少的话，那么x的第一位就是0.

如果两组数量相当，那么对应的位数是0还是1都无所谓，你随便取值，可以暂且取成0。

这样依次类推到最高位，就可以得到一个 x的值，这个x就是我们要求的值。

比如：对3位的整数进行查找为例。
有如下8个3位的整数：0、1、2、3、4、4、6、7， 我们此时故意去掉7，以二进制表示是：
x :1   1   1       

0   0   0 

0   0   1
0   1   0

0   1   1
1   0   0 
1   0   1
1   1   0
可以看出来 最后一列也就是最低位上 有4个0，3个1，此时x的最低位取 1

此时序列里还剩下

001

011

101

第二位上有2个0，1个1，则x第二位取1，之后序列里只剩下
011

第三位上1个0，0个1，x第三位取1，     得到x = 111（二进制） 得到 x=7。

这次我们再去掉一个2，这样的话一共少了2个数 2跟7

以二进制表示是：

x :   ？   ？  ？       

      0   0   0 

      0   0   1

      0   1   1
      1   0   0 
      1   0   1
      1   1   0

此时最低位 0跟1的数目一样，我们暂时取0的那边，之后剩下的序列为：

000

100

110

此时第二位上的0比1多，我们取1，序列还剩下

110

第三位上有个1米有0，则此时取第三位为0，则 x= (010)(二进制)，即x=2

此时回过头去，再把最后一位设置为1的话，球出来的是x=7.

OK了，我的思路就是这样的，只是把书上的概念实例化了一下！！！ 若有43亿个数的话，求重复的数，取位数上出现较多的就可以了。
```


### P3 数组循环左移算法
#### P3-1 杂技算法
分析：  
如果，n与k互质，从0开始，每一个元素向前移动k个位置，一次循环就可以处理完所有元素，最后一个元素会从0位置取元素

如果，n与k不互质，仅仅从0开始，每次向前移动k个位置。终止时是不能把所有元素放到目的地的。这是要需要进行gcd(n，k)次循环。即第一次是从0开始，每次向前移动k个位置，直至循环结束。第二次是从1开始，每次向前移动k个位置，直至循环结束。第三次...直到第gcd(n，k)-1次。而且每次循环的最后一个元素都会回到该循环的起点

这里需要有个储备知识，如何求得两个数的最大公约数，参考如下链接：<https://www.cnblogs.com/verlen11/p/4020714.html>
```
更相减损术,又称"等值算法"

关于约分问题,实质是如何求分子,分母最大公约数的问题。《九章算术》中介绍了这个方法,叫做”更相减损术”,数学家刘徽对此法进行了明确的注解和说明,是一个实用的数学方法。

例：今有九十一分之四十九,问约之得几何?

我们用(91,49)表示91和49的最大公约数.按刘徽所说,分别列出分子,分母。

“以少减多,更相减损,求其等也,以等数约之,等数约之,即除也,其所以相减者皆等数之重叠,故以等数约之。”

译文如下：

约分的法则是：若分子、分母均为偶数时，可先被2除，否则，将分子与分母之数列在它处，然后以小数减大数，辗转相减，求它们的最大公约数，用最大公约数去约简分子与分母。

其与古希腊欧几里德所著的《几何原本》中卷七第一个命题所论的相同。列式如下:

91 49

42 49

42 7

35 7

28 7

21 7

14 7

7  7

这里得到的7就叫做“等数”,91和49都是这等数的重叠(即倍数),故7为其公约数.而7和7的最大公约数就是7,(7,7)=7,所以(91,49)=(42,7)=(7,7)=7

更相减损术在现代仍有理论意义和实用价值.吴文俊教授说:“在我国,求两数最大公约数即等数,用更相减损之术,将两数以小减大累减以得之,如求24与15的等数,其逐步减损如下表所示:(24,15)->(9,15)->(9,6)->(3,6)->(3,3)

每次所得两数与前两数有相同的等数,两数之值逐步减少,因而到有限步后必然获得相同的两数,也即所求的等数,其理由不证自明。

这个寓理于算不证自明的方法,是完全构造性与机械化的尽可以据此编成程序上机实施”.吴先生的话不仅说明了此法的理论价值,而且指明学习和研究的方向.

更相减损法很有研究价值,它奠定了我国渐近分数,不定分析,同余式论和大衍求一术的理论基础.望能仔细品味。
```

```
#include <stdio.h>
#include <stdlib.h>

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
    int num1 = 0;
    int num2 = 0;

    if (argc != 3) {
        printf("%s <num1> <num2>\n", argv[0]);
        return 1;
    }

    num1 = atoi(argv[1]);
    num2 = atoi(argv[2]);

    printf("gcd of %d and %d is: %d\n", num1, num2, gcd(num1, num2));

    return 0;
}
```
> 输出：  
[root@localhost Chapter2]# ./gcd_run 49 93  
gcd of 49 and 93 is: 1  
[root@localhost Chapter2]# ./gcd_run 49 63  
gcd of 49 and 63 is: 7

下面来实现一下杂技算法：
```
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
```
> 输出：  
[root@localhost Chapter2]# ./zaji_run 1  
2 3 4 5 6 1   
[root@localhost Chapter2]# ./zaji_run 2  
3 4 5 6 1 2   
[root@localhost Chapter2]# ./zaji_run 3  
4 5 6 1 2 3  
[root@localhost Chapter2]# ./zaji_run 4  
5 6 1 2 3 4  
[root@localhost Chapter2]# ./zaji_run 5  
6 1 2 3 4 5  
[root@localhost Chapter2]# ./zaji_run 6  
no need to left rotate  

#### P3-2 递归交换算法
```
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
```

#### P3-2 交换算法（非递归法）
```
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
```
> 输出结果：  
MXHWL0201030003:Chapter2 hj$ ./iterate_rotate_run 1  
3 2 4 5 6 7 1  
MXHWL0201030003:Chapter2 hj$ ./iterate_rotate_run 2  
3 5 4 6 7 1 2  
MXHWL0201030003:Chapter2 hj$ ./iterate_rotate_run 3  
4 5 7 6 1 2 3  
MXHWL0201030003:Chapter2 hj$ ./iterate_rotate_run 4  
5 6 7 2 1 3 4  
MXHWL0201030003:Chapter2 hj$ ./iterate_rotate_run 5  
6 7 1 2 4 3 5  
MXHWL0201030003:Chapter2 hj$ ./iterate_rotate_run 6  
7 1 2 3 4 6 5  
MXHWL0201030003:Chapter2 hj$ ./iterate_rotate_run 7  
no need to left rotate  

#### P3-3 翻手算法
ab -> ba
((a)^r(b)^r)^r = ba

### P4 矩阵快速转置算法
```
#include <stdio.h>
#include <stdlib.h>

#define ROWS 4
#define COLS 3

typedef struct WrappedData {
    int i;
    int j;
    int data;
} WrappedData;

int compare_col(const void* arg1, const void* arg2)
{
    return ((WrappedData*)arg1)->j - ((WrappedData*)arg2)->j;
}

int compare_row(const void* arg1, const void* arg2)
{
    return ((WrappedData*)arg1)->i - ((WrappedData*)arg2)->i;
}

int main(int argc, char const* argv[])
{
    int matrix[ROWS][COLS];
    int i = 0;
    int j = 0;
    WrappedData* matrix1 = NULL;

    // Init and print the matrix.
    for (i = 0; i < ROWS; i++) {
        for (j = 0; j < COLS; j++) {
            matrix[i][j] = i*COLS + j;
            printf("%3d ", matrix[i][j]);
        }
        printf("\n");
    }

    matrix1 = (WrappedData*)malloc(ROWS*COLS*sizeof(WrappedData));
    if (!matrix1) {
        perror("malloc failed\n");
        return 1;
    }

    // Init matrix1 with matrix.
    for (i = 0; i < ROWS; i++) {
        for (j = 0; j < COLS; j++) {
            WrappedData* wd = &matrix1[i*COLS + j];
            wd->i = i;
            wd->j = j;
            wd->data = matrix[i][j];
        }
    }

    // Sort matrix1 with col num.
    qsort(matrix1, ROWS*COLS, sizeof(WrappedData), compare_col);

    for (i = 0; i < COLS; i++) {
        // Sort each new row.
        qsort(matrix1 + i*ROWS, ROWS, sizeof(WrappedData), compare_row);
    }

    // Print the reversed matrix.
    for (i = 0; i < COLS; i++) {
        for (j = 0; j < ROWS; j++) {
            printf("%d ", matrix1[i*ROWS + j].data);
        }
        printf("\n");
    }

    free(matrix1);

    return 0;
}
```
> 输出：  
  0   1   2  
  3   4   5  
  6   7   8  
  9  10  11  
0 3 6 9  
1 4 7 10  
2 5 8 11 

矩阵转置算法应该有更高效的才对，怎么上面这个排序的算法会加快速度呢？参考如下解释：  
I think the meaning of this exercise is as follows.

+ For the computer in that old age, there was no sufficient RAM to hold a matrix with that size. So your proposed swapping method could not be feasible. In order to transpose such a large matrix, the external memory storage, i.e. magnetic tape, should be exploited.

+ However, the reading and writing tapes back and forth is rather slow. But tapes are serial storage device. So reading and writing in serial can save a lot of time.

+ Merge sort is very suitable for such serialization storage because of the way it accesses elements, as what is said at this wikipedia page. So I believe the "system tape sort" meant merge sort on tape.

After keeping in mind the three points above, I think you can understand this exercise.


### P5 用全排列算法求出所有的变位词

#### P5-1
看来leetcode没有白刷啊，还是能够徒手写一个全排列的算法。但这个算法有点矬，还使用到了临时的标记空间v。

```
#include <iostream>
#include <vector>

using namespace std;

void foo(const string& str, int cur_idx, string& candidate, bool v[], vector<string>& res)
{
    if (cur_idx == str.length()) {
        // Check duplicated.
        for (vector<string>::iterator it = res.begin(); it != res.end(); it++) {
            if (*it == candidate) {
                return;
            }
        }
        res.push_back(candidate);
        return;
    }

    for (int i = 0; i < str.length(); i++) {
        if (!v[i]) {
            v[i] = true;
            candidate += str[i];
            foo(str, cur_idx + 1, candidate, v, res);
            candidate.erase(candidate.end() - 1);
            v[i] = false;
        }
    }
}

int main(int argc, char const* argv[])
{
    string str("12345");
    vector<string> res;
    string candidate;
    bool* v = new bool[str.length()]();

    foo(str, 0, candidate, v, res);

    for (vector<string>::iterator it = res.begin(); it != res.end(); it++) {
        cout << *it << endl;
    }
    cout << "The total size is: " << res.size() << endl;

    delete [] v;

    return 0;
}
```
> 输出结果：  
123  
132  
213  
231  
312  
321  
The total size is: 6  

#### P5-2 递归全排列算法 不使用临时标记 考虑重复排列的情况

这个算法对比P5-1有两点优化：
1. 没有使用临时标记空间
2. 算法很高效 perm(a, b, c) = a.perm(b,c) + b.perm(a, c) + c.perm(b, a)，大问题变成子问题
3. 考虑了重复元素的情况

```
#include <iostream>
#include <vector>
#include <cstring>

using namespace std;

#define SWAP(arr, a, b) do { \
    if ((a) != (b)) { \
        arr[a] ^= arr[b]; arr[b] ^= arr[a]; arr[a] ^= arr[b]; \
    } \
} while (0)

bool need_swap(char*str, int k, int i)
{
    for (int j = k; j < i; j++) {
        if (str[j] == str[i]) {
            return false;
        }
    }

    return true;
}

void perm(char* str, int k, int m, vector<string>& res)
{
    // Found the candidate.
    if (k == m) {
        res.push_back(str);
        return;
    }

    // Exchange str[k] with str[k .. m].
    for (int i = k; i <= m; i++) {
        // Current check value is str[i], if str[k .. i - 1] has char equals to str[i],
        // then str[i] cannot be swapped.
        // 假设str[k .. i - 1]中已经出现了和str[i]一样的字符，那说明以str[i]“开头”的子全排列已经
        // 被考虑了，所以需要略过当前的str[i]开头的子全排列。
        if (need_swap(str, k, i)) {
            SWAP(str, k, i);
            perm(str, k + 1, m, res);
            SWAP(str, k, i);
        }
    }
}

int main(int argc, char const* argv[])
{
    char str[] = "122";
    vector<string> res;

    perm(str, 0, strlen(str) - 1, res);

    for (vector<string>::iterator it = res.begin(); it != res.end(); it++) {
        cout << *it << endl;
    }

    return 0;
}
```
> 程序输出：  
122  
212  
221  

#### P5-3 非递归全排列算法 字典序排序
算法：
+ 若输入数组排序不是有序的，则先进行递增排序。
+ 从后往前找到第一个递增序列，A[i] < A[i+1]，则从A[i+1 .. n-1]中找到最小的1个大于A[i]的数字，并与A[i]交换。
+ 将A[i+1 .. n-1]逆序。

**神奇的是：这个算法还兼容重复数字的情况，属于全排列的终极算法。**  
但是这个算法需要输入是有序的时候，效率才最高，否则需要先nlogn的开销进行排序。注意。

```
#include <iostream>
#include <vector>

using namespace std;

int comp_char(const void* arg1, const void* arg2)
{
    return *((char*)arg1) - *((char*)arg2);
}

int main(int argc, char const* argv[])
{
    if (argc != 2) {
        cerr << "usage: " << argv[0] << " <str>" << endl;
        return 1;
    }

    char* str = strdup(argv[1]);
    int len = strlen(str);
    // Sort str.
    qsort(str, len, sizeof(char), comp_char);
    cout << "Sorted string is: " << str << endl;

    vector<string> res;
    for ( ; ; ) {
        // Save one candidate.
        res.push_back(str);

        int i = len - 2;
        // Found A[i] < A[i+1] pair.
        while (i >= 0) {
            if (str[i] < str[i+1]) {
                break;
            }
            --i;
        }
        if (i < 0) {
            break;
        }

        // Found the smallest value in A[i+1 .. n-1] bigger than A[i].
#if 0
        // Not quick enough.
        int k = i + 1;
        for (int j = i + 2; j < len; j++) {
            if (str[j] > str[i] && str[j] < str[k]) {
                k = j;
            }
        }
#else
        // 仔细思考一下，大于str[i]的最小元素一定从字符串的尾部出现的第1个大于str[i]的元素。
        int k = len - 1;
        while (k >= i + 1 && str[k] <= str[i]) {
            k--;
        }
#endif

        // Swap str[i] with str[k].
        char t = str[i];
        str[i] = str[k];
        str[k] = t;

        // Reverse str[i+1 .. n-1]
        for (int j = i + 1, k = len - 1; j < k; j++, k--) {
            int t = str[j];
            str[j] = str[k];
            str[k] = t;
        }
    }

    cout << "size is : " << res.size() << endl;
    for (vector<string>::iterator it = res.begin(); it != res.end(); it++) {
        cout << *it << endl;
    }

    free(str);

    return 0;
}
```

#### P5-4 变位词的管道实现
分析：需要编写两个小程序，第1个程序用来从输入的单词求得其标识，第2个程序用来将统一标识的单词在一行输出。  
sign.c
```
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WORD_MAXLEN 32

int comp_char(const void* arg1, const void* arg2)
{
    return *((const char*)arg1) - *((const char*)arg2);
}

int main(int argc, char const* argv[])
{
    char word[WORD_MAXLEN] = { 0 };
    char sign[WORD_MAXLEN] = { 0 };

    while (scanf("%s", word) != EOF) {
        // Copy word into sign for sorting.
        strcpy(sign, word);
        // Use qsort for soring sign.
        qsort(sign, strlen(sign), sizeof(char), comp_char);

        printf("%s %s\n", sign, word);
    }

    return 0;
}
```

signature.c
```
#include <stdio.h>
#include <string.h>

#define WORD_MAXLEN 32

int main(int argc, char const* argv[])
{
    char word[WORD_MAXLEN] = { 0 };
    char sign[WORD_MAXLEN] = { 0 };
    char prev_sign[WORD_MAXLEN] = { 0 };

    while (scanf("%s %s", sign, word) != EOF) {
        if (!strcmp(sign, prev_sign) /*word has the same sign with previous one(s)*/ ||
            prev_sign[0] == '\0' /*this is the first word, print it anyway*/) {
            printf("%s ", word);
        } else { // word has different sign, print \n and it
            printf("\n%s ", word);
        }

        // Save sign.
        strcpy(prev_sign, sign);
    }

    printf("\n");

    return 0;
}
```
> 运行及输出：  
$ cat txt  
stop  
psto  
hello  
lleho  
$ cat txt | ./sign_run  | sort | ./signature_run  
hello lleho  
psto stop 

学会使用小程序解决特定问题，然后组合使用（管道）解决各个特定问题的小程序为一个大程序来解决最终问题。
变位词算法使用标识的方式比求其所有的全排列算法要高效得几个量级。啊哈，算法！


### P6 找出一堆数里重复的数字
描述：有一个文件，里面包含43亿的整型数字。因为int型最多表示的范围不到43亿，所以这个文件中必然存在重复的数字，试找出其中一个。

算法：参考程序P2，每次选多的那一堆。  
实际验证算法时，不可能真生成43亿个数，我们可以缩小范围，采用4个比特表示整型数来模拟。
```
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
```
> 输出：  
The duplicated data is: 7


### P7 判断数组中是否存在k个数之和小于t
分析：有个很直观的算法，先将数组排序，然后取前面的k个数相加是否小于t，这样算法的复杂度是$n\log_2{n}$。  

但是这样还是不够高效，其实只要“排序”前k个数字就好了，可以使用最小堆来实现，这样时间复杂度是$n\log_2{k}$。

更进一步，可以借鉴快速排序的思想，找到第k大的数字的索引，然后求和前面k个数字，这个算法的期望时间复杂度是n。

#### P7-1 排序数组的所有元素，取前k个元素和判断
#### P7-2 排序数组中的前k个元素
```
#include <stdio.h>
#include <stdlib.h>

#define NELEM(arr) (sizeof(arr)/sizeof(arr[0]))

// The most foundamental one.
int partition(int arr[], int left, int right)
{
    // Take arr[right] as the standard element.
    int t = arr[right];

    while (left < right) {
        // Left to right, find the element bigger than t.
        while (left < right && arr[left] <= t) left++;
        // Take arr[left] (the bigger one) to arr[right].
        arr[right] = arr[left];

        // Right to left, find the element smaller than t.
        while (right > left && arr[right] >= t) right--;
        // Take arr[right] (the smaller one) to arr[left].
        arr[left] = arr[right];
    }

    // Take t into arr[left].
    arr[left] = t;

    return left;
}

int median3(int a[], int i, int j, int k)
{
    return (a[i] < a[j]) ?
            (a[j] < a[k] ? j : a[i] < a[k] ? k : i) : (a[k] < a[j] ? j : a[k] < a[i] ? k : i);
}

#define SWAP(arr, i, j) do { \
    arr[i] ^= arr[j]; arr[j] ^= arr[i]; arr[i] ^= arr[j]; \
} while (0)

// 三取样切分。
int partition1(int arr[], int left, int right)
{
    // Found the medium number of arr[left] arr[(right+left)/2] arr[right].
    int m = median3(arr, left, (right+left)/2, right);
    int i, j, t;

    // If use xor method to exchange two elements, must not the same value, or the swapped value is 0!
    if (left != m) {
        // Exchange arr[left] with arr[m], and take the new arr[left] as standard element.
        SWAP(arr, left, m);
    }

    i = left;
    j = right;
    t = arr[left]; // the standard element

    // Special case 1.
    while (arr[++i] <= t) {
        if (i == right) {
            // The right most element is still smaller than t, swap arr[left] and arr[right].
            SWAP(arr, left, right);
            return right;
        }
    }

    // Special case 2.
    while (arr[j] >= t) {
        if (j == left) {
            // The left most element is still bigger than t, already in "order" state.
            return left;
        }
        --j;
    }

    // Currently arr[i] > t and arr[j] < t.

    while (i < j) {
        // Exchange arr[i] with arr[j].
        SWAP(arr, i, j);

        // Try to find next i and j.
        while (arr[++i] <= t) ;
        while (arr[--j] >= t) ;
    }

    // Take the standard element into the right place.
    // Current arr[j] is smaller than t.
    SWAP(arr, j, left);

    return j;
}

int partition2(int arr[], int left, int right)
{
    // Found the medium number of arr[left] arr[(right+left)/2] arr[right].
    int m = median3(arr, left, (right+left)/2, right);
    int i, j, t;

    // If use xor method to exchange two elements, must not the same value, or the swapped value is 0!
    if (left != m) {
        // Exchange arr[left] with arr[m], and take the new arr[left] as standard element.
        SWAP(arr, left, m);
    }

    i = left;
    j = right;
    t = arr[left]; // the standard element

    // Special case 1.
    while (arr[++i] <= t) {
        if (i == right) {
            // The right most element is still smaller than t, swap arr[left] and arr[right].
            SWAP(arr, left, right);
            return right - left + 1;
        }
    }

    // Special case 2.
    while (arr[j] >= t) {
        if (j == left) {
            // The left most element is still bigger than t, already in "order" state.
            return 1;
        }
        --j;
    }

    // Currently arr[i] > t and arr[j] < t.

    while (i < j) {
        // Exchange arr[i] with arr[j].
        SWAP(arr, i, j);

        // Try to find next i and j.
        while (arr[++i] <= t) ;
        while (arr[--j] >= t) ;
    }

    // Take the standard element into the right place.
    // Current arr[j] is smaller than t.
    SWAP(arr, j, left);

    return j - left + 1;
}

void my_qsort(int arr[], int left, int right)
{
    int idx = 0;

    if (left >= right) {
        return;
    }

    idx = partition1(arr, left, right);
    my_qsort(arr, left, idx - 1);
    my_qsort(arr, idx + 1, right);
}

int main(int argc, char const* argv[])
{
    int arr[] = { 4, 5, 9, 1, 2, 10, 6, 3, 99, 46, 23, 11, -1, -3, -1 };
    /*int arr[] = { 1, 2, 3, 4, 5 };*/
    /*int arr[] = { 5, 4, 3, 2, 1 };*/
    int k = 0;
    int t = 0;
    int i = 0;
    int left = 0;
    int right = NELEM(arr) - 1;
    int n = 0;
    int k_bak = 0;
    int sum = 0;

    /*my_qsort(arr, 0, right);*/

    if (argc != 3) {
        fprintf(stderr, "%s <k> <t>\n", argv[0]);
        return 1;
    }

    k = k_bak = atoi(argv[1]);
    t = atoi(argv[2]);

    printf("original array is: \n");
    for (i = 0; i < NELEM(arr); i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    for ( ; ; ) {
        n = partition2(arr, left, right);
        if (n == k) {
            break;
        } else if (n < k) {
            k -= n;
            left += n;
        } else {
            right = left + n - 1 - 1;
        }
    }

    k = k_bak;
    printf("k = %d, t = %d\n", k, t);
    for (i = 0; i < NELEM(arr); i++) {
        printf("%d ", arr[i]);
        if (i < k) {
            sum += arr[i];
            if (i == k - 1) {
                printf("| ");
            }
        }
    }
    printf("\n");

    if (sum < t) {
        printf("Found\n");
    } else {
        printf("Not found\n");
    }

    return 0;
}
```
> 输出：  
[root@localhost Chapter2]# ./sort_k_sum_run 2 9  
original array is:  
4 5 9 1 2 10 6 3 99 46 23 11 -1 -3 -1   
k = 2, t = 9  
-3 -1 | -1 1 2 3 6 4 99 46 23 11 10 9 5   
Found  
[root@localhost Chapter2]# ./sort_k_sum_run 2 -4  
original array is:   
4 5 9 1 2 10 6 3 99 46 23 11 -1 -3 -1   
k = 2, t = -4  
-3 -1 | -1 1 2 3 6 4 99 46 23 11 10 9 5   
Not found  

#### P7-3 最小堆算法

分析：使用最小堆算法，弹出k个数，看k个数的和是否小于t即可。这个算法参考《算法4》上面的实现尤为精妙。下面把它搬运过来。
```
#include <stdio.h>
#include <stdlib.h>

#define NELEM(arr) (sizeof(arr)/sizeof(arr[0]))

typedef int DataType;

typedef struct MinPQ {
    DataType* data;
    int n;
} MinPQ;

int swap(MinPQ* min_pq, int i, int j)
{
    DataType tmp = min_pq->data[i];
    min_pq->data[i] = min_pq->data[j];
    min_pq->data[j] = tmp;

    return 0;
}

int sink(MinPQ* min_pq, int i)
{
    while (2*i <= min_pq->n) {
        int c = 2*i; // left child node

        // Figure out whether right child node is even smaller.
        if (c < min_pq->n && min_pq->data[c] > min_pq->data[c+1]) {
            c++;
        }

        // Check whether this node is smaller than the children, if so, we stop.
        if (min_pq->data[i] <= min_pq->data[c]) {
            break;
        }

        // Swap the parent with smaller child.
        swap(min_pq, i, c);

        // Reconsider the previous smaller child.
        i = c;
    }

    return 0;
}

int extract_min(MinPQ* min_pq, DataType* t)
{
    // If queue is empty, return error.
    if (min_pq->n == 0) {
        return -1;
    }

    // Return the root node.
    if (t) {
        *t = min_pq->data[1];
    }

    // Exchange root node with data[n].
    swap(min_pq, 1, min_pq->n);

    // Queue's size minus 1.
    min_pq->n--;

    // Sink the new root to the right place.
    sink(min_pq, 1);

    return 0;
}

int main(int argc, char const* argv[])
{
    DataType arr[] = { 4, 5, 9, 1, 2, 10, 6, 3, 99, 46, 23, 11, -1, -3, -1 };
    MinPQ min_pq;
    int i;
    int k;
    DataType t;
    DataType sum = 0;

    if (argc != 3) {
        fprintf(stderr, "usage: %s <k> <t>\n", argv[0]);
        return -1;
    }

    k = atoi(argv[1]);
    t = atoi(argv[2]);

    min_pq.n = NELEM(arr);
    min_pq.data = (DataType*)malloc(sizeof(DataType)*(min_pq.n + 1));

    // Init the min heap.
    for (i = 0; i < min_pq.n; i++) {
        min_pq.data[i + 1] = arr[i];
    }
    // We need to do sink[n/2 .. 1].
    for (i = min_pq.n/2; i >= 1; i--) {
        sink(&min_pq, i);
    }

    for (i = 0; i < k; i++) {
        DataType tmp;
        if (extract_min(&min_pq, &tmp) < 0) {
            fprintf(stderr, "extract_min failed\n");
            goto out;
        }

        sum += tmp;
        printf("%d ", tmp);
    }
    printf("\n");

    if (sum < t) {
        printf("Found!\n");
    } else {
        printf("Not found!\n");
    }

out:
    free(min_pq.data);

    return 0;
}
```
> 输出：  
MXHWL0201030003:Chapter2 hj$ ./min_heap_k_sum_run 1 -4  
-3   
Not found!  
MXHWL0201030003:Chapter2 hj$ ./min_heap_k_sum_run 1 1  
-3   
Found!  
