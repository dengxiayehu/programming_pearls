#include <stdio.h>
#include <string.h>

int main(int argc, char const* argv[])
{
    // Char coded table, currently only 'I' supports.
    const char* coded_table[26] = {
        "", "", "", "", "", "", "", "", // 'A' ~ 'H'
        "39x,63 3x3 ,39x", // 'I'
        "", "", "", "", "", "", "", "", // 'J' ~ 'Q'
        "", "", "", "", "", "", "", "", "" // 'R' ~ 'Z'
    };
    char* str = "I";
    char line[19];
    int i;
    int k;

    while (*str) {
        // Locate the coded string for this character.
        const char* coded = coded_table[*str - 'A'];

        // Parse the coded string.
        int row;
        int col;
        int ch;
        const char* p = coded;
        while (*p) {
            row = *p++ - '0';
            line[0] = '\0';
            k = 0;

            // Construct each line.
            while (*p) {
                col = *p++ - '0';
                ch = *p++;

                for (i = 0; i < col; i++) {
                    k += snprintf(line + k, sizeof(line) - k, "%c ", ch);
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

        str++;
    }

    return 0;
}
