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