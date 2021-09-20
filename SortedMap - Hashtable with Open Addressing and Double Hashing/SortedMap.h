#pragma once
//DO NOT INCLUDE SORTEDMAPITERATOR

//DO NOT CHANGE THIS PART
typedef int TKey;
typedef int TValue;
#include <utility>
typedef std::pair<TKey, TValue> TElem;
#define NULL_TVALUE -111111
#define NULL_TPAIR pair<TKey, TValue>(-111111, -111111);
class SMIterator;
typedef bool(*Relation)(TKey, TKey);

#define INITIAL_CAPACITY 11
#define CAPACITY_MULTIPLIER 2
#define MAX_LOAD_FACTOR 0.6

// The elements in the hash table will have 3 possible states: EMPTY, OCCUPIED, DELETED
#define EMPTY 0
#define OCCUPIED 1
#define DELETED 2

class SortedMap {
    friend class SMIterator;
private:
    TElem* elems;	// Holds all the (key, value) pairs in the hash table
    int* element_state;		// Will hold the state of each element in the hash table: EMPTY, DELETED, or OCCUPIED
    int capacity;		// The capacity of the hash table
    int nr_elems;		// The number of elements in the hash table
    Relation relation;		// The relation according to which the sorting will be done
    int h(TKey key, int i) const;	// The hash function
    bool is_prime(int n) const;		// Function that checks if a number is prime; used to find a valid hash table capacity
    void resize_and_rehash();	// Resize function
    double get_load_factor();	// Returns the current load factor of the hash table

public:

    // implicit constructor
    SortedMap(Relation r);

    // adds a pair (key,value) to the map
    //if the key already exists in the map, then the value associated to the key is replaced by the new value and the old value is returned
    //if the key SMes not exist, a new pair is added and the value null is returned
    TValue add(TKey c, TValue v);

    //searches for the key and returns the value associated with the key if the map contains the key or null: NULL_TVALUE otherwise
    TValue search(TKey c) const;

    //removes a key from the map and returns the value associated with the key if the key existed or null: NULL_TVALUE otherwise
    TValue remove(TKey c);

    //returns the number of pairs (key,value) from the map
    int size() const;

    //checks whether the map is empty or not
    bool isEmpty() const;

    // return the iterator for the map
    // the iterator will return the keys following the order given by the relation
    SMIterator iterator() const;

    // destructor
    ~SortedMap();
};
