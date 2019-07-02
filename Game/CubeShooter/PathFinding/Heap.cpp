#include "Heap.h"
#include <iostream>

template<class T>
HeapItem<T>::HeapItem(T val, int heapIndex)
{
    this->val = val;
    this->heapIndex = heapIndex;
}
template<class T>
void Heap<T>::swap(HeapItem<T>** l, HeapItem<T>** r)
{
    std::swap((*l)->heapIndex, (*r)->heapIndex);
    HeapItem<T>* temp = *l;
    *l = *r;
    *r = temp;
}
template<class T>
void Heap<T>::shiftUp(HeapItem<T>* val)
{
    int i = val->heapIndex;
    while(i != 0 && arr[i]->val < arr[(i-1)/2]->val)
    {
        swap(&arr[i], &arr[(i-1)/2]);
        i = (i-1)/2;
    }
}
template<class T>
void Heap<T>::shiftDown(HeapItem<T>* val)
{
    while(val->heapIndex != arr.size() - 1)
    {
        int i = val->heapIndex;
        if((2*i)+1 < arr.size() && arr[(2*i)+1]->val < arr[i]->val)
        {
            swap(&arr[(2*i)+1], &arr[i]);
        }
        else if((2*i)+2 < arr.size() && arr[(2*i)+2]->val < arr[i]->val)
        {
            swap(&arr[(2*i)+2], &arr[i]);
        }
        else
        {
            break;
        }
    }
}
template<class T>
HeapItem<T>* Heap<T>::push(T val)
{
    HeapItem<T>* item = new HeapItem<T>(val, arr.size());
	arr.push_back(item);
    shiftUp(item);
    return item;
}
template<class T>
T* Heap<T>::pop()
{
    T* res = &arr[0]->val;
    swap(&arr[0], &arr[arr.size() - 1]);
    arr[arr.size() - 1]->heapIndex = -1;
    garbage.push_back(arr[arr.size() - 1]);
    arr.pop_back();
	if (arr.size() > 0)
	{
		shiftDown(arr[0]);
	}
    return res;
}
template<class T>
void Heap<T>::update(HeapItem<T>* item)
{
    if(item->heapIndex == -1)
    {
		std::cout << "pingo!";
        return;
    }
    shiftUp(item);
    shiftDown(item);
}
template<class T>
int Heap<T>::size()
{
    return arr.size();
}
template<class T>
Heap<T>::~Heap()
{
    for(HeapItem<T>* i : arr)
    {
        delete i;
    }
    for(HeapItem<T>* i : garbage)
    {
        delete i;
    }
}