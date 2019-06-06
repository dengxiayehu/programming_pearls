### 调试技术

#### LRU（Least Recently Used）算法
最近最少使用，淘汰最近不使用的数据。算法根据数据的历史访问记录来进行淘汰数据，其核心思想是：“如果数据最近被访问过，那么将来被访问的几率也更高”。

最常见的实现是使用一个链表保存缓存数据，详细算法实现如下：
1. 新数据插入到链表的头部；
2. 每当缓存命中（即缓存数据被访问），则将数据移动到链表头部；
3. 当链表满的时候，将链表尾部的数据丢弃。

通常LRU算法会将链表与HashMap结合起来使用。
```
#include <iostream>
#include <string>
#include <map>

using namespace std;

template <typename Key, typename Value>
class LRU {
public:
    LRU(int capacity);
    ~LRU();

    int put(Key key, Value val);
    Value *get(Key key);

private:
    struct CachedNode {
        Key key;
        Value val;
        CachedNode *prev;
        CachedNode *next;

        CachedNode(Key k, Value v)
            : key(k), val(v), prev(NULL), next(NULL) { }
    };

    int add_head(CachedNode *cached_node);
    int remove(CachedNode *cached_node);

private:
    typedef map<Key, CachedNode*> Map;
    typedef typename Map::iterator MapIterator;
    const int capacity_;
    Map map_;
    CachedNode *head_;
    CachedNode *tail_;
};

template <typename Key, typename Value>
LRU<Key, Value>::LRU(int capacity)
    : capacity_(capacity)
    , head_(NULL)
    , tail_(NULL)
{
}

template <typename Key, typename Value>
LRU<Key, Value>::~LRU()
{
    CachedNode *p = head_, *q;
    while (p) {
        q = p->next;
        delete p;
        p = q;
    }
}

template <typename Key, typename Value>
int LRU<Key, Value>::put(Key key, Value val)
{
    MapIterator it = map_.find(key);
    if (it != map_.end()) {
        // key-val exists, update the val and move cached_node to the head of list.
        CachedNode *cached_node = it->second;
        cached_node->val = val;
        remove(cached_node);
        add_head(cached_node);
    } else {
        // New cached_node generated, check if need to remove the oldest cached_node from the tail of list.
        CachedNode *cached_node = new CachedNode(key, val);
        if (map_.size() >= capacity_) {
            MapIterator it1 = map_.find(tail_->key);
            CachedNode *tail_bak = tail_;
            remove(tail_); // note: remove() function will probably modify tail_
            delete tail_bak;
            map_.erase(it1);
        }
        add_head(cached_node);
        map_[key] = cached_node;
    }

    return 0;
}

template <typename Key, typename Value>
Value *LRU<Key, Value>::get(Key key)
{
    MapIterator it = map_.find(key);
    if (it != map_.end()) {
        CachedNode *cached_node = it->second;
        remove(cached_node);
        add_head(cached_node);
        return &cached_node->val;
    }

    return NULL;
}

// 将节点添加到链表头。注意：这个链表并不是环形的。
template <typename Key, typename Value>
int LRU<Key, Value>::add_head(LRU::CachedNode *cached_node)
{
    cached_node->next = head_;
    cached_node->prev = NULL;

    if (head_ != NULL) {
        head_->prev = cached_node;
    }
    head_ = cached_node;

    if (tail_ == NULL) {
        tail_ = head_;
    }

    return 0;
}

template <typename Key, typename Value>
int LRU<Key, Value>::remove(LRU::CachedNode *cached_node)
{
    if (cached_node->prev != NULL) {
        cached_node->prev->next = cached_node->next;
    } else {
        head_ = cached_node->next;
    }

    if (cached_node->next != NULL) {
        cached_node->next->prev = cached_node->prev;
    } else {
        tail_ = cached_node->prev;
    }

    return 0;
}

int main(int argc, char const *argv[])
{
    LRU<int, string> lru(2);
    lru.put(1, "This is 1");
    lru.put(2, "This is 2");

    string *val = lru.get(1);
    cout << "Value of key 1 is: " << (val ? *val : "NULL") << endl;

    lru.put(4, "This is 4");

    val = lru.get(2);
    cout << "Value of key 2 is: " << (val ? *val : "NULL") << endl;

    val = lru.get(1);
    cout << "Value of key 1 is: " << (val ? *val : "NULL") << endl;

    return 0;
}
```
> 输出结果：  
./lru_run  
Value of key 1 is: This is 1  
Value of key 2 is: NULL  
Value of key 1 is: This is 1  

