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
