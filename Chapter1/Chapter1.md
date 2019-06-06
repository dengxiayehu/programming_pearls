### P1 如何生成10^7个不重复的随机数

思路：  
最先想到的方法是使用`rand()`函数生成一个随机数，然后先检测该数字是否已经在数组中出现了，若是则继续生成一个随机数，否则就插入到数组中。**显然这种算法太低效。**  
换个思路，可以先生成一个包含0~N-1的数组，然后使用交换算法将顺序的数组**打乱**，具体是：***遍历数组，将当前元素与其之后的任意一个元素交换***。

代码如下：
```
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

// NUM should be less than RAND_MAX which defined in stdlib.h.
// The value of RAND_MAX is INT_MAX.
#define NUM 10000000

int main(int argc, const char *argv[])
{
    int* numbers = NULL;
    int i = 0;
    FILE* fp = NULL;

    srand(time(NULL));

    numbers = (int*)malloc(sizeof(int)*NUM);
    if (!numbers) {
        perror("malloc failed");
        return 1;
    }

    // Prepare the sorted array, that is 0 --> NUM - 1.
    for (i = 0; i < NUM; i++) {
        numbers[i] = i;
    }

    // Do the shuffer.
    for (i = 0; i < NUM; i++) {
        // The shuffer algorithm, swap numbers[i] with numbers[i --> NUM - 1].
        int idx2swap = i + rand()%(NUM - i);

        int tmp = numbers[i];
        numbers[i] = numbers[idx2swap];
        numbers[idx2swap] = tmp;
    }

    // Open file to store the random numbers.
    fp = fopen("./800.txt", "w");

    // Print the result array.
    for (i = 0; i < NUM; i++) {
        if (numbers[i] >= 2000000) {
            // Only consider valid tele-phone numbers.
            fprintf(fp, "%d\n", numbers[i]);
        }
    }

    fclose(fp);

    free(numbers);

    return 0;
}
```
生成的800.txt文件大概有8000000行数据，说明rand()函数的随机性还是挺好的。


### P2 大文件的归并排序算法
描述：有一个大文件，里面是一些随机数，最大值小于10^7，且每个数字最多出现一次。现要求将这个大文件进行排序，结果生成到另一个文件中。  
约束：内存有限，只有1MB。

分析：若每个数字采用int型来表示，则1MB的空间最多存放1024*1024/4=262144个数字。我们可以将大文件进行拆分，每262144个数字一组在内存中使用系统自带的库函数进行排序（例如：`qsort`），将顺序的数字存入临时的小文件中。然后对已经有序的多个小文件（大概有8000000/262144=30.51个小文件）进行多路归并排序。  
关于多路归并的算法，可以使用标赛算法，每次从多个小文件中取出一个最小的数字写入到最终的输出文件中。

我们将这个题目分成两个程序，第一个程序用以从之前生成的800.txt文件分割成有序的多个小文件，第二个程序用以演示锦标赛算法。

```
// 从大文件分割小文件程序
#include <stdio.h>
#include <stdlib.h>

#define MEMORY_MAX      1024*1024 // 1MB
#define MEMORY_NUMS_MAX MEMORY_MAX/4 // int 4 bytes

// Non decreasing order.
static int cmp(const void* a, const void* b)
{
    return (*(int*)a - *(int*)b);
}


int main(int argc, const char *argv[])
{
    FILE* fp = fopen("./800.txt", "r");
    int num = 0;
    int nums = 0;
    int nums_file = 0;
    int need_split = 1;
    FILE* fp_split = NULL;
    int* num_arr = NULL;

    num_arr = (int*)malloc(sizeof(int)*MEMORY_NUMS_MAX);
    if (!num_arr) {
        perror("malloc failed");
        return 1;
    }

    if (!fp) {
        perror("fopen 800.txt failed");
        return 1;
    }

    for ( ; ; ) {
        if (need_split) {
            // Close previous split file if opend.
            if (fp_split) {
                fclose(fp_split);
            }

            char tmp_buf[32];
            snprintf(tmp_buf, sizeof(tmp_buf), "800_split_%d.txt", nums_file);
            printf("spliting file %s\n", tmp_buf);
            fp_split = fopen(tmp_buf, "w");
            if (!fp_split) {
                perror("fopen split file failed");
                break;
            }
            need_split = 0;
            nums_file++;
        }

        if (fscanf(fp, "%d\n", &num) == EOF) {
            if (ferror(fp)) {
                perror("fscanf split file failed");
            }
            break;
        }

        num_arr[nums] = num;
        ++nums;
        if (nums > MEMORY_NUMS_MAX) {
            int k = 0;

            // Do the sort in memory.
            qsort(num_arr, nums, sizeof(int), cmp);

            for (k = 0; k < nums; k++) {
                fprintf(fp_split, "%d\n", num_arr[k]);
            }

            nums = 0;
            need_split = 1;
        }
    }

    if (fp) {
        fclose(fp);
    }
    if (fp_split) {
        fclose(fp_split);
    }

    free(num_arr);

    return 0;
}
```