#### LFU（Least Frequently Used）算法
次数最少使用，淘汰使用数据最少的数据。算法根据数据的历史访问频率来淘汰数据，其核心思想是：“如果数据过去被访问多次，那么将来被访问的频率也更高”。

LFU的每个数据块都有一个引用技术，所有数据块按照引用计数排序，具有相同引用计数的数据则按照时间排序，详细算法实现如下：
1. 新加入数据插入到队列尾部（因为引用计数为1）；
2. 队列中的数据被访问后，引用计数增加，队列重新排序；
3. 当需要淘汰数据时，将已经排序的列表最后的数据删除。

实现LFU置换算法，置换规则是当容量满时换出使用频率最少的那个，不考虑一定时间内的频率的情况下，可以采用每个页使用的次数作为判断依据，相当于从创建之处到现在的使用频率。

类似的置换算法为了在效率上有一定的保障，通常都是空间换时间，本题也明确规定的时间复杂度是O(1)，而O(1)多数情况下都是类似map的存储结构，当然，C++中map和set是采用红黑树实现的，效率是O(lgN)，而另一个采用hashtable实现的unordered_map和unordered_set则是O(1)的效率。

接下来考虑采用map保存的数据：  
首先为了根据键key找到值value和它的使用频率freq，可以考虑保存一个类似unordered_map<int, std::pair<int, int>>的结构代表键到\<值，频率>的映射。称为keyToVF。

接下来考虑当容量满时进行置换的情况，为了让效率进一步提升，可以借鉴类似操作系统内核处理不同状态进程控制块时的策略，即每种状态的进程控制块维护一个链表，这里就只需为每个频率维护一个链表，再存储在map中，即unordered_map<int, list<int>>结构代表频率freq到保存键的链表的映射。称为fToList。

但是这样仍然不够，没有办法确定某一时刻最小的频率是多少，所以还需要使用一个变量记录当前最小使用频率minF，当置换时，直接从fToList[minF]中移除一个页，这里可以事先规定，新加入的都添加到链表尾部，移除则移除头部。

当调用get函数时需要对键key的频率进行增加，从keyToVF中可以找到key对应的值和频率，从fToList中可以找到频率对应的链表，需要做的事情是从这个链表中删除key，然后添加到fToList[freq+1]这个链表中。由于链表删除与key相等的节点时需要遍历链表，而删除指定迭代器处的节点时只需要改变几个指针，所以为了更进一步提高速度，再次使用一个map保存键key到key在链表中迭代器的映射即unordered_map<int, list<int>::iterator>。称为keyToIt。

至此三个map都已经构造完成，get和put的工作只是在线性时间操作不同的map而已。  
参考：<https://blog.csdn.net/sinat_35261315/article/details/79243834>

