#ifndef LINKEDLIST_H
#define LINKEDLIST_H

struct ListNode {
    int value;
    ListNode* next;
    ListNode(int value) : value(value), next(nullptr) {}
};

class LinkedList {
private:
    ListNode* head;
    ListNode* curr;
    int size;
    int id;
public:
    LinkedList(int id);
    
    void add(int value);

    void print();

    int getSize();
    int getId();

    class Iterator {
    private:
        ListNode* current;
    public:
        Iterator(ListNode* start);

        bool hasNext();
        int value();
        int next();
    };

    Iterator begin();
};

#endif