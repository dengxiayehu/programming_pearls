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
