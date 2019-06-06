#include <stdio.h>
#include <assert.h>

#define isdigit(x) ((x) >= '0' && (x) <= '9')
#define isupper(x) ((x) >= 'A' && (x) <= 'Z')
#define islower(x) ((x) >= 'a' && (x) <= 'z')
#define isalpha(x) (isupper(x) || islower(x))
#define isalphanum(x) (isalpha(x) || isdigit(x))

#define ORD_CHR 0
#define LOWER 1
#define UPPER 2
#define DIGIT 4
 
unsigned char hash_table[256];

static void preprocess()
{
    int i;

    for (i = '0'; i <= '9'; i++) {
        hash_table[i] |= DIGIT;
    }
    for (i = 'A'; i <= 'Z'; i++) {
        hash_table[i] |= UPPER;
        hash_table[i+'a'-'A'] |= LOWER;
    }
}

#define isdigit1(x) (hash_table[x]&DIGIT)
#define isupper1(x) (hash_table[x]&UPPER)
#define islower1(x) (hash_table[x]&LOWER)
#define isalpha1(x) (hash_table[x]&(UPPER|LOWER))
#define isalphanum1(x) (hash_table[x]&(UPPER|LOWER|DIGIT))

int main(int argc, char const *argv[])
{
    int i;

    preprocess();


    for (i = 0; i < 10; i++) {
        assert(isdigit('0'+i));
        assert(isalphanum('0'+i));
        assert(isdigit1('0'+i));
        assert(isalphanum1('0'+i));
    }

    for (i = 0; i < 26; i++) {
        char ch = 'A'+i;
        assert(isupper(ch));
        assert(islower(ch+32));
        assert(isalphanum(ch));
        assert(isupper1(ch));
        assert(islower1(ch+32));
        assert(isalphanum1(ch));
    }

    return 0;
}