锦标赛算法需要使用到胜者树，具体是：
胜者树需要叶子节点是8的倍数，若是不够，则需要补齐。
胜者树总的节点个数（包括叶子节点n）为 n + n - 1 = 2n - 1。
```
// 锦标赛算法
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

#define NELEM(arr) (sizeof(arr)/sizeof(arr[0]))

typedef struct Node {
    int num;
    int win_idx;
} Node;

int main(int argc, const char *argv[])
{
    int num_arr[] = { 3, 4, 1, 6, 2, 8, 7, 9 };
    const int num_arr_len = NELEM(num_arr);
    int n = 1;
    int i = 0;
    int adjust_idx = 0;

    // Leaf number must be multiple of 2.
    while (n < num_arr_len) {
        n <<= 1;
    }

    // Alloc space for tournament algorithm
    Node* nodes = (Node*)malloc(sizeof(Node)*(2*n - 1));
    if (!nodes) {
        perror("malloc failed");
        return 1;
    }

    // Init leaf nodes.
    for (i = n - 1; i < 2*n - 1; i++) {
        int idx = i - (n - 1);

        if (idx < num_arr_len) {
            // Original item in array.
            nodes[i].num = num_arr[idx];
            nodes[i].win_idx = i;
        } else {
            // Extended item, set num to INT_MAX to avoid winning.
            nodes[i].num = INT_MAX;
            nodes[i].win_idx = -1;
        }
    }

    // Built the tree.
    for (i = n - 2; i >= 0; i--) {
        // We should compare from the end of nodes, that is 2*n-2 and 2*n-3.
        // 2*n-2 = 2*(n-1), 2*n-3=2*(n-1)-1 for n.
        // Now we iterate from i=(n-2), hense r=2*(i+1)=2*i+2, l=2*(i+1)-1=2*i+1.
        int r = 2*i + 1;
        int l = 2*i + 2;
        if (nodes[r].num < nodes[l].num) { // r wins (smaller)
            nodes[i] = nodes[r];
        } else { // l wins.
            nodes[i] = nodes[l];
        }
    }

    printf("The built tree is: ");
    for (i = 0; i < 2*n - 1; i++) {
        printf("%d:%d ", nodes[i].num, nodes[i].win_idx);
    }
    printf("\n");

    for (i = 0; i < num_arr_len; i++) {
        // Print the smallest num in root node.
        printf("%d:%d ", nodes[0].num, nodes[0].win_idx);

        // Set the winner-leaf's num to INT_MAX, so it won't win again in the adjustment.
        adjust_idx = nodes[0].win_idx;
        nodes[adjust_idx].num = INT_MAX;
        // Adjust the previous winner-leaf, from end to beginning.
        while (adjust_idx != 0) {
            int competitor_idx = 0;
            int parent_idx = 0;

            if (adjust_idx%2 == 0) { // parent's left node
                competitor_idx = adjust_idx - 1;
                parent_idx = adjust_idx/2 - 1;
            } else { // parent's right node
                competitor_idx = adjust_idx + 1;
                parent_idx = adjust_idx/2;
            }

            // Rematch again.
            if (nodes[competitor_idx].num < nodes[adjust_idx].num) {
                // Competitor wins.
                nodes[parent_idx] = nodes[competitor_idx];
            } else {
                // Me!
                nodes[parent_idx] = nodes[adjust_idx];
            }

            adjust_idx = parent_idx;
        }
    }

    printf("\n");

    free(nodes);

    return 0;
}
```
思考：锦标赛排序和堆排序的对比。  
参考如下网上找的一段说明文字：
> 首先锦标赛排序又叫树型选择排序，也是用二叉树这种数据结构。这种排序方法比快速排序快，主要是在N个选手中选出K个选手中有优势。这封信中说了一道高盛面试题，即如何从25个选手中决出前三名，就使用了这种算法。
但是缺点也很明显，需要比较大的辅助空间。

