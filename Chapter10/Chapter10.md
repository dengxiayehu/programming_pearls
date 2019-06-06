### 节省空间

这一章主要讲了空间节省的一些策略和方法。空间节省分为如下两方面：
1. 数据空间节省（内存）  
重新计算、稀疏矩阵、数据压缩、分配空间、复用空间（垃圾回收）
2. 代码空间节省（软件体积）  
使用函数、解释语言、汇编语言


### 稀疏矩阵的应用

我们简化一下书中的题目，将棋盘设置为5x5，房子的编号为0~10。

| | 列0 | 列1 | 列2 | 列3 | 列4 |
| :-: | :-: | :-: | :-: | :-: | :-: |
| 行4 |  3 |   | | 6| 1 |
| 行3 |    | 7 | | | |
| 行2 | 8  |   | | | 4 |
| 行1 |    | 0 | | 2 | | 
| 行0 | 5 |   | | | 9|

```
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define M 5
#define N 5

#define POINT_SIZE 10

#define MKRC(x, y) ((x)<<4|(y))
#define GR(v) (((v)&0xff)>>4)
#define GC(v) ((v)&0xf)

#define NELEM(arr) (sizeof(arr)/sizeof((arr[0])))

typedef struct Point {
    int row;
    int col;
    int price;
} Point;

typedef struct Node {
    int row;
    int col;
    int pointnum;
    struct Node *next;
} Node;

/*
 *     列0  列1 列2 列3 列4
 *  行4 3           6   1
 *  行3     7			
 *  行2 8               4
 *  行1     0       2   
 *  行0 5               9
 */
int init_points(Point *points)
{
    const int coord[POINT_SIZE] = {
        MKRC(1, 1), MKRC(4, 4), MKRC(1, 3), MKRC(4, 0), MKRC(2, 4),
        MKRC(0, 0), MKRC(4, 3), MKRC(3, 1), MKRC(2, 0), MKRC(0, 4),
    };
    int i;

    srand((unsigned int)time(NULL));
    for (i = 0; i < NELEM(coord); i++) {
        points[i].row = GR(coord[i]);
        points[i].col = GC(coord[i]);
        points[i].price = rand()%100+1;
    }

    return 0;
}

int main(int argc, char const *argv[])
{
    Point *points;
    int ret = 0;
    int i;
    int k;
    Node *nodes[N];
    
    points = (Point*)malloc(sizeof(Point)*POINT_SIZE);

    // Init points array.
    if (init_points(points) < 0) {
        ret = 1;
        goto out;
    }

    // Print points array.
    for (i = 0; i < POINT_SIZE; i++) {
        printf("%d-%d %d$\n", points[i].row, points[i].col, points[i].price);
    }

    // Init the sparse matrix.
    memset(nodes, 0, sizeof(nodes));
    for (i = 0; i < N; i++) {
        for (k = 0; k < POINT_SIZE; k++) {
            if (points[k].col == i) {
                Node *node = (Node*)malloc(sizeof(Node));
                node->col = i;
                node->row = points[k].row;
                node->pointnum = k;
                node->next = NULL;

                if (nodes[i] == NULL) {
                    nodes[i] = node;
                } else {
                    node->next = nodes[i];
                    nodes[i] = node;
                }
            }
        }
    }

    // Print the sparse matrix.
    for (i = 0; i < N; i++) {
        Node *p = nodes[i];

        printf("col:%d => ", i);
        while (p) {
            printf("%d-%d %d$; ", p->row, p->col, points[p->pointnum].price);
            p = p->next;
        }
        printf("\n");
    }

    // Search the sparse matrix.
    for ( ; ; ) {
        int row;
        int col;
        Node *p;
        int res;

        printf("input row col: ");
        if (scanf("%d %d", &row, &col) == EOF) {
            break;
        }

        res = -1;
        p = nodes[col];
        while (p) {
            if (p->row == row) {
                res = p->pointnum;
                break;
            }
            p = p->next;
        }
        printf("%d\n", res);
    }

    // Free the sparse matrix.
    for (i = 0; i < N; i++) {
        Node *p = nodes[i], *q;

        while (p) {
            q = p->next;
            free(p);
            p = q;
        }
    }

out:
    free(points);

    return ret;
}
```
> 输出结果：  
./sparse_matrix_run  
1-1 38$  
4-4 30$  
1-3 54$  
4-0 16$  
2-4 6$  
0-0 58$  
4-3 7$  
3-1 16$  
2-0 90$  
0-4 96$  
col:0 => 2-0 90$; 0-0 58$; 4-0 16$;  
col:1 => 3-1 16$; 1-1 38$;   
col:2 =>   
col:3 => 4-3 7$; 1-3 54$;   
col:4 => 0-4 96$; 2-4 6$; 4-4 30$;  
input row col: 1 1  
0  
input row col: 4 4  
1  
input row col: 4 4  
1  
input row col: 2 4  
4  
input row col: 4 2  
-1  


### 平行数组使用

算法：固有的points数组。将行列信息单独出去到一个平行数组，压缩row、col为一个元素到该数组中以节省空间。使用firstincol数组用以加速查询操作。firstincol数组“指向”的pointnum数组中的元素按行排序。如此在查询时可以使用二分查找更加高效。

