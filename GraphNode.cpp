#include <iostream>
#include <algorithm>
#include <memory>
#include "GraphNode.h"
#include "GraphEdge.h"

GraphNode::GraphNode(int id) {
    m_id = id;
    
    firstEdge = nullptr;
    lastEdge = nullptr;
    nextNode = nullptr;
}

// Getters
int GraphNode::getId() {
    return m_id;
}

float GraphNode::getWeight() {
    return m_weight;
}

int GraphNode::getGroupId() {
    return m_groupId;
}

int GraphNode::getDegree() {
    return m_inDegree + m_outDegree;
}

int GraphNode::getInDegree() {
    return m_inDegree;
}

int GraphNode::getOutDegree() {
    return m_outDegree;
}

GraphEdge* GraphNode::getFirstEdge() {
    return firstEdge;
}

GraphEdge* GraphNode::getLastEdge() {
    return lastEdge;
}

GraphNode* GraphNode::getNextNode() {
    return nextNode;
}

// Setters
void GraphNode::setWeight(float weight) {
    m_weight = weight;
}

void GraphNode::setNextNode(GraphNode *node) {
    nextNode = node;
}

// Edges
void GraphNode::insertEdge(GraphNode *targetNode, float weight, bool isDirected) {
    GraphEdge *newEdge = new GraphEdge(m_id, targetNode->getId());
    newEdge->setWeight(weight);

    if (firstEdge != nullptr) {
        lastEdge->setNextEdge(newEdge);
        lastEdge = newEdge;
    } else {
        firstEdge = newEdge;
        lastEdge = newEdge;
    }

    if (isDirected) {
        incrementOutDegree();
        targetNode->incrementInDegree();
    } else {
        incrementOutDegree();
        incrementInDegree();

        targetNode->incrementOutDegree();
        targetNode->incrementInDegree();
    }
}

void GraphNode::removeEdge(GraphNode *targetNode, bool isDirected) {

}

void GraphNode::removeAllEdges() {
    GraphEdge *edge = firstEdge;
    while (edge != nullptr) {
        GraphEdge *aux = edge->getNextEdge();
        delete edge;
        edge = aux;
    }
}

void GraphNode::setGroupId(int groupId) {
    m_groupId = groupId;
}

// Miscellaneous
void GraphNode::incrementInDegree() {
    m_inDegree++;
}

void GraphNode::decrementInDegree() {
    m_inDegree--;
}

void GraphNode::incrementOutDegree() {
    m_outDegree++;
}

void GraphNode::decrementOutDegree() {
    m_outDegree--;
}