#include "SMIterator.h"
#include "SortedMap.h"
#include <exception>
using namespace std;

#define INITIAL_CAPACITY 4
#define CAPACITY_MULTIPLIER 2


// Theta(n)
SortedMap::SortedMap(Relation r) {
    this->nr_elems = 0;
    this->capacity = INITIAL_CAPACITY;
    this->elems = new SLLANode[INITIAL_CAPACITY];
    this->relation = r;
    this->head = -1;
    this->tail = -1;
    for (int i = 0; i < this->capacity - 1; i++) {
        this->elems[i].next = i + 1;
    }
    this->elems[this->capacity - 1].next = -1;
    this->first_empty = 0;
}

// Theta(n)
void SortedMap::resize() {
    this->capacity *= CAPACITY_MULTIPLIER;
    // Create a new array with the new capacity
    SLLANode* new_elems = new SLLANode[this->capacity];
    // Copy all the previous (key, value) pairs in the new array
    for (int i = 0; i < this->nr_elems; i++) {
        new_elems[i] = this->elems[i];
    }
    // Set the next fields of empty array elements
    for (int i = this->nr_elems; i < this->capacity - 1; i++) {
        new_elems[i].next = i + 1;
    }
    new_elems[this->capacity - 1].next = -1;
    // Delete the previous elements and set the first_empty attribute
    delete[] this->elems;
    this->elems = new_elems;
    this->first_empty = this->nr_elems;
}

// Best Case: Theta(1) (amortized) - When we have to add a new (key, value) pair at the end / at the beginning of the map; When the map is empty
// Worst Case: Theta(n) - When we have to add a new (key, value) pair at the last position in the map
// Total Complexity: O(n)
TValue SortedMap::add(TKey k, TValue v) {
    if (this->nr_elems == 0) {
        // If the list is empty
        if (this->first_empty == -1) this->resize();
        int insert_pos = this->first_empty;
        this->first_empty = this->elems[first_empty].next;
        this->elems[insert_pos].info.first = k;
        this->elems[insert_pos].info.second = v;
        this->elems[insert_pos].next = -1;
        this->head = this->tail = insert_pos;
        this->nr_elems += 1;
        return NULL_TVALUE;
    }
    if (this->relation(k, this->elems[this->head].info.first) && k != this->elems[this->head].info.first) {
        // If the key of the new (key, value) pair is "smaller" than the first key, then add the new element as the first element in the map
        if (this->first_empty == -1) this->resize();
        int insert_pos = this->first_empty;
        this->first_empty = this->elems[first_empty].next;
        this->elems[insert_pos].info.first = k;
        this->elems[insert_pos].info.second = v;
        this->elems[insert_pos].next = this->head;
        this->head = insert_pos;
        this->nr_elems += 1;
        return NULL_TVALUE;
    }
    if (this->relation(this->elems[this->tail].info.first, k) && this->elems[this->tail].info.first != k) {
        // If the key of the new element is "greater" than the last key, then add the new element as the last element in the map
        if (this->first_empty == -1) this->resize();
        int insert_pos = this->first_empty;
        this->first_empty = this->elems[first_empty].next;
        this->elems[insert_pos].info.first = k;
        this->elems[insert_pos].info.second = v;
        this->elems[insert_pos].next = -1;
        this->elems[this->tail].next = insert_pos;
        this->tail = insert_pos;
        this->nr_elems += 1;
        return NULL_TVALUE;
    }
    // If the above conditions have not been satisfied, then iterate over the elements of the map until a key that is "greater"
    // than the new key is found (or until the end of the map is reached)
    int current = this->head;
    int prev = -1;
    while (this->relation(this->elems[current].info.first, k) && k != this->elems[current].info.first) {
        prev = current;
        current = this->elems[current].next;
    }
    if (this->elems[current].info.first == k) {
        // If the key is already in the map, then change the value of this (key, value) pair and return the old value
        TValue old_value = this->elems[current].info.second;
        this->elems[current].info.second = v;
        return old_value;
    }
    else {
        // Else, if the key is not in the set of keys in the map, then add the new (key, value) pair
        if (this->first_empty == -1) this->resize();
        int insert_pos = this->first_empty;
        this->first_empty = this->elems[this->first_empty].next;
        this->elems[insert_pos].info.first = k;
        this->elems[insert_pos].info.second = v;
        this->elems[insert_pos].next = current;
        this->elems[prev].next = insert_pos;
        this->nr_elems += 1;
        return NULL_TVALUE;
    }
}

