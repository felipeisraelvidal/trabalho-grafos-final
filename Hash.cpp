#include <iostream>
#include "Hash.h"
#include "HashNode.h"

Hash::Hash(int order) {
    if (order >= 0) {
        m_order = order;
        table = (HashNode **)malloc(sizeof(HashNode) * m_order);
        for (int i = 0; i < m_order; i++) {
            table[i] = NULL;
        }
    }
}

Hash::~Hash() {
    delete table;
}

// Miscellaneous
int Hash::hashCode(int key) {
    return key % m_order;
}

// Insertion and removal
void Hash::insert(int key, int value) {
    HashNode *temp = new HashNode(key, value);

    int hashIndex = hashCode(key);

    while (table[hashIndex] != NULL && table[hashIndex]->getKey() != key && table[hashIndex]->getKey() != -1) {
        hashIndex++;
        hashIndex %= m_order;
    }

    table[hashIndex] = temp;

    std::cout << temp->getKey() << ": " << temp->getValue() << "\n";
}

// Getters
int Hash::get(int key) {
    
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

HashNode* Hash::getFromIndex(int index) {
    return table[index];
}

// Miscellaneous
void Hash::print() {
    for (int i = 0; i < m_order; i++) {
        if (table[i] == NULL) {
            std::cout << "-1 ";
        } else {
            std::cout << table[i]->getValue() << " ";
        }
    }
    std::cout << "\n";
}

void Hash::printKeys() {
    for (int i = 0; i < m_order; i++) {
        if (table[i] == NULL) {
            std::cout << "-1 ";
        } else {
            std::cout << table[i]->getKey() << " ";
        }
    }
    std::cout << "\n";
}

void Hash::update(int index, int value) {
    table[index]->setValue(value);
}

void Hash::updateKey(int key, int value) {
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