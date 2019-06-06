#include <stdio.h>

#define N 1000

int main(int argc, char const *argv[])
{
    int arr[N];
    int l;
    int i;
    int t;
    int p;

    for (i = 0; i < N; i++) {
        arr[i] = i+1;
    }

    for ( ; ; ) {
        printf("Input t: ");
        if (scanf("%d", &t) == EOF) {
            break;
        }

        // Check t is in (-1, 511) or (488, 1000).
        l = -1;
        i = 512;
        // Binary search algorithm by hand.
        if (arr[511] < t) { l = 1000-512; }
        if (arr[l+256] < t) { l += 256; }
        if (arr[l+128] < t) { l += 128; }
        if (arr[l+64]  < t) { l += 64;  }
        if (arr[l+32]  < t) { l += 32;  }
        if (arr[l+16]  < t) { l += 16;  }
        if (arr[l+8]   < t) { l += 8;   }
        if (arr[l+4]   < t) { l += 4;   }
        if (arr[l+2]   < t) { l += 2;   }
        if (arr[l+1]   < t) { l += 1;   }
        p = l+1;
        if (p >= N || arr[p] != t) {
            p = -1;
        }
        printf("index: %d\n", p);
    }

    return 0;
}
