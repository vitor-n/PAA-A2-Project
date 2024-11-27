#ifndef HEAP_H
#define HEAP_H

enum HeapType{
    MIN,
    MAX
};

template <typename T>
class Heap {
private:
    //important things to store
    int* heap;
    int* pos;
    T* mapping;
    int maxSize;
    int heapEnd;
    HeapType type;

    //heap algorithms
    void swap(int, int);
    void maxHeapify(int);
    void minHeapify(int);
    void maxHeapifyBottom(int);
    void minHeapifyBottom(int);

public:
    //class constructors and destructors
    Heap(int size, HeapType);
    ~Heap();
    void set_mapping(T[]);
    int heapSize();

    //retrieval and insertion operations
    int get_top();
    void remove_top();
    int pop_top();
    void insert(int);

    //debugging
    void print();
};

#endif

