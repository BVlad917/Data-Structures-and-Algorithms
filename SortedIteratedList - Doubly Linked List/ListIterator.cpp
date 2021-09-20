#include "ListIterator.h"
#include "SortedIteratedList.h"
#include <exception>

using namespace std;


// Theta(1)
ListIterator::ListIterator(const SortedIteratedList& list) : list(list){
    this->current_elem = list.head;
}

// Theta(1)
void ListIterator::first(){
    this->current_elem = this->list.head;
}

// Theta(1)
void ListIterator::next(){
    if (this->current_elem == NULL) {
        throw exception();
    }
    this->current_elem = this->current_elem->next;
}

// Theta(1)
bool ListIterator::valid() const{
    if (this->current_elem == NULL) {
        return false;
    }
    return true;
}

// Theta(1)
TComp ListIterator::getCurrent() const{
    if (this->current_elem == NULL) {
        throw exception();
    }
    return this->current_elem->info;
}