```
#include <iostream>
#include <unordered_map>
#include <list>
#include <string>

using namespace std;

template <typename Key, typename Value>
class LFU {
public:
    LFU(int capacity);
    ~LFU();

    int put(Key key, Value val);
    Value *get(Key key);

private:
    typedef typename unordered_map<Key, std::pair<Value, int> >::iterator Key2VFIterator;
    typedef typename unordered_map<int, list<Key> >::iterator F2ListIterator;
    typedef typename list<Key>::iterator ListIterator;
    typedef typename unordered_map<Key, ListIterator>::iterator Key2ITIterator;

    int capacity_;
    int min_f_;
    int size_;
    unordered_map<Key, std::pair<Value, int> > key2vf_;
    unordered_map<int, list<Key> > f2list_;
    unordered_map<Key, ListIterator> key2it_;
};

template <typename Key, typename Value>
LFU<Key, Value>::LFU(int capacity)
    : capacity_(capacity)
    , min_f_(1)
    , size_(0)
{
}

template <typename Key, typename Value>
LFU<Key, Value>::~LFU()
{
}

template <typename Key, typename Value>
int LFU<Key, Value>::put(Key key, Value val)
{
    if (capacity_ == 0) {
        cout << "capacity is inited to 0, cant put" << endl;
        return 0;
    }

    if (get(key) != NULL) {
        // This key-value is already in the cache, return the value.
        val = key2vf_[key].first;
        return 0;
    }

    if (size_ == capacity_) {
        // Cache pool is full, remove the lfu.
        key2vf_.erase(f2list_[min_f_].front()); // remove from key2vf_
        key2it_.erase(f2list_[min_f_].front()); // remove from key2it_
        f2list_[min_f_].pop_front(); // remove from f2list_
        size_--;
    }

    // Add this key-val.
    key2vf_[key] = make_pair<Value, int>(val, 1);
    f2list_[1].push_back(key);
    key2it_[key] = --f2list_[1].end();

    min_f_ = 1;
    size_++;

    return 0;
}

template <typename Key, typename Value>
Value *LFU<Key, Value>::get(Key key)
{
    if (key2vf_.find(key) == key2vf_.end()) {
        // This is key is never put.
        return NULL;
    }
    // Reference Value Freq.
    Value &val = key2vf_[key].first;
    int &freq = key2vf_[key].second;

    // Erase from f2list_ with freq.
    f2list_[freq].erase(key2it_[key]);

    // Add to f2list_ to the tail with freq+1.
    f2list_[++freq].push_back(key);

    // Add to key2it.
    key2it_[key] = --f2list_[freq].end();

    // Update min frequency.
    if (f2list_[min_f_].empty()) {
        ++min_f_;
    }

    return &val;
}

int main(int argc, char const *argv[])
{
    LFU<int, string> lfu(2);
    lfu.put(1, "hello 1");
    lfu.put(2, "hello 2");

    string* tmp;
    tmp = lfu.get(1);
    cout << "lfu.get(1) = " << (tmp ? *tmp : "NULL") << endl;

    lfu.put(3, "hello 3");

    tmp = lfu.get(2);
    cout << "lfu.get(2) = " << (tmp ? *tmp : "NULL") << endl;

    tmp = lfu.get(3);
    cout << "lfu.get(3) = " << (tmp ? *tmp : "NULL") << endl;

    lfu.put(4, "hello 4");

    tmp = lfu.get(1);
    cout << "lfu.get(1) = " << (tmp ? *tmp : "NULL") << endl;

    tmp = lfu.get(3);
    cout << "lfu.get(3) = " << (tmp ? *tmp : "NULL") << endl;

    tmp = lfu.get(4);
    cout << "lfu.get(4) = " << (tmp ? *tmp : "NULL") << endl;

    return 0;
}
```
> 输出结果：  
./lfu_run  
lfu.get(1) = hello 1  
lfu.get(2) = NULL  
lfu.get(3) = hello 3  
lfu.get(1) = NULL  
lfu.get(3) = hello 3  
lfu.get(4) = hello 4  


#### 线性查找优化版本1（使用哨兵元素）

这个算法比较简单，通过在数组有效元素紧接着放置一个哨兵节点，可以省略一次元素有效区间的判断，增加了效率。

```
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main(int argc, char const *argv[])
{
    int *arr;
    int i;
    int n;
    int t;
    int hold;

    printf("input n: ");
    if (scanf("%d", &n) == EOF) {
        return 0;
    }
    assert(n > 0);

    arr = (int*)malloc((n+1)*sizeof(int)); // +1 for the guard
    assert(arr);
    // Init the arr, 1, 2, 3, ... n.
    for (i = 0; i < n; i++) {
        arr[i] = i+1;
    }

    for ( ; ; ) {
        printf("input t: ");
        if (scanf("%d", &t) == EOF) {
            break;
        }

        hold = arr[n]; // backup the element which will be replaced by the guard
        arr[n] = t; // set the guard
        for (i = 0; ; i++) {
            if (arr[i] == t) {
                break;
            }
        }
        arr[n] = hold; // recover the backup element
        if (i == n) {
            printf("not found\n");
        } else {
            printf("found with index: %d\n", i);
        }
    }

    free(arr);

    return 0;
}
```


#### 线性查找优化版本2（使用哨兵元素+部分循环展开）

对于现代计算机系统来说，将循环展开则有助于避免管道阻塞、减少分之、增加指令级的并行性。

