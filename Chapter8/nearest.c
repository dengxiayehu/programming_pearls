#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define FLT_MAX 3.402823466e+38F

#define NELEM(arr) (sizeof(arr)/sizeof(arr[0]))

int main(int argc, char const *argv[])
{
    float array[] = { 31, -41, 59, 26, -53, 58, 97, -93, -23, 84 };
    const int n = NELEM(array);
    float tmp[n+1], *curmum = tmp+1;
    float t;
    float nearest = FLT_MAX;
    int i;
    int j;
    int l;
    int r;

    for ( ; ; ) {
        printf("input t: ");
        if (scanf("%f", &t) == EOF) {
            break;
        }

        curmum[-1] = 0;
        for (i = 0; i < n; i++) {
            // curmum[i] is the sum of elements with index -1 to i.
            curmum[i] = curmum[i-1]+array[i];
        }

        for (i = 0; i < n; i++) {
            for (j = i; j < n; j++) {
                float sum = curmum[j]-curmum[i-1];
                
                if (fabs(sum-t) < fabs(nearest-t)) {
                    nearest = sum;
                    l = i;
                    r = j;
                }
            }
        }

        printf("the nearest sum is: %f [%d .. %d]\n", nearest, l, r);
    }

    return 0;
}
