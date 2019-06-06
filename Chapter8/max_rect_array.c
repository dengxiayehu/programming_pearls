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
