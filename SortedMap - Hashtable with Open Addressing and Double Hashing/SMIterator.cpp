#include "SMIterator.h"
#include "SortedMap.h"
#include <exception>

using namespace std;

// Theta(nr_elems*log(nr_elems))
SMIterator::SMIterator(const SortedMap& m) : map(m){
    this->current_index = 0;
    this->entries = new TElem[this->map.nr_elems];
    int j = 0;
    // Save the elements from the OCCUPIED positions into an array
    for (int i = 0; i < this->map.capacity; i++) {
        if (this->map.element_state[i] == OCCUPIED) {
            this->entries[j] = this->map.elems[i];
            j += 1;
        }
    }
    this->merge_sort(this->entries, 0, j - 1);	// Sort the array
}

void SMIterator::merge(TElem* arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    int i, j, k;
    TElem* left_subarray = new TElem[n1];
    TElem* right_subarray = new TElem[n2];
    // Copy the left part of the array into an auxilary array
    for (i = 0; i < n1; i++) {
        left_subarray[i] = arr[l + i];
    }
    // Copy the right part of the array into an auxilary array
    for (j = 0; j < n2; j++) {
        right_subarray[j] = arr[m + 1 + j];
    }
    // Merge the 2 subarrays
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (this->map.relation(left_subarray[i].first, right_subarray[j].first)) {
            arr[k] = left_subarray[i];
            i += 1;
        }
        else {
            arr[k] = right_subarray[j];
            j += 1;
        }
        k += 1;
    }
    // Copy any remaining elements from the left subarray into the original array
    while (i < n1) {
        arr[k] = left_subarray[i];
        i += 1;
        k += 1;
    }
    // Copy any remaining elements from the right subarray into the original array
    while (j < n2) {
        arr[k] = right_subarray[j];
        j += 1;
        k += 1;
    }
    // Delete the auxilary arrays
    delete[] left_subarray;
    delete[] right_subarray;
}

// Theta(n*logn)
void SMIterator::merge_sort(TElem* arr, int l, int r) {
    if (l >= r) {
        return;
    }
    int m = l + (r - l) / 2;
    this->merge_sort(arr, l, m);
    this->merge_sort(arr, m + 1, r);
    this->merge(arr, l, m, r);
}

// Theta(1)
void SMIterator::first(){
    this->current_index = 0;
}

// Theta(1)
void SMIterator::next(){
    if (this->current_index == -1 || this->current_index == this->map.nr_elems) {
        throw exception();
    }
    this->current_index += 1;
}

// Theta(1)
void SMIterator::previous() {
    if (this->current_index == -1 || this->current_index == this->map.nr_elems) {
        throw exception();
    }
    this->current_index -= 1;
}

// Theta(1)
bool SMIterator::valid() const{
    return (this->current_index < this->map.nr_elems) && (this->current_index > -1);
}

// Theta(1)
TElem SMIterator::getCurrent() const{
    if (this->current_index == this->map.nr_elems || this->current_index == -1) {
        throw exception();
    }
    return this->entries[this->current_index];
}