```
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main(int argc, char const *argv[])
{
    int *arr;
    int i;
    int n;
    int t;
    int hold;

    printf("input n: ");
    if (scanf("%d", &n) == EOF) {
        return 0;
    }
    assert(n > 0);

    arr = (int*)malloc((n+1)*sizeof(int)); // +1 for the guard
    assert(arr);
    // Init the arr, 1, 2, 3, ... n.
    for (i = 0; i < n; i++) {
        arr[i] = i+1;
    }

    for ( ; ; ) {
        printf("input t: ");
        if (scanf("%d", &t) == EOF) {
            break;
        }

        hold = arr[n]; // backup the element which will be replaced by the guard
        arr[n] = t; // set the guard
        for (i = 0; ; i+=8) {
            if (arr[i] == t)   {        break; }
            if (arr[i+1] == t) { i+= 1; break; }
            if (arr[i+2] == t) { i+= 2; break; }
            if (arr[i+3] == t) { i+= 3; break; }
            if (arr[i+4] == t) { i+= 4; break; }
            if (arr[i+5] == t) { i+= 5; break; }
            if (arr[i+6] == t) { i+= 6; break; }
            if (arr[i+7] == t) { i+= 7; break; }
        }
        arr[n] = hold; // recover the backup element, sometimes not necessary
        if (i == n) {
            printf("not found\n");
        } else {
            printf("found with index: %d\n", i);
        }
    }

    free(arr);

    return 0;
}
```


#### 二分查找优化版本1

修改最原始版本的二分搜索算法，我们仍旧使用下标l和u指示数组中包含t的部分，但是不变式关系变为x[l-1]<t<=x[u]。此外，我们假设n>=0，x[-1]<t以及x[n]>=t（但是程序并不访问这两个假想的元素）。

```
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main(int argc, char const *argv[])
{
    int *arr;
    int i;
    int n;
    int t;
    int l;
    int u;
    int m;
    int p;

    printf("input n: ");
    if (scanf("%d", &n) == EOF) {
        return 0;
    }
    assert(n > 0);

    arr = (int*)malloc(n*sizeof(int));
    assert(arr);
    // Init the arr, 1, 2, 3, ... n.
    for (i = 0; i < n; i++) {
        arr[i] = i+1;
    }

    for ( ; ; ) {
        printf("input t: ");
        if (scanf("%d", &t) == EOF) {
            break;
        }

        l = -1;
        u = n;
        while (l+1 != u) {
            m = l+(u-l)/2; // locate middle of l..u
            if (arr[m] < t) { // t maybe in right half
                l = m;
            } else { // t maybe in left half
                u = m;
            }
        }

        // assert l+1 = u && arr[l] < t && arr[u] >= t
        p = u;
        if (p >= n || arr[p] != t) {
            p = -1;
        }
        printf("index: %d\n", p);
    }

    free(arr);

    return 0;
}
```
> 输出结果：  
input n: 10  
input t: 11  
index: -1  
input t: 10  
index: 9  


#### 二分查找优化版本2（特定1000个元素）

优化的思想是：将之前的(l-1, u)范围表示成(l-1, l+i)。其中每次对半查找时，i>>=1。
保持i为2^n次方。

```
#include <stdio.h>

#define N 1000

int main(int argc, char const *argv[])
{
    int arr[N];
    int l;
    int i;
    int t;
    int p;

    for (i = 0; i < N; i++) {
        arr[i] = i+1;
    }

    for ( ; ; ) {
        printf("Input t: ");
        if (scanf("%d", &t) == EOF) {
            break;
        }

        // Check t is in (-1, 511) or (488, 1000).
        l = -1;
        i = 512;
        if (arr[511] < t) {
            l = 1000 - 512;
        }

        // Binary search algorithm.
        while (i > 1) {
            i >>= 1;
            if (arr[l+i] < t) {
                l += i;
            }
        }
        p = l+1;
        if (p >= N || arr[p] != t) {
            p = -1;
        }
        printf("index: %d\n", p);
    }

    return 0;
}
```


#### 二分查找优化版本3（特定1000个元素 神奇的二分查找）

这个算法其实就是上面二分查找优化版本2的展开，进一步提升了效率。

```
#include <stdio.h>

#define N 1000

int main(int argc, char const *argv[])
{
    int arr[N];
    int l;
    int i;
    int t;
    int p;

    for (i = 0; i < N; i++) {
        arr[i] = i+1;
    }

    for ( ; ; ) {
        printf("Input t: ");
        if (scanf("%d", &t) == EOF) {
            break;
        }

        // Check t is in (-1, 511) or (488, 1000).
        l = -1;
        i = 512;
        // Binary search algorithm by hand.
        if (arr[511] < t) { l = 1000-512; }
        if (arr[l+256] < t) { l += 256; }
        if (arr[l+128] < t) { l += 128; }
        if (arr[l+64]  < t) { l += 64;  }
        if (arr[l+32]  < t) { l += 32;  }
        if (arr[l+16]  < t) { l += 16;  }
        if (arr[l+8]   < t) { l += 8;   }
        if (arr[l+4]   < t) { l += 4;   }
        if (arr[l+2]   < t) { l += 2;   }
        if (arr[l+1]   < t) { l += 1;   }
        p = l+1;
        if (p >= N || arr[p] != t) {
            p = -1;
        }
        printf("index: %d\n", p);
    }

    return 0;
}
```

