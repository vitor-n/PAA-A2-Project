#include <iostream>

#include "heap.h"

using namespace std;

template <typename T>
Heap<T>::Heap(int size, HeapType type): heap(nullptr), mapping(nullptr),
                                        maxSize(size), heapEnd(0), type(type) {
    heap = new int[maxSize];
    mapping = new T[maxSize];
    for(int i = 0; i < maxSize; i++){
        heap[i] = 0;
        mapping[i] = i;
    }
}

template <typename T>
Heap<T>::~Heap(){
    delete heap;
}

template <typename T>
void Heap<T>::swap(int i, int j){
    T temp = heap[i];
    heap[i] = heap[j];
    heap[j] = temp;
}


template <typename T>
void Heap<T>::set_mapping(T correspondeces[]){
    mapping = correspondeces;
}

template <typename T>
int Heap<T>::heapSize(){
    return heapEnd;
}

template <typename T>
void Heap<T>::maxHeapify(int iIndex){
    int i = iIndex;
    int iLeft = 2 * iIndex + 1; //+1 because arrays are 0-indexed
    int iRight = 2 * iIndex + 2;
    if (iLeft < heapEnd && mapping[heap[i]] < mapping[heap[iLeft]]){
        i = iLeft;
    }
    if (iRight < heapEnd && mapping[heap[i]] < mapping[heap[iRight]]){
        i = iRight;
    }
    if (i != iIndex){
        swap(i, iIndex);
        maxHeapify(i);
    }
}

template <typename T>
void Heap<T>::minHeapify(int iIndex){
    int i = iIndex;
    int iLeft = 2 * iIndex + 1; //+1 because arrays are 0-indexed
    int iRight = 2 * iIndex + 2;
    if (iLeft < heapEnd && mapping[heap[i]] >= mapping[heap[iLeft]]){
        i = iLeft;
    }
    if (iRight < heapEnd && mapping[heap[i]] >= mapping[heap[iRight]]){
        i = iRight;
    }
    if (i != iIndex){
        swap(i, iIndex);
        minHeapify(i);
    }
}


template <typename T>
void Heap<T>::maxHeapifyBottom(int iIndex){
    int iParent = (iIndex - 1) / 2;
    if (iParent >= 0) { //If negative means it's the top of the heap
        if (mapping[heap[iIndex]] > mapping[heap[iParent]]) {
            swap(iIndex, iParent);
            // Recursively heapify the parent node
            maxHeapifyBottom(iParent);
        }
    }
}

template <typename T>
void Heap<T>::minHeapifyBottom(int iIndex){
    int iParent = (iIndex - 1) / 2;
    if (iParent >= 0) { //If negative means it's the top of the heap
        if (mapping[heap[iIndex]] < mapping[heap[iParent]]) {
            swap(iIndex, iParent);
            // Recursively heapify the parent node
            minHeapifyBottom(iParent);
        }
    }
}

template <typename T>
int Heap<T>::get_top(){
    return heap[0];
}

template <typename T>
void Heap<T>::remove_top(){
    heapEnd--;
    swap(0, heapEnd);
    if(type == MIN) minHeapify(0);
    else if(type == MAX) maxHeapify(0);
}

template <typename T>
int Heap<T>::pop_top(){
    int value = get_top();
    remove_top();
    return value;
}

template <typename T>
void Heap<T>::insert(int iElement){
    if(heapEnd < maxSize){
        heap[heapEnd] = iElement;
        heapEnd++;
        if(type == MIN) minHeapifyBottom(heapEnd - 1);
        else if(type == MAX) maxHeapifyBottom(heapEnd - 1);

    }
}

template <typename T>
void Heap<T>::print(){
    cout << heapEnd << "/" << maxSize << " [ ";
    for(int i = 0; i < heapEnd; i++){
        cout << heap[i] << " ";
    }
    cout << "]" << endl << "[ ";
    for(int i = 0; i < heapEnd; i++){
        cout << mapping[heap[i]] << " ";
    }
    cout << "]" << endl;
}

template class Heap<int>;
template class Heap<float>;

/*
int main(){
    Heap<int> heap = Heap<int>(90, MIN);
    heap.print();
    heap.insert(9);
    heap.insert(80);
    heap.insert(14);
    heap.insert(12);
    heap.insert(65);
    heap.insert(19);
    heap.insert(11);
    heap.print();
    cout << heap.pop_top() << endl;
    cout << heap.pop_top() << endl;
    heap.print();

    float mapping[90];
    for(int i = 0; i < 90; i++){mapping[i] = 90 - i;}
    Heap<float> newHeap = Heap<float>(90, MAX);
    newHeap.set_mapping(mapping);
    newHeap.insert(9);
    newHeap.insert(80);
    newHeap.insert(14);
    newHeap.insert(12);
    newHeap.insert(65);
    newHeap.insert(19);
    newHeap.insert(11);
    newHeap.insert(0);
    newHeap.insert(55);
    newHeap.print();
}*/

/*
int main () {
    int mapping[10000];
    for(int i = 0; i < 10000; i++){mapping[i] = 2147483647;}
    mapping[202] = 600;
    mapping[4] = 630;
    mapping[301] = 660;
    mapping[103] = 650;
    mapping[400] = 580;
    mapping[401] = 760;


    Heap<int> heap = Heap<int>(10000, MIN);
    heap.set_mapping(mapping);
    heap.print();
    heap.insert(202);
    heap.insert(4);
    heap.insert(301);
    heap.insert(103);
    //heap.print();
    heap.insert(400);
    heap.print();
    heap.pop_top();
    heap.print();
    heap.insert(401);
    heap.print();
}
*/


/*
4/10000 [ 202 4 301 103 ]
[ 600 630 660 650 ]
5/10000 [ 400 202 301 103 4 ]
[ 580 600 660 650 630 ]
4/10000 [ 202 103 301 4 ]
[ 600 650 660 630 ]
5/10000 [ 202 103 301 4 401 ]
[ 600 650 660 630 760 ]
6/10000 [ 202 103 301 4 401 500 ]
[ 600 650 660 630 760 730 ]
5/10000 [ 103 401 301 4 500 ]
[ 650 760 660 630 730 ]

*/
