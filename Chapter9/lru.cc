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
