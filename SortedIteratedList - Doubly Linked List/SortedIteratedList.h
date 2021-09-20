#pragma once
//DO NOt INCLUDE LISTITERATOR

//DO NOT CHANGE THIS PART
class ListIterator;
typedef int TComp;
typedef bool (*Relation)(TComp, TComp);
typedef bool (*Condition)(TComp);
#define NULL_TCOMP -11111

class SortedIteratedList {
private:
    friend class ListIterator;
    struct DLLNode {
        TComp info;
        DLLNode* next;
        DLLNode* previous;
    };

private:
    Relation relation;
    DLLNode* head;
    DLLNode* tail;
    int nr_elems;

public:
    // constructor
    SortedIteratedList(Relation r);

    // returns the number of elements from the list
    int size() const;

    // verifies if the list is empty
    bool isEmpty() const;

    // returns the first position from the list
    ListIterator first() const;

    // returns the element from the given position
    //throws an exception if the position is not valid
    TComp getElement(ListIterator pos) const;

    //keeps in the SortedList only the elements that respect the given condition
    void filter(Condition cond);

    // adds a new element to the list
    void add(TComp e);

    // removes the element from position pos
    //returns the removed element
    //after removal pos is positioned on the next element (the one after the removed one) or it is invalid if the last element was removed
    //throws an exception if the position is not valid
    TComp remove(ListIterator& poz);

    // searches for the first occurrence of an element
    //returns an iterator that points to the element, if it appear in the list, or an invalid iterator if the element is not in the list
    ListIterator search(TComp e) const;

    //destructor
    ~SortedIteratedList();

};