*以上几个优化版本的算法可参见第5章中的search.c文件。*


### 9.2
实现一个特制的、高效的malloc函数。对于特定大小的内存空间，我们使用内存池技术。过程如下：  
1. 当空余时间为nodesleft为0时，申请NODEGROUP个节点（每个节点大小为NODESIZE），并将空闲节点添加到链表。
2. 当需要使用某个节点时，从空闲链表中拿出一个节点来并返回。
3. 当需要释放某个节点时，将空闲节点添加到链表头。

```
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
```


### 9.3

这道题阐述了在递归调用中慎重使用宏定义，避免宏定义在展开时带来但副作用。

```
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

int max(int a, int b)
{
    return a > b ? a : b;
}

int arrmax(int *arr, int n)
{
    if (n == 1) {
        return arr[0];
    }

    // return MAX(arr[n-1], arrmax(arr, n-1)); // BAD
    return max(arr[n-1], arrmax(arr, n-1)); // GOOD
}

int main(int argc, char const *argv[])
{
    int *arr;
    int n;
    int i;
    int max;

    for ( ; ; ) {
        printf("input array's size: ");
        if (scanf("%d", &n) == EOF) {
            break;
        }
        assert(n > 0);

        arr = (int*)malloc(n*sizeof(int));
        if (!arr) {
            perror("malloc");
            return 1;
        }

        for (i = 0; i < n; i++) {
            arr[i] = n-i;
        }

        max = arrmax(arr, n);
        printf("max is: %d\n", max);
    }

    return 0;
}
```


### 9.5

在未排序但数组中进行二分查找，若是能找到则该数肯定存在，若是找不到，则该数不一定不存在。所以需要在进行二分搜索之前，判断整个数组是否有序的。

    int sorted()
    {   int i;
        for (i = 0; i < n-1; i++)
            if (x[i] > x[i+1])
                return 0;
        return 1;
    }


### 9.6

> C 和 C++ 库提供了字符分类函数（如 isdigit、isupper 和 islower）来确定字符的类型。你会如何实现这些函数呢？

这道题非常有意思，需重点掌握。这个实现主要有两种方法，一种是利用宏定义，一种是利用HASH表，宏定义需要判断，而HASH主要是利用了位运算。

```
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
```


### 9.7

> 给定一个非常长的字节序列（假如有十亿或万亿），如何高效地统计 1 的个数呢？（也就是说，在整个序列中有多少个位的值为 1？）

这个题主要在于如何求整数中位为1的个数算法。最直观的方法是测试每1个bit位，但其时间复杂度是O(n)。  
更快捷的算法如下：

```
#include <stdio.h>
#include <stdint.h>

int main(int argc, char const *argv[])
{
    uint32_t n;
    int cnt;

    for ( ; ; ) {
        printf("input a number: ");
        if (scanf("%u", &n) == EOF) {
            break;
        }

        cnt = 0;
        while (n) {
            n &= (n-1);
            cnt++;
        }
        printf("bits 1 count: %d\n", cnt);
    }

    return 0;
}
```
> 输出结果：  
input a number: 5  
bits 1 count: 2  
input a number: 7  
bits 1 count: 3  

对于比特数为n的数，这算法的时间复杂度是O(k)，k为数中1的个数。

另外一种方式是查表法，将数字对应的比特1个数建表。

这道题，最高效的算法是：先统计输入中每个数出现的次数，然后查表得每个数比特1个数，相乘，最后统计求和所有的数字。

