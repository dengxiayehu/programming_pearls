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
