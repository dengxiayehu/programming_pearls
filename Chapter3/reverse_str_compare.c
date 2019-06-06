#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NELEM(arr) (sizeof(arr)/sizeof(arr[0]))

void reverse_str(char* str)
{
    char* p = str;
    char* q = str + strlen(str) - 1;

    while (p < q) {
        char ch = *p;
        *p = *q;
        *q = ch;
        p++;
        q--;
    }
}

int main(int argc, char const* argv[])
{
    // 10 is the longest suffix's length + 1.
    char coded_table[][10] = {
        "et-ic", "al-is-tic", "s-tic", "p-tic", "-lyt-ic", "ot-ic", "an-tic",  
        "n-tic", "c-tic", "at-ic", "h-nic", "n-ic", "m-ic", "l-lic", "b-lic",
        "-clic", "l-ic",  "h-ic", "f-ic", "d-ic", "-bic", "a-ic", "-mac", "i-ac"
    };
    int i;
    char str[128];
    int match_index;

    // Reconstruct the coded_table.
    for (i = 0; i < NELEM(coded_table); i++) {
        char* p = coded_table[i];
        char* q = coded_table[i];

        // Remove the '-'.
        while (*q) {
            if (*q != '-') {
                *p++ = *q;
            }

            q++;
        }
        *p = '\0';

        // Reverse the suffix.
        reverse_str(coded_table[i]);
    }

    // 脚手架程序。
    while (scanf("%s", str) != EOF) {
        reverse_str(str);

        // Try the compare.
        match_index = -1;
        for (i = 0; i < NELEM(coded_table); i++) {
            char* p = str;
            char* q = coded_table[i];

            while (*p && *q) {
                if (*p != *q) {
                    break;
                }

                p++;
                q++;
            }
            if (*q == '\0') {
                // Suffix to the end, match.
                match_index = i;
                break;
            }
        }
        if (match_index >= 0) {
            printf("Match(%d) suffix reversed=%s\n", match_index, coded_table[match_index]);
        } else {
            printf("No match\n");
        }
    }

    return 0;
}
