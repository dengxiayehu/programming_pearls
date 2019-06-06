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
