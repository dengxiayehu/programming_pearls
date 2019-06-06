#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

#define NELEM(arr) (sizeof(arr)/sizeof(arr[0]))

typedef struct Node {
    int num;
    int win_idx;
} Node;

int main(int argc, const char *argv[])
{
    int num_arr[] = { 3, 4, 1, 6, 2, 8, 7, 9 };
    const int num_arr_len = NELEM(num_arr);
    int n = 1;
    int i = 0;
    int adjust_idx = 0;

    // Leaf number must be multiple of 2.
    while (n < num_arr_len) {
        n <<= 1;
    }

    // Alloc space for tournament algorithm
    Node* nodes = (Node*)malloc(sizeof(Node)*(2*n - 1));
    if (!nodes) {
        perror("malloc failed");
        return 1;
    }

    // Init leaf nodes.
    for (i = n - 1; i < 2*n - 1; i++) {
        int idx = i - (n - 1);

        if (idx < num_arr_len) {
            // Original item in array.
            nodes[i].num = num_arr[idx];
            nodes[i].win_idx = i;
        } else {
            // Extended item, set num to INT_MAX to avoid winning.
            nodes[i].num = INT_MAX;
            nodes[i].win_idx = -1;
        }
    }

    // Built the tree.
    for (i = n - 2; i >= 0; i--) {
        // We should compare from the end of nodes, that is 2*n-2 and 2*n-3.
        // 2*n-2 = 2*(n-1), 2*n-3=2*(n-1)-1 for n.
        // Now we iterate from i=(n-2), hense r=2*(i+1)=2*i+2, l=2*(i+1)-1=2*i+1.
        int r = 2*i + 1;
        int l = 2*i + 2;
        if (nodes[r].num < nodes[l].num) { // r wins (smaller)
            nodes[i] = nodes[r];
        } else { // l wins.
            nodes[i] = nodes[l];
        }
    }

    printf("The built tree is: ");
    for (i = 0; i < 2*n - 1; i++) {
        printf("%d:%d ", nodes[i].num, nodes[i].win_idx);
    }
    printf("\n");

    for (i = 0; i < num_arr_len; i++) {
        // Print the smallest num in root node.
        printf("%d:%d ", nodes[0].num, nodes[0].win_idx);

        // Set the winner-leaf's num to INT_MAX, so it won't win again in the adjustment.
        adjust_idx = nodes[0].win_idx;
        nodes[adjust_idx].num = INT_MAX;
        // Adjust the previous winner-leaf, from end to beginning.
        while (adjust_idx != 0) {
            int competitor_idx = 0;
            int parent_idx = 0;

            if (adjust_idx%2 == 0) { // parent's left node
                competitor_idx = adjust_idx - 1;
                parent_idx = adjust_idx/2 - 1;
            } else { // parent's right node
                competitor_idx = adjust_idx + 1;
                parent_idx = adjust_idx/2;
            }

            // Rematch again.
            if (nodes[competitor_idx].num < nodes[adjust_idx].num) {
                // Competitor wins.
                nodes[parent_idx] = nodes[competitor_idx];
            } else {
                // Me!
                nodes[parent_idx] = nodes[adjust_idx];
            }

            adjust_idx = parent_idx;
        }
    }

    printf("\n");

    free(nodes);

    return 0;
}
