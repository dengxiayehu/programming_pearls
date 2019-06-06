#include <stdio.h>
#include <string.h>

#define MAKE_DIGIT_BYTE(b0, b1, b2, b3, b4, b5, b6) \
    (((b0) ? 1<<0 : 0) | \
     ((b1) ? 1<<1 : 0) | \
     ((b2) ? 1<<2 : 0) | \
     ((b3) ? 1<<3 : 0) | \
     ((b4) ? 1<<4 : 0) | \
     ((b5) ? 1<<5 : 0) | \
     ((b6) ? 1<<6 : 0))

static const char digit_table[10] = {
    MAKE_DIGIT_BYTE(1, 0, 1, 1, 1, 1, 1), // 0
    MAKE_DIGIT_BYTE(0, 0, 0, 0, 1, 0, 1), // 1
    MAKE_DIGIT_BYTE(1, 1, 1, 0, 1, 1, 0), // 2
    MAKE_DIGIT_BYTE(1, 1, 1, 0, 1, 0, 1), // 3
    MAKE_DIGIT_BYTE(0, 1, 0, 1, 1, 0, 1), // 4
    MAKE_DIGIT_BYTE(1, 1, 1, 1, 0, 0, 1), // 5
    MAKE_DIGIT_BYTE(1, 1, 1, 1, 0, 1, 1), // 6
    MAKE_DIGIT_BYTE(0, 0, 1, 0, 1, 0, 1), // 7
    MAKE_DIGIT_BYTE(1, 1, 1, 1, 1, 1, 1), // 8
    MAKE_DIGIT_BYTE(0, 1, 1, 1, 1, 0, 1)  // 9
};

static const char* digit_graph[10] = {
    "11 2-1 ,31|2 1|,11 2-1 ",
    "14 ,33 1|,14 ",
    "11 2-1 ,13 1|,11 2-1 ,11|3 ,11 2-1 ",
    "11 2-1 ,13 1|,11 2-1 ,13 1|,11 2-1 ",
    "14 ,11|2 1|,11 2-1 ,13 1|,14 ",
    "11 2-1 ,11|3 ,11 2-1 ,13 1|,11 2-1 ",
    "11 2-1 ,11|3 ,11 2-1 ,11|2 1|,11 2-1 ",
    "11 2-1 ,33 1|",
    "11 2-1 ,11|2 1|,11 2-1 ,11|2 1|,11 2-1 ",
    "11 2-1 ,11|2 1|,11 2-1 ,13 1|,11 2-1 "
};

void print_graph(const char* codec_desc)
{
    // Parse the coded string.
    int row;
    int col;
    int ch;
    const char* p = codec_desc;
    char line[128];
    int i;
    int k;

    while (*p) {
        row = *p++ - '0';
        line[0] = '\0';
        k = 0;

        // Construct each line.
        while (*p) {
            col = *p++ - '0';
            ch = *p++;

            for (i = 0; i < col; i++) {
                k += snprintf(line + k, sizeof(line) - k, "%c", ch);
            }

            if (*p == ',') {
                // Skip ','.
                ++p;
                break;
            }
        }

        // Print each line.
        for (i = 0; i < row; i++) {
            printf("%s\n", line);
        }
    }
}

int main(int argc, char const* argv[])
{
    char str[6];

    while (scanf("%s", str) != EOF) {
        const char* p = str;

        // Print codec byte and graph of each digit.
        while (*p) {
            printf("0x%x\n", digit_table[*p - '0']);

            print_graph(digit_graph[*p - '0']);

            p++;
        }
        printf("\n");
    }

    return 0;
}
