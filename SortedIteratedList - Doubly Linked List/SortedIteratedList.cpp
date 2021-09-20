#include "ListIterator.h"
#include "SortedIteratedList.h"
#include <iostream>
#include <exception>
using namespace std;


// Theta(1)
SortedIteratedList::SortedIteratedList(Relation r) {
    this->head = NULL;
    this->tail = NULL;
    this->nr_elems = 0;
    this->relation = r;
}

// Theta(1)
int SortedIteratedList::size() const {
    return this->nr_elems;
}

// Theta(1)
bool SortedIteratedList::isEmpty() const {
    return this->nr_elems == 0;
}

// Theta(1)
ListIterator SortedIteratedList::first() const {
    return ListIterator(*this);
}

// Theta(1)
TComp SortedIteratedList::getElement(ListIterator poz) const {
    if (poz.current_elem == NULL) {
        throw exception();
    }
    return poz.getCurrent();
}

// There is no Best Case / Worst Case since whatever we do, we still have to check every element for that condition
// We cannot use the fact that the elements are sorted to skip checking since we may have a condition that doesn't take into consideration
// the order of the elements (e.g., a condition that checks if the given element is even, the order of the elements doesn't help us in any
// way in this case)
// Theta(n) - Iterate over all elements and if a certain element does not satisfy the condition, then remove it
void SortedIteratedList::filter(Condition cond) {
    ListIterator it(*this);
    while (it.current_elem != NULL) {
        if (!cond(it.current_elem->info)) {
            this->remove(it);
        }
        else {
            it.current_elem = it.current_elem->next;
        }
    }
}

// Theta(1)
TComp SortedIteratedList::remove(ListIterator& poz) {
    if (!poz.valid()) {
        throw exception();
    }
    TComp removed = poz.getCurrent();

    // Save the node which will be removed, the previous node, and the next node in auxilary variables for ease of use
    DLLNode* prev_node = poz.current_elem->previous;
    DLLNode* to_remove_node = poz.current_elem;
    DLLNode* next_node = poz.current_elem->next;

    // If the element that we have to remove turns out to be the tail, then we also have to update the SDLL's tail
    if (poz.current_elem == this->tail) {
        this->tail = this->tail->previous;
    }
    // If the element that we have to remove turns out to be the head, then we also have to update the SDLL's head
    if (poz.current_elem == this->head) {
        this->head = this->head->next;
    }
    // Set the iterator to point to the element after the one we remove
    poz.current_elem = poz.current_elem->next;
    // If the node we remove is not the head, then we also have to update the "next link" of the node to the left of the removed one
    if (prev_node != NULL) {
        prev_node->next = to_remove_node->next;
    }
    // If the node we remove is not the tail, then we also have to update the "previous link" of the node to the right of the removed one
    if (next_node != NULL) {
        next_node->previous = to_remove_node->previous;
    }
    // Remove the links of the node we will delete
    to_remove_node->previous = to_remove_node->next = NULL;
    // Remove the element from memory and decrease the count of the number of elements
    delete to_remove_node;
    this->nr_elems -= 1;
    return removed;
}

// Best Case: Theta(1) - if the searched element is "smaller" than the head or "greater" than the tail
// Worst Case: Theta(n) - if the element is in the list on the last position/the element is not in the list
// Total Complexity: O(n)
ListIterator SortedIteratedList::search(TComp e) const{
    ListIterator it(*this);
    // If the list is not empty and the searched element is "smaller" than the head node, then the element is not in the list
    if (this->head != NULL && this->head->info != e && this->relation(e, this->head->info)) {
        it.current_elem = NULL;
    }
        // Else if the list is not empty and the searched element is "greater" than the tail node, then the element is not in the list
    else if (this->tail != NULL && this->tail->info != e && this->relation(this->tail->info, e)) {
        it.current_elem = NULL;
    }
        // Else iterate over the list until the element is found or we get to the end of the list
    else {
        // We stop looking for the element when we get to the first element that is "greater" than the one we are looking for
        while (it.current_elem != NULL && this->relation(it.current_elem->info, e) && it.current_elem->info != e) {
            it.current_elem = it.current_elem->next;
        }
    }
    if (it.current_elem != NULL && it.current_elem->info != e) {
        // If the element we stopped at is not equal to the searched element, that means that the searched element is not in the list
        it.current_elem = NULL;
    }
    return it;
}

// Best Case: Theta(1) - when we have to add a new node either as the first element or as the last element
// Worst Case: Theta(n) - when we have to add a new node on the second to last position of the list
// Total Complexity: O(n)
void SortedIteratedList::add(TComp e) {
    DLLNode* new_node = new DLLNode;
    new_node->info = e;
    new_node->next = NULL;
    new_node->previous = NULL;

    // If the list is empty (i.e., the head node points to NULL), then point both the head and the tail to the new node
    if (this->head == NULL) {
        this->head = new_node;
        this->tail = new_node;
    }
    else {
        if (this->relation(e, this->head->info)) {
            // If the list is not empty and the element we want to add is "smaller" than the head node, then add the new node as the head node
            new_node->next = head;
            head->previous = new_node;
            head = new_node;
        }
        else if (this->relation(this->tail->info, e)) {
            // If the list is not empty and the element we want to add is "greater" than the tail node, then add the new node as the tail node
            new_node->previous = tail;
            tail->next = new_node;
            tail = new_node;
        }
        else {
            // Else, we have to add the new node somewhere in the list
            DLLNode* current_node = this->head;
            while (this->relation(current_node->info, e)) {
                current_node = current_node->next;
            }
            new_node->next = current_node;
            new_node->previous = current_node->previous;
            current_node->previous->next = new_node;
            current_node->previous = new_node;
        }
    }
    this->nr_elems += 1;
    return;
}

// Theta(n) - we have to itearate over the list and delete each element individually
SortedIteratedList::~SortedIteratedList() {
    DLLNode* temp = this->head;
    DLLNode* next_to_temp;
    while (temp != NULL) {
        next_to_temp = temp->next;
        // Remove the links first
        temp->previous = NULL;
        temp->next = NULL;
        // Remove the dynamically allocated node
        delete temp;
        // Go to the next node
        temp = next_to_temp;
    }
    this->nr_elems = 0;
}