相关信息：  
百度一道面试题说起，题目是这样的： 给出一个长度是N的数组，现在要找出最小的两个元素，最少要多少次比较。  
分析： 如果找出1个最小的，比较次数无疑是 n - 1, ；如果用选择排序，再取选择第二个最小的又得比较n-2次。这种寻找的办法其实是可以优化的，在第一次寻找最小元素过程中，其实我们已经比较了很多元素了，那么为什么不利用前面比较的结果来寻找第二个最小的呢。  
这用到锦标赛排序的方法，这样就可以再使用 logn就可以找到了第二小的元素。


### P3 实现大文件的多趟排序算法
和P2程序实现相同的功能，将大文件中的数字进行排序，生成到新的文件中。内存限制在1MB。  
有10^7个数，内存中每次最多存储约250000个数。则需要扫描文件10^7/250000=40次。  
第一次从文件中扫出0~249999之间的数存入内存中，然后使用内存排序算法后将结果写到输出文件中，第二次从文件中扫出250000~499999同样处理后写到输出文件中，依此类推。  
这个算法比较直观。


### P4 实现用位图的算法来排序大文件
算法：有10^7个数字，内存有限。我们可以使用10^7个bit位来表示。对于出现的每个数，在对应的bit位上置为1。  
这个位向量的算法还是具有通用性的（虽然有一定的技巧性），可以适当应用到实际环境中。  
位向量算法真的是即省空间又省时间，对于出现诸如大数据的排序等问题，可以考虑使用位向量算法，多往这方面想想。  
但缺点是：只能判断数字的单次出现或者不出现，不能统计出数字出现了多少次。（通过将单bit扩展为多bit可以解决这个问题，参考程序P5）

```
#include <stdio.h>

#define BITSPERWORD 32 // use int array to represent bit vector
#define SHIFT 5 // div 32
#define MASK 0x1f // mask, the value of this is 31!

#define N 10000000 // numbers to sort

// i>>SHIFT locates the int element in array.
// i&MASK figures out value within 32.
// i<<(i&MASK) set the bit of value left shift.
static void set(int a[], int i)
{
    a[i>>SHIFT] |= (1<<(i&MASK)); // 对应int元素的对应bit位置1
}

static void clr(int a[], int i)
{
    a[i>>SHIFT] &= ~(1<<(i&MASK)); // 对应int元素与上只有特定位为0的值，效果就是特定位变成0
}

static int test(int a[], int i)
{
    return a[i>>SHIFT] & (1<<(i&MASK)); // 判断对应int元素的特定位是否位1（与上只有特定位为1的数）
}

int main(int argc, const char *argv[])
{
    int a[1 + N/BITSPERWORD];
    int i;
    int arr[11] = { 0, 34, 2, 3, 31,1,63, 4, 62, 33, 64 };

    // Clear the bit vector.
    for (i = 0; i < N; i++) {
        clr(a, i);
    }

    // Set the bit vector.
    for (i = 0; i < sizeof(arr)/sizeof(arr[0]); i++) {
        set(a, arr[i]);
    }

    // Print the sorted result.
    for (i = 0; i < N; i++) {
        if (test(a, i)) { // value i set
            printf("%d ", i);
        }
    }

    printf("\n");

    return 0;
}
```
> 程序输出：  
0 1 2 3 4 31 33 34 62 63 64 


