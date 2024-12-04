#include <iostream>

#include "linkedlist.h"

using namespace std;

LinkedList::LinkedList(int id) : head(nullptr), size(0), id(id) {}

void LinkedList::add(int value) {
    ListNode* node = new ListNode(value);
    if (head == nullptr) {
        head = node;
    } else {
        ListNode* current = head;
        while (current) {
            if (current->value == value) return;
            if (current->next == nullptr) break;
            current = current->next;
        }
        current->next = node;
    }
    size++;
}

void LinkedList::print() {
    ListNode* current = head;
    while (current != nullptr) {
        cout << current->value << " ";
        current = current->next;
    }
    cout << endl;
}

int LinkedList::getSize() { return size; }
int LinkedList::getId() { return id; }


LinkedList::Iterator::Iterator(ListNode* start) : current(start) { }

bool LinkedList::Iterator::hasNext() { return current != nullptr; }

int LinkedList::Iterator::value() { return current->value; }

int LinkedList::Iterator::next() {
    int value = current->value;
    current = current->next;
    return value;
}

LinkedList::Iterator LinkedList::begin() {
    return LinkedList::Iterator(head);
}