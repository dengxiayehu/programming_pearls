#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

uint32_t count1(uint8_t n)
{
    uint32_t cnt = 0;

    while (n) {
        n &= (n-1);
        cnt++;
    }

    return cnt;
}

int main(int argc, char const *argv[])
{
    uint32_t table[UINT8_MAX+1];
    int i;
    char buf[] = "hitherehelloworld";
    char counts[UINT8_MAX+1];
    char *p;
    int total;

    // Init the count table for uint8_t.
    for (i = 0; i < (UINT8_MAX+1); i++) {
        table[i] = count1(i);
    }

    printf("the count table is: \n");
    for (i = 0; i < (UINT8_MAX+1); i++) {
        printf("%d ", table[i]);
    }
    printf("\n");

    for ( ; ; ) {
        printf("input string: ");
        if (scanf("%s", buf) == EOF) {
            break;
        }

        memset(counts, 0, sizeof(counts));
        p = buf;
        while (*p) {
            counts[(int)(*p)]++;
            p++;
        }

        total = 0;
        for (i = 0; i < (UINT8_MAX+1); i++) {
            if (counts[i]) {
                total += counts[i]*count1(i);
            }
        }
        printf("the result is: %d\n", total);
    }

    return 0;
}
