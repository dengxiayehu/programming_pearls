#include <stdio.h>

#define BITSPERWORD 32 // use int array to represent bit vector
#define SHIFT 5 // div 32
#define MASK 0x1f // mask, the value of this is 31

#define N 10000000 // numbers to sort

// i>>SHIFT locates the int element in array.
// i&MASK figures out value within 32.
// i<<(i&MASK) set the bit of value left shift.
static void set(int a[], int i)
{
    a[i>>SHIFT] |= (1<<(i&MASK));
}

static void clr(int a[], int i)
{
    a[i>>SHIFT] &= ~(1<<(i&MASK));
}

static int test(int a[], int i)
{
    return a[i>>SHIFT] & (1<<(i&MASK));
}

int main(int argc, const char *argv[])
{
    int a[1 + N/BITSPERWORD];
    int i;
    int arr[11] = { 0, 34, 2, 3, 31,1,63, 4, 62, 33, 64 };

    // Clear the bit vector.
    for (i = 0; i < N; i++) {
        clr(a, i);
    }

    // Set the bit vector.
    for (i = 0; i < sizeof(arr)/sizeof(arr[0]); i++) {
        set(a, arr[i]);
    }

    // Print the sorted result.
    for (i = 0; i < N; i++) {
        if (test(a, i)) { // value i set
            printf("%d ", i);
        }
    }

    printf("\n");

    return 0;
}
