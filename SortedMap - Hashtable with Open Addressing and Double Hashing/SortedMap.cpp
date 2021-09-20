#include "SMIterator.h"
#include "SortedMap.h"
#include <exception>
using namespace std;

// Theta(1)
int SortedMap::h(TKey key, int i) const {
    if (key >= 0) {
        return (key % this->capacity + i * (1 + key % (this->capacity - 1))) % this->capacity;
    }
    // If the key k is negative, then we have to use -k instead of k (Alternatively, we can use abs(key))
    // Otherwise, the hash function might return a negative number; it actually did that in the tests, not fun debugging :(
    // Negating just the second function h''(k) (the one after the addition sign '+') doesn't work!!! We have to use -k in both places
    // I thought it might work since we have the condition that h''(k) should always return a value in the interval [1, m - 1],
    // but we have no condition for the first function h'(k). But it doesn't work that way, we need a positive value in both places.
    return ((-key) % this->capacity + i * (1 + (-key) % (this->capacity - 1))) % this->capacity;
}

// Best Case: Theta(1)
// Worst Case: Theta(sqrt(n))
// Total Complexity: O(sqrt(n))
bool SortedMap::is_prime(int n) const {
    if (n <= 1) {
        return false;
    }
    if (n == 2) {
        return true;
    }
    if (n % 2 == 0) {
        return false;
    }
    for (int d = 3; d * d <= n; d += 2) {
        if (n % d == 0) {
            return false;
        }
    }
    return true;
}

// Theta(capacity)
void SortedMap::resize_and_rehash(){
    // Save the capacity, elements, and element states in auxilary variables
    int old_capacity = this->capacity;
    TElem* old_elems = this->elems;
    int* old_elem_state = this->element_state;
    // Multiply the capacity by the CAPACITY_MULTIPLIER and then increase it until we find a prime number
    this->capacity *= CAPACITY_MULTIPLIER;
    while (!this->is_prime(this->capacity)) {
        this->capacity += 1;
    }
    // Create new arrays for the elements and the elements states
    this->elems = new TElem[this->capacity];
    this->element_state = new int[this->capacity];
    // Initialize all the new element states to EMPTY
    for (int i = 0; i < this->capacity; i++) {
        this->element_state[i] = EMPTY;
    }
    // Adding the elements will increase the <nr_elems> count, so we set its initial value to 0
    this->nr_elems = 0;
    for (int i = 0; i < old_capacity; i++) {
        // Add the old elements to the new hash table
        if (old_elem_state[i] == OCCUPIED) {
            this->add(old_elems[i].first, old_elems[i].second);
        }
    }
    // Delete the old arrays of elements and element states
    delete[] old_elems;
    delete[] old_elem_state;
}

// Theta(1)
double SortedMap::get_load_factor() {
    return (double)this->nr_elems / this->capacity;
}

// Theta(capacity)
SortedMap::SortedMap(Relation r) {
    this->capacity = INITIAL_CAPACITY;
    this->elems = new TElem[INITIAL_CAPACITY];
    this->element_state = new int[INITIAL_CAPACITY];
    this->relation = r;
    this->nr_elems = 0;
    // Initialize all element states as empty
    for (int i = 0; i < INITIAL_CAPACITY; i++) {
        this->element_state[i] = EMPTY;
    }
}

// Best Case: Theta(1) amortized
// Average Case: Theta(1) amortized
// Worst Case: Theta(nr_elems)
// Total Complexity: O(nr_elems)
// The add depends on searching the key in the hash table, and the search functionality is Theta(1) (on average)
// The complexity will be Theta(capacity) when we need to resize and rehash (rarely)
TValue SortedMap::add(TKey k, TValue v) {
    TValue old_value = NULL_TVALUE;		// Returned value; Will be an array element if we insert an existing key
    int i = 0;
    int position = this->h(k, i);
    int first_deleted_position = -1;	// Used in the case that we have to add on a deleted position
    // If the load factor is bigger than the MAX_LOAD_FACTOR, then we have to resize, rehash, and recompute the initial position
    if (this->get_load_factor() >= MAX_LOAD_FACTOR) {
        this->resize_and_rehash();
        position = this->h(k, i);
    }
    while (this->element_state[position] != EMPTY && this->element_state[position] != DELETED) {
        if (this->element_state[position] == OCCUPIED && this->elems[position].first == k) {
            break;
        }
        i += 1;
        position = this->h(k, i);
    }
    if (this->element_state[position] == OCCUPIED && this->elems[position].first == k) {
        old_value = this->elems[position].second;
        this->elems[position].second = v;
    }
    else if (this->element_state[position] == EMPTY || this->element_state[position] == DELETED) {
        this->elems[position].first = k;
        this->elems[position].second = v;
        this->element_state[position] = OCCUPIED;
        this->nr_elems += 1;
    }
    return old_value;
}

// Best Case: Theta(1)
// Average Case: Theta(1) - On average, searching depends on the load factor alpha, which we keep constant => Theta(1 + alpha) = Theta(1)
// Worst Case: Theta(nr_elems)
// Total Complexity: O(nr_elems)
TValue SortedMap::search(TKey k) const {
    TValue found_value = NULL_TVALUE;
    int i = 0;
    int position = this->h(k, i);	// Initialize the position
    // Loop over the hash table elements until we find the first empty position or we find the key in an occupied position
    while (this->element_state[position] != EMPTY) {
        // If we found the key on an occupied position in the hash table, then we break out of the loop
        if (this->element_state[position] == OCCUPIED && this->elems[position].first == k) {
            break;
        }
        // Increase the index i and recompute the position
        i += 1;
        position = this->h(k, i);
    }
    // If we found the key in an occupied position, then we return that. Else, we return NULL_TVALUE
    if (this->element_state[position] == OCCUPIED && this->elems[position].first == k) {
        found_value = this->elems[position].second;
    }
    return found_value;
}

// Best Case: Theta(1)
// Average Case: Theta(1)
// Worst Case: Theta(nr_elems)
// Total Complexity: O(nr_elems)
// The complexity of the remove functionality is given by the searching done inside it
TValue SortedMap::remove(TKey k) {
    TValue removed_value = NULL_TVALUE;
    int i = 0;
    int position = this->h(k, i);	// Initialize the position
    // Search for the key in the hash table
    while (this->element_state[position] != EMPTY) {
        // If we found the key on an occupied position in the hash table, then we break out of the loop
        if (this->element_state[position] == OCCUPIED && this->elems[position].first == k) {
            break;
        }
        // Increase the index i and recompute the position
        i += 1;
        position = this->h(k, i);
    }
    // If we found the key on an occupied position in the hash table, then we save the value, decrease the elements counter,
    // and mark that position as DELETED
    if (this->element_state[position] == OCCUPIED && this->elems[position].first == k) {
        removed_value = this->elems[position].second;
        this->nr_elems -= 1;
        this->element_state[position] = DELETED;
    }
    return removed_value;
}

// Theta(1)
int SortedMap::size() const {
    return this->nr_elems;
}

// Theta(1)
bool SortedMap::isEmpty() const {
    return this->nr_elems == 0;
}

// Theta(nr_elems*log(nr_elems))
SMIterator SortedMap::iterator() const {
    return SMIterator(*this);
}

// Theta(capacity)
SortedMap::~SortedMap() {
    delete[] this->elems;
    delete[] this->element_state;
}
