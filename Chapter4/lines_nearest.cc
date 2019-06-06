#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// 这个算法和第3章计算税率的有点像。
int binarysearch(float a[], float b[], int n, float x, float y)
{
    int l, u;

    l = 0;
    u = n - 1;
    while (l <= u) {
        int mid = l + (u - l)/2;
        float y_mid = a[mid]*x + b[mid];
        if (y <= y_mid) { // the point is under this "mid line"
            if (mid == 0) { // the point is not between two lines
                return -1;
            } else {
                float y_mid_minus_1 = a[mid-1]*x + b[mid-1];
                if (y >= y_mid_minus_1) { // the line under this "mid line" is under this point
                    return mid; // find the lines wrap this point
                } else {
                    u = mid - 1;
                }
            }
        } else {
            l = mid + 1;
        }
    }

    return -1;
}

int main(int argc, char const *argv[])
{
    float *a, *b;
    int n;
    float x, y;
    int i;
    int ret;

    for ( ; ; ) {
        printf("Input line number: ");
        if (scanf("%d", &n) == EOF) {
            break;
        }
        assert(n >= 2);
        
        printf("Input x y: ");
        if (scanf("%f %f", &x, &y) == EOF) {
            break;
        }
        assert(x >= 0 && y >= 0 && x <= 1);

        a = (float*)malloc(n*sizeof(float));
        b = (float*)malloc(n*sizeof(float));
        if (!a || !b) {
            perror("malloc failed");
            return 1;
        }

        for (i = 0; i < n; i++) {
            a[i] = 1.0f; // lines with the same gradient
            b[i] = i;
        }

        ret = binarysearch(a, b, n, x, y);
        printf("ret = %d\n", ret);

        free(a);
        free(b);
    }

    return 0;
}
