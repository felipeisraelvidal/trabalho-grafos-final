#ifndef HASH_H_INCLUDED
#define HASH_H_INCLUDED

#include <iostream>
#include "HashNode.h"

class Hash {
private:
    int m_order;
    HashNode **table;

    // Miscellaneous
    int hashCode(int key);

public:
    Hash(int order);
    ~Hash();

    // Insertion and removal
    void insert(int key, int value);

    // Getters
    int get(int key);
    HashNode* getFromIndex(int index);

    // Miscellaneous
    void print();
    void printKeys();
    void update(int index, int value);
    void updateKey(int key, int value);

};

#endif // HASH_H_INCLUDED