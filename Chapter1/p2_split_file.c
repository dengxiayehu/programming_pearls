#include <stdio.h>
#include <stdlib.h>

#define MEMORY_MAX      1024*1024 // 1MB
#define MEMORY_NUMS_MAX MEMORY_MAX/4 // int 4 bytes

// Non decreasing order.
static int cmp(const void* a, const void* b)
{
    return (*(int*)a - *(int*)b);
}


int main(int argc, const char *argv[])
{
    FILE* fp = fopen("./800.txt", "r");
    int num = 0;
    int nums = 0;
    int nums_file = 0;
    int need_split = 1;
    FILE* fp_split = NULL;
    int* num_arr = NULL;

    num_arr = (int*)malloc(sizeof(int)*MEMORY_NUMS_MAX);
    if (!num_arr) {
        perror("malloc failed");
        return 1;
    }

    if (!fp) {
        perror("fopen 800.txt failed");
        return 1;
    }

    for ( ; ; ) {
        if (need_split) {
            // Close previous split file if opend.
            if (fp_split) {
                fclose(fp_split);
            }

            char tmp_buf[32];
            snprintf(tmp_buf, sizeof(tmp_buf), "800_split_%d.txt", nums_file);
            printf("spliting file %s\n", tmp_buf);
            fp_split = fopen(tmp_buf, "w");
            if (!fp_split) {
                perror("fopen split file failed");
                break;
            }
            need_split = 0;
            nums_file++;
        }

        if (fscanf(fp, "%d\n", &num) == EOF) {
            if (ferror(fp)) {
                perror("fscanf split file failed");
            }
            break;
        }

        num_arr[nums] = num;
        ++nums;
        if (nums > MEMORY_NUMS_MAX) {
            int k = 0;

            // Do the sort in memory.
            qsort(num_arr, nums, sizeof(int), cmp);

            for (k = 0; k < nums; k++) {
                fprintf(fp_split, "%d\n", num_arr[k]);
            }

            nums = 0;
            need_split = 1;
        }
    }

    if (fp) {
        fclose(fp);
    }
    if (fp_split) {
        fclose(fp_split);
    }

    free(num_arr);

    return 0;
}
