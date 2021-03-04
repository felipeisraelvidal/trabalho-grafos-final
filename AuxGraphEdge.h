#ifndef AUXGRAPHEDGE_H_INCLUDED
#define AUXGRAPHEDGE_H_INCLUDED

#include <iostream>

class AuxGraphEdge {
private:
    int m_sourceId;
    int m_destinationId;
    float m_weight;

public:
    AuxGraphEdge() {}

    AuxGraphEdge(int sourceId, int destinationId, float weight) {
        m_sourceId = sourceId;
        m_destinationId = destinationId;
        m_weight = weight;
    }

    // Getters
    int getSourceId() {
        return m_sourceId;
    }

    int getDestinationId() {
        return m_destinationId;
    }

    int getWeight() {
        return m_weight;
    }

    // Setters
    void setSourceId(int sourceId) {
        m_sourceId = sourceId;
    }

    void setDestinationId(int destinationId) {
        m_destinationId = destinationId;
    }

    void setWeight(float weight) {
        m_weight = weight;
    }

    bool operator < (const AuxGraphEdge& edge2) const {
		return (m_weight < edge2.m_weight);
	}

};

#endif // AUXGRAPHEDGE_H_INCLUDED