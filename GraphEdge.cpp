#include <iostream>
#include <memory>
#include "GraphEdge.h"

GraphEdge::GraphEdge(int id, int targetId) {
    m_id = id;
    m_targetId = targetId;
    nextEdge = nullptr;
}

// Getters
int GraphEdge::getId() {
    return m_id;
}

int GraphEdge::getTargetId() {
    return m_targetId;
}

float GraphEdge::getWeight() {
    return m_weight;
}

GraphEdge* GraphEdge::getNextEdge() {
    return nextEdge;
}

// Setters
void GraphEdge::setWeight(float weight) {
    m_weight = weight;
}

void GraphEdge::setNextEdge(GraphEdge *edge) {
    nextEdge = edge;
}