// Best Case: Theta(1) - if the searched key is "<=" the first key or ">=" the last key
// Worst Case: Theta(n) - if the searched key is on the second-to-last position in the map
// Total Complexity: O(n)
TValue SortedMap::search(TKey k) const {
    if (this->relation(k, this->elems[this->head].info.first)) {
        // If the searched key is "smaller" than the first key, then return NULL_TVALUE
        if (k != this->elems[this->head].info.first) {
            return NULL_TVALUE;
        }
            // Else if the searched key is equal to the first key, then return the first value
        else if (k == this->elems[this->head].info.first) {
            return this->elems[this->head].info.second;
        }
    }
    if (this->relation(this->elems[this->tail].info.first, k)) {
        // If the seached key is "greater" than the last key, then return NULL_TVALUE
        if (k != this->elems[this->tail].info.first) {
            return NULL_TVALUE;
        }
            // Else if the searched key is equal to the last key, then return the last value
        else if (k == this->elems[this->tail].info.first) {
            return this->elems[this->tail].info.second;
        }
    }
    // Initialize the current node as head
    int current = this->head;
    // Iterate over the elements of the map while the end is not reached and the current key is smaller than the searched key
    while (current != -1 && this->relation(this->elems[current].info.first, k) && this->elems[current].info.first != k) {
        current = this->elems[current].next;
    }
    // If the key was found, then return its value
    if (current != -1 && this->elems[current].info.first == k) {
        return this->elems[current].info.second;
    }
    else {
        // Else, return NULL_TVALUE
        return NULL_TVALUE;
    }
}

// Best Case: Theta(1) - when the key is "smaller" than the first key / "greater" than the last key
// Worst Case: Theta(n) - when we need to remove the last element
// Total Complexity: O(n)
TValue SortedMap::remove(TKey k) {
    if (this->relation(k, this->elems[this->head].info.first) && k != this->elems[this->head].info.first) {
        // If the key is "smaller" than the first key, then the element is not in the map
        return NULL_TVALUE;
    }
    if (this->relation(this->elems[this->tail].info.first, k) && k != this->elems[this->tail].info.first) {
        // If the key is "greater" than the last key, then the element is not in the map
        return NULL_TVALUE;
    }
    // Iterate over the pairs in the map until the relation between the seached key and the current key is not satisfied anymore
    int current = this->head;
    int prev = -1;
    while (current != -1 && this->relation(this->elems[current].info.first, k) && this->elems[current].info.first != k) {
        prev = current;
        current = this->elems[current].next;
    }
    if (current != -1 && this->elems[current].info.first == k) {
        // If the searched key was found, then its corresponding (key, value) pair must be removed
        TValue old_value = this->elems[current].info.second;
        if (current == this->head || current == this->tail) {
            // If the removed pair is actually the head, set the new head
            if (current == this->head) {
                this->head = this->elems[current].next;
            }
            // If the removed pair is actually the tail, set the new tail
            if (current == this->tail) {
                this->tail = prev;
            }
        }
        else {
            // If the removed pair is somewhere between the head and the tail, set the "next" field of the previous node
            this->elems[prev].next = this->elems[current].next;
        }
        this->elems[current].next = this->first_empty;
        this->first_empty = current;
        this->nr_elems -= 1;
        return old_value;
    }
    else {
        return NULL_TVALUE;
    }
}

// Theta(1)
int SortedMap::size() const {
    return this->nr_elems;
}

// Theta(1)
bool SortedMap::isEmpty() const {
    return this->nr_elems == 0;
}

SMIterator SortedMap::iterator() const {
    return SMIterator(*this);
}

// Theta(1)
SortedMap::~SortedMap() {
    delete[] this->elems;
}
