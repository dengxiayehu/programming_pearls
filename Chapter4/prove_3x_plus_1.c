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
