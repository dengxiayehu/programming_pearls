#include <stdio.h>
#include <stdlib.h>

#define NELEM(arr) (sizeof(arr)/sizeof(arr[0]))

typedef int DataType;

typedef struct MinPQ {
    DataType* data;
    int n;
} MinPQ;

int swap(MinPQ* min_pq, int i, int j)
{
    DataType tmp = min_pq->data[i];
    min_pq->data[i] = min_pq->data[j];
    min_pq->data[j] = tmp;

    return 0;
}

int sink(MinPQ* min_pq, int i)
{
    while (2*i <= min_pq->n) {
        int c = 2*i; // left child node

        // Figure out whether right child node is even smaller.
        if (c < min_pq->n && min_pq->data[c] > min_pq->data[c+1]) {
            c++;
        }

        // Check whether this node is smaller than the children, if so, we stop.
        if (min_pq->data[i] <= min_pq->data[c]) {
            break;
        }

        // Swap the parent with smaller child.
        swap(min_pq, i, c);

        // Reconsider the previous smaller child.
        i = c;
    }

    return 0;
}

int extract_min(MinPQ* min_pq, DataType* t)
{
    // If queue is empty, return error.
    if (min_pq->n == 0) {
        return -1;
    }

    // Return the root node.
    if (t) {
        *t = min_pq->data[1];
    }

    // Exchange root node with data[n].
    swap(min_pq, 1, min_pq->n);

    // Queue's size minus 1.
    min_pq->n--;

    // Sink the new root to the right place.
    sink(min_pq, 1);

    return 0;
}

int main(int argc, char const* argv[])
{
    DataType arr[] = { 4, 5, 9, 1, 2, 10, 6, 3, 99, 46, 23, 11, -1, -3, -1 };
    MinPQ min_pq;
    int i;
    int k;
    DataType t;
    DataType sum = 0;

    if (argc != 3) {
        fprintf(stderr, "usage: %s <k> <t>\n", argv[0]);
        return -1;
    }

    k = atoi(argv[1]);
    t = atoi(argv[2]);

    min_pq.n = NELEM(arr);
    min_pq.data = (DataType*)malloc(sizeof(DataType)*(min_pq.n + 1));

    // Init the min heap.
    for (i = 0; i < min_pq.n; i++) {
        min_pq.data[i + 1] = arr[i];
    }
    // We need to do sink[n/2 .. 1].
    for (i = min_pq.n/2; i >= 1; i--) {
        sink(&min_pq, i);
    }

    for (i = 0; i < k; i++) {
        DataType tmp;
        if (extract_min(&min_pq, &tmp) < 0) {
            fprintf(stderr, "extract_min failed\n");
            goto out;
        }

        sum += tmp;
        printf("%d ", tmp);
    }
    printf("\n");

    if (sum < t) {
        printf("Found!\n");
    } else {
        printf("Not found!\n");
    }

out:
    free(min_pq.data);

    return 0;
}
