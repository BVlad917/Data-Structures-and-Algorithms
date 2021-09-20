#include <cmath>
#include "SortedSet.h"
#include "SortedSetIterator.h"

#define INITIAL_CAPACITY 4
#define CAPACITY_MULTIPLIER 2

// Theta(1)
SortedSet::SortedSet(Relation r) {
    this->nr_elems = 0;
    this->capacity = INITIAL_CAPACITY;
    this->elements = new TComp[INITIAL_CAPACITY];
    this->relation = r;
}

// Theta(nr_elems)
void SortedSet::resize() {
    this->capacity *= CAPACITY_MULTIPLIER;
    TComp* new_elems = new TComp[this->capacity];
    for (int i = 0; i < this->nr_elems; i++) {
        new_elems[i] = this->elements[i];
    }
    delete[] this->elements;
    this->elements = new_elems;
}

void SortedSet::unionize(const SortedSet& s) {
    SortedSetIterator it = this->iterator();
    SortedSetIterator it2 = s.iterator();
    TComp* aux = new TComp[100000];
    int len = 0;

    while (it.valid() && it2.valid()) {
        if (this->relation(it.getCurrent(), it2.getCurrent())) {
            if (len == 0 || (len > 0 && it.getCurrent() != aux[len - 1])) {
                aux[len++] = it.getCurrent();
            }
            it.next();
        }
        else {
            if (len == 0 || (len > 0 && it2.getCurrent() != aux[len - 1])) {
                aux[len++] = it2.getCurrent();
            }
            it2.next();
        }
    }
    while (it.valid()) {
        if (len == 0 || (len > 0 && it.getCurrent() != aux[len - 1])) {
            aux[len++] = it.getCurrent();
        }
        it.next();
    }
    while (it2.valid()) {
        if (len == 0 || (len > 0 && it2.getCurrent() != aux[len - 1])) {
            aux[len++] = it2.getCurrent();
        }
        it2.next();
    }

    this->nr_elems = len;
    delete[] this->elements;
    this->elements = aux;
}

// Best Case: Theta(1) if the element is on the middle position
// Worst Case: Theta(log(nr_elems)) if the element is not in the set
// Total Complexity: O(log(nr_elems))
int SortedSet::corresponding_index(TComp elem) {
    int mid = 0, low = 0, high = this->nr_elems - 1;
    while (low <= high) {
        mid = (low + high) / 2;
        if (this->elements[mid] == elem) {
            return mid;
        }
        else if (this->relation(elem, this->elements[mid])) {
            high = mid - 1;
        }
        else {
            low = mid + 1;
        }
    }
    return high + 1;
}

// Best Case: Theta(1) - When we want to add an existing element on the middle position
// Worst Case: Theta(nr_elems) - When we need to resize / When we want to add an element on the first position
// Total Complexity: O(nr_elems)
bool SortedSet::add(TComp elem) {
    int position = this->corresponding_index(elem);
    // If the element is in the list, it should be on the position returned by <corresponding_index()>
    // If it's not in the list, then we know we need to insert it on the returned position
    if (this->elements[position] == elem) {
        return false;
    }
    // If the array is filled, resize it
    if (this->nr_elems == this->capacity) {
        this->resize();
    }
    // Now we can move elements one position to the right and insert the new element in the right place
    for (int i = this->nr_elems; i > position; i--) {
        this->elements[i] = this->elements[i - 1];
    }
    this->elements[position] = elem;
    this->nr_elems += 1;
    return true;

}

// Best Case: Theta(log(nr_elems)) - When we want to remove an element that is not in the array
// Worst Case: Theta(nr_elems) - When we want to remove the first element from the array
// Total Complexity: O(nr_elems)
bool SortedSet::remove(TComp elem) {
    int position = this->corresponding_index(elem);
    // If the element is in the list, it should be on the position returned by <corresponding_index()>
    // If it's not in the list, then there is nothing to do and we return false
    if (this->elements[position] != elem) {
        return false;
    }
    for (int i = position; i < this->nr_elems; i++) {
        this->elements[i] = this->elements[i + 1];
    }
    this->nr_elems -= 1;
    return true;
}

// Best Case: Theta(1) if the searched element is on the middle position;
// Worst Case: Theta(log(nr_elems)) if the searched element is not in the set
// Total Complexity: O(log(nr_elems))
bool SortedSet::search(TComp elem) const {
    int mid, low = 0, high = this->nr_elems - 1;
    while (low <= high) {
        mid = (low + high) / 2;
        if (this->elements[mid] == elem) {
            return true;
        }
        else if (this->relation(elem, this->elements[mid])) {
            high = mid - 1;
        }
        else {
            low = mid + 1;
        }
    }
    return false;
}

// Theta(1)
int SortedSet::size() const {
    return this->nr_elems;
}

// Theta(1)
bool SortedSet::isEmpty() const {
    return this->nr_elems == 0;
}

// Theta(1)
SortedSetIterator SortedSet::iterator() const {
    return SortedSetIterator(*this);
}

// Theta(1)
SortedSet::~SortedSet() {
    delete[] this->elements;
}
