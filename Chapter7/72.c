#include <stdio.h>

int main(int argc, char const *argv[])
{
    int money;
    float r;
    int y;
    float total = 0.f;
    int i;

    for ( ; ; ) {
        printf("input money r y: ");
        if (scanf("%d %f %d", &money, &r, &y) == EOF) {
            break;
        }

        total = money;
        for (i = 0; i < y; i++) {
            total += r*total;
        }
        printf("total: %f\n", total);
    }

    return 0;
}
