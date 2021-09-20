#include "SMIterator.h"
#include "SortedMap.h"
#include <exception>

using namespace std;

// Theta(1)
SMIterator::SMIterator(const SortedMap& m) : map(m){
    this->current_index = m.head;
}

// Theta(1)
void SMIterator::first(){
    this->current_index = this->map.head;
}

// Theta(1)
void SMIterator::next(){
    if (this->current_index == -1) {
        throw exception();
    }
    this->current_index = this->map.elems[current_index].next;
}

// Theta(1)
bool SMIterator::valid() const{
    if (this->current_index != -1) {
        return true;
    }
    return false;
}

// Theta(1)
TElem SMIterator::getCurrent() const{
    if (this->current_index == -1) {
        throw exception();
    }
    return this->map.elems[this->current_index].info;
}
