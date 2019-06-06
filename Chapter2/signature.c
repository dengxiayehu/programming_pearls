#include <stdio.h>
#include <string.h>

#define WORD_MAXLEN 32

int main(int argc, char const* argv[])
{
    char word[WORD_MAXLEN] = { 0 };
    char sign[WORD_MAXLEN] = { 0 };
    char prev_sign[WORD_MAXLEN] = { 0 };

    while (scanf("%s %s", sign, word) != EOF) {
        if (!strcmp(sign, prev_sign) /*word has the same sign with previous one(s)*/ ||
            prev_sign[0] == '\0' /*this is the first word, print it anyway*/) {
            printf("%s ", word);
        } else { // word has different sign, print \n and it
            printf("\n%s ", word);
        }

        // Save sign.
        strcpy(prev_sign, sign);
    }

    printf("\n");

    return 0;
}
