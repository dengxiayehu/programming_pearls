### 数据决定程序结构

在进行程序编码时，通常会有各种各样的数据需要处理。有时会很急躁地进行编程，然后程序中会有很多的分支测试判断，既臃肿又很难扩展。当出现这样的情况时，我们可以跳出程序的框架束缚，集中心力来研究数据，通常会有奇效。可以考虑使用数组来集中数据，也可以使用一些高级工具，例如键值对、超文本、表格、专门领域的语言或工具来进行处理数据。

例如在进行日期问题的处理时，可以使用如下形式的二维数组
```
int table[2][12 + 1] = {
    { 365, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}, // not leap year
    { 366, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31} // is leap year
};

闰年天数 table[1][0]
平年天数 table[0][0]
闰年x月份天数 table[1][x]
平年x月份天数 table[0][x]
```

**集中相关的数据到数组然后集中处理，不要很多变量、很多if-else判断。**


### P1 工资税率计算
描述：工资计税的阶梯层次如下，试设计一个算法，输入税前收入，计算出需要报的税额。
```
if income <= 2200
    tax = 0;
else if income <= 2700
    tax = 0.14 * (income - 2200);
else if income <= 3200
    tax = 70 + 0.15 * (income - 2700);
else if income <= 3700
    tax = 145 + 0.16 * (income -3200);
else if income <= 4200
    tax =225 + 0.17 * (income - 3700);
```

分析：分段线性数据处理可以顺序搜索每个分段，若分段是递增有序的，也可以考虑使用二分搜索。
```
#include <stdio.h>
#include <stdlib.h>

#define TAX_RANGE_NUM 10

typedef struct TaxRange {
    int low_bound;
    float rate;
    int base_tax;
} TaxRange;

int main(int argc, char const* argv[])
{
    int income;
    int i;
    TaxRange* tax_range;
    int low, high;
    int range_idx;
    float tax;

    if (argc != 2) {
        fprintf(stderr, "usage: %s <income>\n", argv[0]);
        return 1;
    }

    income = atoi(argv[1]);

    // Alloc space for the tax table.
    tax_range = (TaxRange*)malloc(TAX_RANGE_NUM*sizeof(TaxRange));
    if (!tax_range) {
        perror("malloc failed");
        return 1;
    }
    
    // Init the tax table.
    tax_range[0].low_bound = 2200;
    tax_range[0].rate = 0.0f;
    tax_range[0].base_tax = 0;
    tax_range[1].low_bound = 2700;
    tax_range[1].rate = 0.14f;
    tax_range[1].base_tax = 0;
    for (i = 2; i < TAX_RANGE_NUM; i++) {
        tax_range[i].low_bound = tax_range[i-1].low_bound + 500;
        tax_range[i].rate = 0.13f + i*0.01f;
        tax_range[i].base_tax = tax_range[i-1].base_tax + 70 + (i-2)*5;
    }

    // Print the tax table.
    for (i = 0; i < TAX_RANGE_NUM; i++) {
        printf("%d,%.2f,%d ",
               tax_range[i].low_bound, tax_range[i].rate, tax_range[i].base_tax);
    }
    printf("\n");

    // Binary search the tax range for my income.
    range_idx = -1;
    low = 0;
    high = TAX_RANGE_NUM - 1;
    while (low <= high) {
        int mid = (low + high)/2;
        if (income <= tax_range[mid].low_bound) {
            if ((mid == 0) || (mid > 0 && tax_range[mid-1].low_bound < income)) {
                range_idx = mid;
                break;
            } else {
                high = mid - 1;
            }
        } else {
            low = mid + 1;
        }
    }
    printf("The range index = %d\n", range_idx);

    // Calc the tax.
    tax = tax_range[range_idx].base_tax
        + (income - tax_range[range_idx].low_bound + 500)*tax_range[range_idx].rate;
    printf("The tax is: %.2f\n", tax);

    free(tax_range);

    return 0;
}
```
> 输出：  
./tax_calc_run 0  
2200,0.00,0 2700,0.14,0 3200,0.15,70 3700,0.16,145 4200,0.17,225 4700,0.18,310 5200,0.19,400 5700,0.20,495 6200,0.21,595 6700,0.22,700   
The range index = 0  
The tax is: 0.00  
./tax_calc_run 2701  
2200,0.00,0 2700,0.14,0 3200,0.15,70 3700,0.16,145 4200,0.17,225 4700,0.18,310 5200,0.19,400 5700,0.20,495 6200,0.21,595 6700,0.22,700   
The range index = 2  
The tax is: 70.15  


