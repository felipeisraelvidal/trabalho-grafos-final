#ifndef HASH_H_INCLUDED
#define HASH_H_INCLUDED

#include <iostream>
#include "HashNode.h"

template <class T>
class Hash {
private:
    int m_order;
    HashNode<T> **table;

    // Miscellaneous
    int hashCode(int key) {
        return key % m_order;
    }

public:
    Hash(int order) {
        if (order >= 0) {
            m_order = order;
            table = (HashNode<T> **)malloc(sizeof(HashNode<T>) * m_order);
            for (int i = 0; i < m_order; i++) {
                table[i] = NULL;
            }
        }
    }

    ~Hash() {
        delete table;
    }

    // Insertion and removal
    void insert(int key, T value) {
        HashNode<T> *temp = new HashNode<T>(key, value);

        int hashIndex = hashCode(key);

        while (table[hashIndex] != NULL && table[hashIndex]->getKey() != key && table[hashIndex]->getKey() != -1) {
            hashIndex++;
            hashIndex %= m_order;
        }

        table[hashIndex] = temp;
    }

    // Getters
    T get(int key) {
        int hashIndex = hashCode(key);

        int counter = 0;
        while (table[hashIndex] != NULL) {
            counter = 0;
            if (counter++ > m_order) {
                return -1;
            }

            if (table[hashIndex]->getKey() == key) {
                return table[hashIndex]->getValue();
            }

            hashIndex++;
            hashIndex %= m_order;
        }

        return -1;
    }

    HashNode<T>* getFromIndex(int index) {
        return table[index];
    }

    // Miscellaneous
    void print() {
        for (int i = 0; i < m_order; i++) {
            if (table[i] == NULL) {
                std::cout << "-1\t ";
            } else {
                std::cout << table[i]->getValue() << "\t";
            }
        }
        std::cout << "\n";
    }
    
    void printKeys() {
        for (int i = 0; i < m_order; i++) {
            if (table[i] == NULL) {
                std::cout << "-1\t";
            } else {
                std::cout << table[i]->getKey() << "\t";
            }
        }
        std::cout << "\n";
    }
    
    void updateAtIndex(int index, T value) {
        table[index]->setValue(value);
    }
    
    void updateKey(int key, T value) {
        int hashIndex = hashCode(key);

        int counter = 0;
        while (table[hashIndex] != NULL) {
            counter = 0;
            if (counter++ > m_order) {
                return;
            }

            if (table[hashIndex]->getKey() == key) {
                table[hashIndex]->setValue(value);
                return;
            }

            hashIndex++;
            hashIndex %= m_order;
        }
    }

};

#endif // HASH_H_INCLUDED