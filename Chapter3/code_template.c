#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define TEMPLATE_FILE "./template.txt"
#define DATA_FILE "./data.txt"

int construct_letter(const char* template, char* data, char* constructed)
{
    char* p;
    char* q;
    char* fields[3] = { NULL };
    int i;
    int n;
    const char* pc;

    constructed[0] = '\0';

    // Split data info fields.
    p = data;
    n = 0;
    while (*p) {
        // Skip leading blanks.
        while (*p && isspace(*p)) {
            p++;
        }

        // Locate this field.
        if (*p) {
            fields[n++] = p;
        }

        // Locate to the end of this field.
        while (*p && !isspace(*p)) {
            p++;
        }

        // Seperate each field in data.
        if (*p) {
            *p++ = '\0';
        }
    }

    // Print the fields.
    for (i = 0; i < n; i++) {
        printf("field[%d]=%s\n", i, fields[i]);
    }

    pc = template;
    q = constructed;
    while (*pc) {
        if (*pc == '$') { // $ case
            if (*(pc+1) && *(pc+1) == '$') { // $$ case
                *q++ = *pc;
                pc += 2;
            } else if (*(pc+1) && isdigit(*(pc+1))) { // $n case
                i = *(pc+1) - '0';
                if (i >= n) {
                    fprintf(stderr, "Bad schema, fields\n");
                    return -1;
                }

                // Replace fields[i] with $i.
                p = fields[i];
                while (*p) {
                    *q++ = *p++;
                }

                pc += 2;
            } else {
                fprintf(stderr, "Bad schema\n");
                return -1;
            }
        } else {
            *q++ = *pc++;
        }
    }
    *q = '\0';

    return 0;
}

int main(int argc, char const* argv[])
{
    FILE* fp_template;
    FILE* fp_data;
    char template[4096] = { 0 };
    char line[512];
    char constructed[4096];

    fp_template = fopen(TEMPLATE_FILE, "r");
    if (!fp_template) {
        perror("fopen failed");
        return 1;
    }

    fp_data = fopen(DATA_FILE, "r");
    if (!fp_data) {
        perror("fopen failed");
        return 1;
    }

    // Read the entire content from template.txt.
    while (fgets(line, sizeof(line), fp_template)) {
        strcat(template, line);
    }
    printf("=== Template's content is: \n");
    printf("%s\n", template);

    while (fgets(line, sizeof(line), fp_data)) {
        printf("=== Line data is: \n");
        printf("%s", line);

        if (construct_letter(template, line, constructed) < 0) {
            fprintf(stderr, "construct_letter failed\n");
            return 1;
        }
        printf("=== Constructed letter is: \n");
        printf("%s\n", constructed);
    }

    fclose(fp_template);
    fclose(fp_data);

    return 0;
}
