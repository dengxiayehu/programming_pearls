#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

// NUM should be less than RAND_MAX which defined in stdlib.h.
// The value of RAND_MAX is INT_MAX.
#define NUM 10000000

int main(int argc, const char *argv[])
{
    int* numbers = NULL;
    int i = 0;
    FILE* fp = NULL;

    srand(time(NULL));

    numbers = (int*)malloc(sizeof(int)*NUM);
    if (!numbers) {
        perror("malloc failed");
        return 1;
    }

    // Prepare the sorted array, that is 0 --> NUM - 1.
    for (i = 0; i < NUM; i++) {
        numbers[i] = i;
    }

    // Do the shuffer.
    for (i = 0; i < NUM; i++) {
        // The shuffer algorithm, swap numbers[i] with numbers[i --> NUM - 1].
        int idx2swap = i + rand()%(NUM - i);

        int tmp = numbers[i];
        numbers[i] = numbers[idx2swap];
        numbers[idx2swap] = tmp;
    }

    // Open file to store the random numbers.
    fp = fopen("./800.txt", "w");

    // Print the result array.
    for (i = 0; i < NUM; i++) {
        if (numbers[i] >= 2000000) {
            // Only consider valid tele-phone numbers.
            fprintf(fp, "%d\n", numbers[i]);
        }
    }

    fclose(fp);

    free(numbers);

    return 0;
}
