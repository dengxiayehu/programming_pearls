#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define M 5
#define N 5

#define POINT_SIZE 10

#define MKRC(x, y) ((x)<<4|(y))
#define GR(v) (((v)&0xff)>>4)
#define GC(v) ((v)&0xf)

#define NELEM(arr) (sizeof(arr)/sizeof((arr[0])))

typedef struct Point {
    int row;
    int col;
    int price;
} Point;

typedef struct Node {
    int row;
    int col;
    int pointnum;
    struct Node *next;
} Node;

/*
 *     列0  列1	列2	列3	列4
 *  行4 3           6   1
 *  行3     7			
 *  行2 8               4
 *  行1     0       2   
 *  行0 5               9
 */
int init_points(Point *points)
{
    const int coord[POINT_SIZE] = {
        MKRC(1, 1), MKRC(4, 4), MKRC(1, 3), MKRC(4, 0), MKRC(2, 4),
        MKRC(0, 0), MKRC(4, 3), MKRC(3, 1), MKRC(2, 0), MKRC(0, 4),
    };
    int i;

    srand((unsigned int)time(NULL));
    for (i = 0; i < NELEM(coord); i++) {
        points[i].row = GR(coord[i]);
        points[i].col = GC(coord[i]);
        points[i].price = rand()%100+1;
    }

    return 0;
}

int main(int argc, char const *argv[])
{
    Point *points;
    int ret = 0;
    int i;
    int k;
    Node *nodes[N];
    
    points = (Point*)malloc(sizeof(Point)*POINT_SIZE);

    // Init points array.
    if (init_points(points) < 0) {
        ret = 1;
        goto out;
    }

    // Print points array.
    for (i = 0; i < POINT_SIZE; i++) {
        printf("%d-%d %d$\n", points[i].row, points[i].col, points[i].price);
    }

    // Init the sparse matrix.
    memset(nodes, 0, sizeof(nodes));
    for (i = 0; i < N; i++) {
        for (k = 0; k < POINT_SIZE; k++) {
            if (points[k].col == i) {
                Node *node = (Node*)malloc(sizeof(Node));
                node->col = i;
                node->row = points[k].row;
                node->pointnum = k;
                node->next = NULL;

                if (nodes[i] == NULL) {
                    nodes[i] = node;
                } else {
                    node->next = nodes[i];
                    nodes[i] = node;
                }
            }
        }
    }

    // Print the sparse matrix.
    for (i = 0; i < N; i++) {
        Node *p = nodes[i];

        printf("col:%d => ", i);
        while (p) {
            printf("%d-%d %d$; ", p->row, p->col, points[p->pointnum].price);
            p = p->next;
        }
        printf("\n");
    }

    // Search the sparse matrix.
    for ( ; ; ) {
        int row;
        int col;
        Node *p;
        int res;

        printf("input row col: ");
        if (scanf("%d %d", &row, &col) == EOF) {
            break;
        }

        res = -1;
        p = nodes[col];
        while (p) {
            if (p->row == row) {
                res = p->pointnum;
                break;
            }
            p = p->next;
        }
        printf("%d\n", res);
    }

    // Free the sparse matrix.
    for (i = 0; i < N; i++) {
        Node *p = nodes[i], *q;

        while (p) {
            q = p->next;
            free(p);
            p = q;
        }
    }

out:
    free(points);

    return ret;
}
