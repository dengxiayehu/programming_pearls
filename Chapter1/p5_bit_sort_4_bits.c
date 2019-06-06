#include <stdio.h>

#define BITSPERWORD 32
#define SHIFT 3
#define MASK 0x07

#define N 10000000

static void set(unsigned int a[], int i)
{
    a[i>>SHIFT] += 1<<(4*(i&MASK));
}

static void clr(unsigned int a[], int i)
{
    a[i>>SHIFT] &= ~(0x0F<<(4*(i&MASK)));
}

static int test(unsigned int a[], int i)
{
    return (a[i>>SHIFT] & (0x0F<<(4*(i&MASK))))>>(4*(i&MASK));
}

int main(int argc, const char *argv[])
{
    // BITSPERWORD only contains 8 numbers, not 32 numbers, so the size must x4.
    unsigned int a[1 + N/BITSPERWORD*4];
    FILE* fp = NULL;
    FILE* fp_out = NULL;
    int i = 0;

    fp = fopen("800.txt", "r");
    if (!fp) {
        perror("fopen 800.txt failed");
        return 1;
    }

    fp_out = fopen("800_sorted_p5.txt", "w");
    if (!fp_out) {
        perror("fopen 800_sorted_p5.txt failed");
        return 1;
    }

    // Clear the bit vector.
    for (i = 0; i < N; i++) {
        clr(a, i);
    }
    printf("clear the bit vector done\n");

    // Read from file and set into bit vector.
    for ( ; ; ) {
        int num = 0;
        if (fscanf(fp, "%d\n", &num) != 1) {
            if (ferror(fp)) {
                perror("fscanf failed");
            }
            break;
        }

        set(a, num);
    }
    printf("load numbers from bit vector done\n");

    for (i = 0; i < N; i++) {
        int j = 0;
        int n = test(a, i);

        for (j = 0; j < n; j++) {
            fprintf(fp_out, "%d\n", i);
        }
    }

    fclose(fp_out);

    fclose(fp);

    return 0;
}
