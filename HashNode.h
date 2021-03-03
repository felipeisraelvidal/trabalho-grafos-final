#ifndef HASHNODE_H_INCLUDED
#define HASHNODE_H_INCLUDED

class HashNode {
private:
    int m_key;
    int m_value;
public:
    HashNode(int key, int value) {
        m_key = key;
        m_value = value;
    }

    // Getters
    int getKey() {
        return m_key;
    }

    int getValue() {
        return m_value;
    }

    // Setters
    void setValue(int value) {
        m_value = value;
    }

};

#endif // GRAPHNODE_H_INCLUDED
