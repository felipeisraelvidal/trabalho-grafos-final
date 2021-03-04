#ifndef HASHNODE_H_INCLUDED
#define HASHNODE_H_INCLUDED

template <class T>
class HashNode {
private:
    int m_key;
    T m_value;
    
public:
    HashNode(int key, T value) {
        m_key = key;
        m_value = value;
    }

    // Getters
    int getKey() {
        return m_key;
    }

    T getValue() {
        return m_value;
    }

    // Setters
    void setValue(T value) {
        m_value = value;
    }

};

#endif // GRAPHNODE_H_INCLUDED