### P5 类比P4程序，若是一个数字最多出现10次，应该怎么处理呢？
算法：  
最多10次的话， 用4个bit就可以了，这样每个数字需要半个字节，而不再是之前的1个bit。那么10^7的数字，需要5M左右的空间，若是内存限制在1MB的话，可以分多次用位向量算法来处理。
我们先关注内存足够的情况。  
+ 对于数字x，首先定位到它落在哪个数组元素里面，算法为x/8（也就是x>>3）
+ 然后定位在unsigned int元素里面的第几个半字节，算法为x%8（也就是x&0x07）
+ 然后将对应的半字节数+1（例如若是第1个半字节数需要加1，那么加上的数应用是10000b，而不是简单的1；若是第7个半字节需要加1，若最高位bit为1，若是int的数组，那么就会按照负数的情况加上一个数，这不是我们想要的，所以我们需要使用unsigned int）

注意：分配的空间大小是1+N/BITSPERWORD*4。

```
#include <stdio.h>

#define BITSPERWORD 32
#define SHIFT 3
#define MASK 0x07

#define N 10000000

static void set(unsigned int a[], int i)
{
    a[i>>SHIFT] += 1<<(4*(i&MASK));
}

static void clr(unsigned int a[], int i)
{
    a[i>>SHIFT] &= ~(0x0F<<(4*(i&MASK)));
}

static int test(unsigned int a[], int i)
{
    return (a[i>>SHIFT] & (0x0F<<(4*(i&MASK))))>>(4*(i&MASK));
}

int main(int argc, const char *argv[])
{
    // BITSPERWORD only contains 8 numbers, not 32 numbers, so the size must x4.
    unsigned int a[1 + N/BITSPERWORD*4];
    FILE* fp = NULL;
    FILE* fp_out = NULL;
    int i = 0;

    fp = fopen("800.txt", "r");
    if (!fp) {
        perror("fopen 800.txt failed");
        return 1;
    }

    fp_out = fopen("800_sorted_p5.txt", "w");
    if (!fp_out) {
        perror("fopen 800_sorted_p5.txt failed");
        return 1;
    }

    // Clear the bit vector.
    for (i = 0; i < N; i++) {
        clr(a, i);
    }
    printf("clear the bit vector done\n");

    // Read from file and set into bit vector.
    for ( ; ; ) {
        int num = 0;
        if (fscanf(fp, "%d\n", &num) != 1) {
            if (ferror(fp)) {
                perror("fscanf failed");
            }
            break;
        }

        set(a, num);
    }
    printf("load numbers from bit vector done\n");

    for (i = 0; i < N; i++) {
        int j = 0;
        int n = test(a, i);

        for (j = 0; j < n; j++) {
            fprintf(fp_out, "%d\n", i);
        }
    }

    fclose(fp_out);

    fclose(fp);

    return 0;
}
```


### P6 初始化大型空间的技术
描述：使用更多的空间来换取更少的运行时间存在一个问题：初始化空间本身需要消耗大量的时间。说明如何设计一种技术，在第一次访问向量的项时将其初始化为0。你的方案应该使用常量时间进行初始化和向量访问，使用的额外空间应正比于向量的大小。因为该方法通过进一步增加空间减少初始化的时间，所以仅在空间很廉价、时间很宝贵且向量很稀疏的情况下才考虑。  
分析：第一次看到这个题目是个莫名其妙的表情，仔细琢磨是这个意思：
就是当我们使用辅助空间的时候，假如辅助空间很大，那么我们在初始化这些辅助空间的时候也是件很费时间的事情。
算法：在使用到辅助空间的某个元素的时候进行初始化（设置一个需要的值），并且记录下来，其余没有被初始化的元素就保持不变。  
这种技术对辅助空间是个巨大的稀疏表时的时候很有用，也能够优化巨大稀疏表的矩阵算法。  
![P6 初始化技术](https://raw.githubusercontent.com/dengxiayehu/playground/master/Programming_Pearls/Chapter1/p6_init.jpg "P6 初始化技术")

```
具体的设置算法如下：
初始：top=0
from[i] = top
to[top] = i
top++
data[i] = 指定的初始值
当发现from[i] < top 且 to[from[i]] = i 时，表示下标i所在的data数组元素已经被初始化了。
```
