#include <stdio.h>
#include <assert.h>

long square(long x)
{
    return x * x;
}

// Use iterate O(n) algorithm.
long myexp1(long num, int n)
{
    long res = 1;
    int i;

    // If n is 0, the following loop skipped, result is 1.
    // That is x^0 = 1.
    for (i = 1; i <= n; i++) {
        res *= num;
    }

    return res;
}

// Recursive algorithm.
long myexp2(long num, int n)
{
    if (n == 0) {
        return 1;
    } else {
        if (n&1) { // n is odd
            return num*myexp2(num, n-1);
        } else { // n is even
            return square(myexp2(num, n>>1));
        }
    }
}

// Recursive algorithm, another perspective.
long myexp3(long num, int n)
{
    if (n == 0) { // num^0 = 1
        return 1;
    } else {
        if (n&1) { // n is odd
            return num*myexp3(num*num, n>>1);
        } else { // n is even
            return myexp3(num*num, n>>1);
        }
    }
}

// Iterate algorithm.
long myexp4(long num, int n)
{
    long res = 1;

    while (n) {
        if (n&1) { // n is odd
            res *= num;
        }
        num *= num;
        n >>= 1;
    }

    return res;
}

int main(int argc, char const *argv[])
{
    long num;
    int n;

    for ( ; ; ) {
        printf("Input num n: ");
        if (scanf("%ld %d", &num, &n) == EOF) {
            break;
        }
        assert(num >= 0 && n >= 0);

        printf("myexp1: %ld\n", myexp1(num, n));
        printf("myexp2: %ld\n", myexp2(num, n));
        printf("myexp3: %ld\n", myexp3(num, n));
        printf("myexp4: %ld\n", myexp4(num, n));
    }

    return 0;
}
