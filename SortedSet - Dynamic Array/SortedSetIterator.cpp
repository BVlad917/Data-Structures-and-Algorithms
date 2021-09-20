#include "SortedSetIterator.h"
#include <exception>

using namespace std;

// Theta(1)
SortedSetIterator::SortedSetIterator(const SortedSet& m) : multime(m)
{
    this->current_index = 0;
}

// Theta(1)
void SortedSetIterator::first() {
    this->current_index = 0;
}

// Theta(1)
void SortedSetIterator::next() {
    if (this->current_index == this->multime.nr_elems) {
        throw exception();
    }
    this->current_index += 1;
}

// Theta(1)
TElem SortedSetIterator::getCurrent() {
    if (this->current_index == this->multime.nr_elems) {
        throw exception();
    }
    return this->multime.elements[this->current_index];
}

// Theta(1)
bool SortedSetIterator::valid() const {
    if (this->current_index < this->multime.nr_elems) {
        return true;
    }
    return false;
}

