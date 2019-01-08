#include <vector>

#ifndef HEAP_CLASS
#define HEAP_CLASS

template<class T>
struct HeapItem
{
public:
    T val;
    int heapIndex;
    HeapItem(T, int);
};
template<class T>
class Heap
{
private:
    std::vector<HeapItem<T>*> garbage;
    std::vector<HeapItem<T>*> arr;
    void swap(HeapItem<T>**, HeapItem<T>**);
    void shiftUp(HeapItem<T>*);
    void shiftDown(HeapItem<T>*);
public:
    HeapItem<T>* push(T);
    T* pop();
    void update(HeapItem<T>*);
    int size();
    ~Heap();
};

#endif