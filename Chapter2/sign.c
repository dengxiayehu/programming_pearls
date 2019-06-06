#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WORD_MAXLEN 32

int comp_char(const void* arg1, const void* arg2)
{
    return *((const char*)arg1) - *((const char*)arg2);
}

int main(int argc, char const* argv[])
{
    char word[WORD_MAXLEN] = { 0 };
    char sign[WORD_MAXLEN] = { 0 };

    while (scanf("%s", word) != EOF) {
        // Copy word into sign for sorting.
        strcpy(sign, word);
        // Use qsort for soring sign.
        qsort(sign, strlen(sign), sizeof(char), comp_char);

        printf("%s %s\n", sign, word);
    }

    return 0;
}
