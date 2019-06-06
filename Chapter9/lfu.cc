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