```
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

uint32_t count1(uint8_t n)
{
    uint32_t cnt = 0;

    while (n) {
        n &= (n-1);
        cnt++;
    }

    return cnt;
}

int main(int argc, char const *argv[])
{
    uint32_t table[UINT8_MAX+1];
    int i;
    char buf[] = "hitherehelloworld";
    char counts[UINT8_MAX+1];
    char *p;
    int total;

    // Init the count table for uint8_t.
    for (i = 0; i < (UINT8_MAX+1); i++) {
        table[i] = count1(i);
    }

    printf("the count table is: \n");
    for (i = 0; i < (UINT8_MAX+1); i++) {
        printf("%d ", table[i]);
    }
    printf("\n");

    for ( ; ; ) {
        printf("input string: ");
        if (scanf("%s", buf) == EOF) {
            break;
        }

        memset(counts, 0, sizeof(counts));
        p = buf;
        while (*p) {
            counts[(int)(*p)]++;
            p++;
        }

        total = 0;
        for (i = 0; i < (UINT8_MAX+1); i++) {
            if (counts[i]) {
                total += counts[i]*count1(i);
            }
        }
        printf("the result is: %d\n", total);
    }

    return 0;
}
```
> 输出结果：  
./count_1_buffer_run  
the count table is:   
0 1 1 2 1 2 2 3 1 2 2 3 2 3 3 4 1 2 2 3 2 3 3 4 2 3 3 4 3 4 4 5 1 2 2 3 2 3 3 4 2 3 3 4 3 4 4 5 2 3 3 4 3 4 4 5 3 4 4 5 4 5 5 6 1 2 2 3 2 3 3 4 2 3 3 4 3 4 4 5 2 3 3 4 3 4 4 5 3 4 4 5 4 5 5 6 2 3 3 4 3 4 4 5 3 4 4 5 4 5 5 6 3 4 4 5 4 5 5 6 4 5 5 6 5 6 6 7 1 2 2 3 2 3 3 4 2 3 3 4 3 4 4 5 2 3 3 4 3 4 4 5 3 4 4 5 4 5 5 6 2 3 3 4 3 4 4 5 3 4 4 5 4 5 5 6 3 4 4 5 4 5 5 6 4 5 5 6 5 6 6 7 2 3 3 4 3 4 4 5 3 4 4 5 4 5 5 6 3 4 4 5 4 5 5 6 4 5 5 6 5 6 6 7 3 4 4 5 4 5 5 6 4 5 5 6 5 6 6 7 4 5 5 6 5 6 6 7 5 6 6 7 6 7 7 8  
input string: 1  
the result is: 3  
input string: 11  
the result is: 6  
input string: 11111111  
the result is: 24  
input string: 0  
the result is: 2  
input string: kdjfkasjdfieur8eujifkadsjkfh39rut89ijfd  
the result is: 156  


### 9.8

> 如何在程序中使用哨兵来找出数组中的最大元素？

```
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define N 20

int main(int argc, char const *argv[])
{
    int arr[N+1];
    int i;
    int max;

    srand((unsigned)time(NULL));

    printf("the array is: ");
    for (i = 0; i < N; i++) {
        arr[i] = rand()%100;
        printf("%d ", arr[i]);
    }
    printf("\n");

    // The algorithm to find the maximum element in array, use guard.
    i = 0;
    while (i < N) {
        max = arr[i];
        arr[N] = max;
        i++;
        while (arr[i] < max) {
            i++;
        }
    }
    printf("the max element is: %d\n", max);

    return 0;
}
```
> 输出结果：  
./maximum_element_guard_run   
the array is: 56 93 22 49 40 75 28 96 19 32 82 73 71 77 90 12 90 49 73 48  
the max element is: 96  

这个算法较一般的算法来讲，少了一些判断。


### 9.11

> 20 世纪 60 年代早期，Vic Berecz 发现 Sikorsky 飞机的仿真程序的大部分时间运行时间都消耗在计算三角函数上了。进一步的观察发现，只有在角度为 5 度的整数倍时才计算这些函数。他应该如何减少运行时间？

把所有 5 的倍数的度数都计算好，放入表中，用到时直接查表即可，不用再动态求取。


### 9.12

> 人们在调优程序时有时会从数学的角度考虑而不是从代码的角度考虑。为了计算下面的多项式：

$$
y = a_nx^n + a_{n-1}x^{n-1} + \dots + a_1x^1 + a_0
$$

> 如下的代码使用了 2n 次乘法。请给出一个更快的函数。

    y = a[0]
    xi = 1
    for i = [1, n]
        xi = x * xi
        y = y + a[i] * xi
        
这道题对多项式的计算进行了探讨，换个角度来思考算法，知道就可以了。

    y = a[n]
    for (i=n-1; i>=0; i--)
        y = x*y + a[i]