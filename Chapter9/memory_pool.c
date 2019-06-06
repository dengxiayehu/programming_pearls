#include <stdio.h>
#include <stdlib.h>

#define NODEGROUP 100
#define NODESIZE 8

typedef struct ListNode {
    void *p;
    struct ListNode *next;
} ListNode;

typedef struct MemoryPool {
    ListNode *freenodes;
    ListNode *groups;
    int nodesleft;
    void *(*alloc)(struct MemoryPool *mp, int size);
    int (*free)(struct MemoryPool *mp, void *p);
} MemoryPool;

static void *alloc_node(ListNode **phead)
{
    void *res = (*phead)->p = (char*)(*phead)+sizeof(ListNode);
    (*phead) = (*phead)->next;

    return res;
}

static int free_node(ListNode **phead, ListNode *node)
{
    if (*phead == NULL) {
        *phead = node;
        node->next = NULL;
    } else {
        node->next = *phead;
        *phead = node;
    }

    return 0;
}

static void *my_alloc(MemoryPool *mp, int size)
{
    int i;

    if (size != NODESIZE) {
        printf("use malloc\n");
        return malloc(size);
    }

    if (mp->nodesleft == 0) {
        // No free node space left, need to alloc a group of nodes.
        void *group = malloc(NODEGROUP*(sizeof(ListNode)+NODESIZE));
        if (!group) {
            perror("malloc failed");
            return NULL;
        }

        for (i = 0; i < NODEGROUP; i++) {
            ListNode *node = (ListNode*)((char*)group+i*(sizeof(ListNode)+NODESIZE));
            node->p = ((char*)node+sizeof(ListNode));
            free_node(&mp->freenodes, node);
        }

        mp->nodesleft = NODEGROUP;
        ListNode *gnode = (ListNode*)malloc(sizeof(ListNode));
        gnode->p = group;
        free_node(&mp->groups, gnode);
    }

    // Fetch a free node from pool, and erase it from the pool.
    void *res = alloc_node(&mp->freenodes);
    mp->nodesleft--;
    
    return res;
}

static int my_free(MemoryPool *mp, void *p)
{
    // Check whether p is alloc from pool.
    int my_alloced = 0;

    ListNode *node = mp->groups;
    while (node) {
        if (p >= node->p && p < (void*)((char*)node->p+(NODEGROUP*(sizeof(ListNode)+NODESIZE)))) {
            my_alloced = 1;
            break;
        }
        node = node->next;
    }
    if (!my_alloced) {
        printf("use free\n");
        free(p);
        return 0;
    }

    node = (ListNode*)((char*)p-sizeof(ListNode));
    if (!node->p) {
        fprintf(stderr, "double free detected!\n");
        return -1;
    }

    free_node(&mp->freenodes, node);
    node->p = NULL; // set p to NULL to detect double free
    mp->nodesleft++;

    return 0;
}

// 创建MemoryPool并进行初始化。
MemoryPool *create_memory_pool()
{
    MemoryPool *mp = (MemoryPool*)malloc(sizeof(MemoryPool));
    if (!mp) {
        perror("malloc MemoryPool failed");
        return NULL;
    }

    mp->freenodes = NULL;
    mp->groups = NULL;
    mp->nodesleft = 0;
    mp->alloc = my_alloc;
    mp->free = my_free;
    
    return mp;
}

int destroy_memory_pool(MemoryPool **mp)
{
    ListNode *p, *q;

    if (!*mp) {
        return 0;
    }

    p = (*mp)->groups;
    while (p) {
        q = p->next;
        free(p->p);
        free(p);
        p = q;
    }

    free(*mp);
    *mp = NULL;

    return 0;
}

int main(int argc, char const *argv[])
{
    MemoryPool *mp;
    void *p[19990];
    int i;

    mp = create_memory_pool();

    for (i = 0; i < 3000; i++) {
        p[i] = mp->alloc(mp, 8);
    }

    p[3000] = mp->alloc(mp, 9877);

    for (i = 0; i < 200; i++) {
        p[3000+1+i] = mp->alloc(mp, 8);
    }

    for (i = 0; i < 3201; i++) {
        mp->free(mp, p[i]);
    }

    destroy_memory_pool(&mp);

    return 0;
}
