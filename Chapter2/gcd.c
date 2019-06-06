#include <stdio.h>
#include <stdlib.h>

int gcd(int a, int b)
{
    while (a != b) {
        if (a > b) {
            a -= b;
        } else {
            b -= a;
        }
    }

    return a;
}

int main(int argc, const char* argv[])
{
    int num1 = 0;
    int num2 = 0;

    if (argc != 3) {
        printf("%s <num1> <num2>\n", argv[0]);
        return 1;
    }

    num1 = atoi(argv[1]);
    num2 = atoi(argv[2]);

    printf("gcd of %d and %d is: %d\n", num1, num2, gcd(num1, num2));

    return 0;
}
