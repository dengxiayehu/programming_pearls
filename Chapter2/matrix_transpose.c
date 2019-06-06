#include <stdio.h>
#include <stdlib.h>

#define ROWS 10
#define COLS 20

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