### P2 编写banner函数

分析：这题的难点在如何编码一个大写字母。我们就以大写字母I为例，输出它。
```
x  x  x  x  x  x  x  x  x
x  x  x  x  x  x  x  x  x
x  x  x  x  x  x  x  x  x
         x  x  x         
         x  x  x         
         x  x  x         
         x  x  x         
         x  x  x         
         x  x  x         
x  x  x  x  x  x  x  x  x
x  x  x  x  x  x  x  x  x
x  x  x  x  x  x  x  x  x
```

这个大写字母I可以编码为：`39x,63 3x3 ,39x`，我们建立一个字母到编码的映射，然后解析编码并打印。

```
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
```
> 输出结果如上面的I所示。


### P3 英文单词连字符匹配

分析：这个问题是对字符串进行“尾部匹配”，有个技巧，将字符串（输入串与后缀模版串）进行倒序，然后就可以从头往后进行匹配了。

**这个程序主要是考察后缀模版数组的使用，以及对它进行的一些预处理来加速后续的处理。**

```
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
```
> 输出：  
./reverse_str_compare_run  
fantastic  
Match(2) suffix reversed=cits  
hello  
No match  


### P4 格式信函发生器

类比MVC模式，我们可以将数据模型M与控制C分离开来，分别存储。格式模版如下： 
``` 
Hi $0:
I am $1, nice to meet you.
Yours, date: $2.
```

我们将其存储在一个名为template.txt的文件中。

数据放到名为data.txt的文件中，格式为：
```
Tom Bob 2019-03-02
Alice Bob 2019-03-02

```

```
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
```
> 运行结果：
```
=== Template's content is: 
Hi $0:
I am $1, nice to meet you.
Yours, date: $2.
=== Line data is: 
Tom Bob 2019-03-02
field[0]=Tom
field[1]=Bob
field[2]=2019-03-02
=== Constructed letter is: 
Hi Tom:
I am Bob, nice to meet you.
Yours, date: 2019-03-02.
=== Line data is: 
Alice Bob 2019-03-02
field[0]=Alice
field[1]=Bob
field[2]=2019-03-02
=== Constructed letter is: 
Hi Alice:
I am Bob, nice to meet you.
Yours, date: 2019-03-02.
```


### P5 七段显示设备

```
七段数码管上每一块对应在字节编码中的比特位
   2
   --
 3|1 |4  
   --
 5|0 |6
   --
```

通过上图，可知`0~9`这10个数字对应的显示段为：  
0：0 2 3 4 5 6  
1：4 6  
2：0 1 2 4 5  
3：0 1 2 4 6  
4：1 3 4 6  
5：0 1 2 3 6  
6：0 1 2 3 5 6  
7：2 4 6  
8：0 1 2 3 4 5 6  
9：1 2 3 4 6  

每一个数字的图形化输出也可以参考P2使用编码：  
0："11 2-1 ,31|2 1|,11 2-1 "  
1："14 ,33 1|,14 "  
2："11 2-1 ,13 1|,11 2-1 ,11|3 ,11 2-1 "  
3："11 2-1 ,13 1|,11 2-1 ,13 1|,11 2-1 "  
4："14 ,11|2 1|,11 2-1 ,13 1|,14 "  
5："11 2-1 ,11|3 ,11 2-1 ,13 1|,11 2-1 "  
6："11 2-1 ,11|3 ,11 2-1 ,11|2 1|,11 2-1 "  
7："11 2-1 ,33 1|"  
8："11 2-1 ,11|2 1|,11 2-1 ,11|2 1|,11 2-1 "  
9："11 2-1 ,11|2 1|,11 2-1 ,13 1|,11 2-1 "  

```
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
```

输出结果：
```
./seven_display_run
12345
0x50
    
   |
   |
   |
    
0x37
 -- 
   |
 -- 
|   
 -- 
0x57
 -- 
   |
 -- 
   |
 -- 
0x5a
    
|  |
 -- 
   |
    
0x4f
 -- 
|   
 -- 
   |
 -- 

67890
0x6f
 -- 
|   
 -- 
|  |
 -- 
0x54
 -- 
   |
   |
   |
0x7f
 -- 
|  |
 -- 
|  |
 -- 
0x5e
 -- 
|  |
 -- 
   |
 -- 
0x7d
 -- 
|  |
|  |
|  |
 -- 
```