```
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#define ROW 5
#define COL 5 // ROWxCOL board
#define POINT_SIZE 10

#define NELEM(arr) (sizeof(arr)/sizeof((arr[0])))
#define MKRC(r, c) ((r)<<4|(c)) // combine row and col into a single byte
#define GR(v) (((v)&0xff)>>4) // get row info from the single byte
#define GC(v) ((v)&0xf) // get col info from the single byte

typedef struct Point {
    int dummy; // some info about this point, temp is dummy
} Point;

int my_sort(uint8_t arr[], int n, uint8_t infos[])
{
    int i;
    int j;
    uint8_t t, bk;

    // Use simple insertion-sort.
    for (i = 1; i < n; i++) {
        bk = arr[i];
        t = GR(infos[arr[i]]);
        for (j = i; j > 0 && GR(infos[arr[j-1]]) > t; j--) {
            arr[j] = arr[j-1];
        }
        arr[j] = bk;
    }
    return 0;
}

int binary_search(uint8_t arr[], uint8_t n, uint8_t infos[], uint8_t row)
{
    int l;
    int u;

    l = 0;
    u = n-1;
    while (l <= u) {
        int m = l+(u-l)/2;
        uint8_t mrow = GR(infos[arr[m]]);
        
        if (mrow == row) {
            return m;
        } else if (mrow > row) {
            u = m-1;
        } else {
            l = m+1;
        }
    }

    return -1;
}

int main(int argc, char const *argv[])
{
    Point points[POINT_SIZE];
    uint8_t infos[POINT_SIZE] = {
        MKRC(1, 1)/*0*/, MKRC(4, 4)/*1*/, MKRC(1, 3)/*2*/, MKRC(4, 0)/*3*/, MKRC(2, 4)/*4*/,
        MKRC(0, 0)/*5*/, MKRC(4, 3)/*6*/, MKRC(3, 1)/*7*/, MKRC(2, 0)/*8*/, MKRC(0, 4)/*9*/,
    };
    uint8_t firstincol[COL+1];
    uint8_t pointnum[POINT_SIZE];
    int j, k, i, t;

    (void)points;

    // Init firstincol and pointnum array.
    for (j = i = 0; j < COL; j++) {
        firstincol[j] = i;
        t = 0; // rows in this col
        for (k = 0; k < POINT_SIZE && t < COL; k++) {
            if (GC(infos[k]) == j) { // in this col
                pointnum[i++] = k;
                t++;
            }
        }
    }
    firstincol[j] = i;

    // Dump the firstcol and pointnum array.
    printf("pointnum: ");
    for (i = 0; i < POINT_SIZE; i++) {
        printf("%d ", pointnum[i]);
    }
    printf("\nfirstincol: ");
    for (i = 0; i < COL+1; i++) {
        printf("%d ", firstincol[i]);
    }
    printf("\n");

    // Preprocess the pointnum array for binary search later.
    for (i = 0; i < COL; i++) {
        int n = firstincol[i+1]-firstincol[i];
        if (n > 1) {
            uint8_t *subarr = pointnum+firstincol[i];
            my_sort(subarr, n, infos);
        }
    }

    // Dump the pointnum array.
    printf("pointnum(sorted): ");
    for (i = 0; i < POINT_SIZE; i++) {
        printf("%d ", pointnum[i]);
    }
    printf("\n");

    for ( ; ; ) {
        int row;
        int col;
        uint8_t n;
        int res;

        printf("input row col: ");
        if (scanf("%d %d", &row, &col) == EOF) {
            break;
        }

        res = -1;
        n = firstincol[col+1]-firstincol[col];
        if (n > 0) {
            uint8_t *subarr = pointnum+firstincol[col];
            res = binary_search(subarr, n, infos, row);
            if (res != -1) {
                res += firstincol[col];
            }
        }
        printf("%d\n", res);  
    }

    return 0;
}
```
> 输出结果：  
./parallel_array_run  
pointnum: 3 5 8 0 7 2 6 1 4 9   
firstincol: 0 3 5 5 7 10   
pointnum(sorted): 5 8 3 0 7 2 6 9 4 1   
input row col: 1 1  
3  
input row col: 0 1  
-1  
input row col: 4 4  
9  


### 数字字符串编码

描述：输入形如"1234"的字符串，将其进行编码。具体编码算法是通过`c=a*10+b`可以将两个数字字符合并为一个。然后对其进行解码，还原输入字符串。

```
#include <stdio.h>
#include <string.h>
#include <stdint.h>

int main(int argc, char const *argv[])
{
    char str[128];
    char *p;
    uint8_t coded[64];
    char a, b, c;
    int k;
    int i;
    int j;

    for ( ; ; ) {
        printf("input int str: ");
        if (scanf("%s", str) == EOF) {
            break;
        }

        // Encode the input int string.
        p = str;
        k = 0;
        while (*p) {
            a = *p++ - '0';
            b = 0;
            if (*p) {
                b = *p++ - '0';
            }
            c = a*10+b;
            coded[k++] = c;
        }
        printf("orig length: %d, coded length: %d\n", (int)(p-str), k);

        // Let's try to decode it.
        memset(str, 0, sizeof(str));
        for (i = j = 0; i < k; i++) {
            c = coded[i];
            a = c/10 + '0';
            b = c%10 + '0';
            str[j++] = a;
            str[j++] = b;
        }
        str[j] = '\0';
        printf("the decoded str: %s\n", str);
    }

    return 0;
}
```
> 输出结果：  
./codec_int_str_run  
input int str: 1234  
orig length: 4, coded length: 2  
the decoded str: 1234  

*但这个程序有bug，当原始字符串个数是奇数时，结尾会多一个0。这个程序只能实践一下书中的编码算法*
