#include <stdio.h>
#include <stdint.h>

int main(int argc, char const *argv[])
{
    uint32_t n;
    int cnt;

    for ( ; ; ) {
        printf("input a number: ");
        if (scanf("%u", &n) == EOF) {
            break;
        }

        cnt = 0;
        while (n) {
            n &= (n-1);
            cnt++;
        }
        printf("bits 1 count: %d\n", cnt);
    }

    return 0;
}
