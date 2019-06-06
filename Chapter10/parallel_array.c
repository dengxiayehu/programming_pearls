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
