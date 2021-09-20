#pragma once
//DO NOT INCLUDE SETITERATOR

//DO NOT CHANGE THIS PART
typedef int TElem;
typedef TElem TComp;
typedef bool(*Relation)(TComp, TComp);
#define NULL_TELEM -11111
class SortedSetIterator;


class SortedSet {
    friend class SortedSetIterator;
private:
    int nr_elems;
    int capacity;
    TComp* elements;
    Relation relation;
    // Resize function
    void resize();
    // Function which finds the index of an element if it is in the set, and finds the index
    // where the element should be inserted such that the set will keep being sorted
    int corresponding_index(TComp elem);

public:
    //constructor
    SortedSet(Relation r);

    //adds all elements of s into the SortedSet (assume both SortedSets use the same relation)
    void unionize(const SortedSet& s);

    //adds an element to the sorted set
    //if the element was added, the operation returns true, otherwise (if the element was already in the set)
    //it returns false
    bool add(TComp e);

    //removes an element from the sorted set
    //if the element was removed, it returns true, otherwise false
    bool remove(TComp e);

    //checks if an element is in the sorted set
    bool search(TElem elem) const;

    //returns the number of elements from the sorted set
    int size() const;

    //checks if the sorted set is empty
    bool isEmpty() const;

    //returns an iterator for the sorted set
    SortedSetIterator iterator() const;

    // destructor
    ~SortedSet();


};
