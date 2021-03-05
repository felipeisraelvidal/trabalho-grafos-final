#ifndef GRAPHNODE_H_INCLUDED
#define GRAPHNODE_H_INCLUDED

#include <iostream>
#include "GraphEdge.h"

class GraphNode {
private:
    int m_id;
    float m_weight;

    int m_groupId;
    
    unsigned int m_inDegree;
    unsigned int m_outDegree;

    GraphEdge *firstEdge;
    GraphEdge *lastEdge;
    GraphNode *nextNode;

public:
    GraphNode(int id);

    // Getters
    int getId();
    float getWeight();
    int getGroupId();
    
    int getDegree();
    int getInDegree();
    int getOutDegree();
    
    GraphEdge* getFirstEdge();
    GraphEdge* getLastEdge();
    
    GraphNode* getNextNode();

    // Setters
    void setWeight(float weight);
    void setNextNode(GraphNode *node);
    void setGroupId(int groupId);

    // Edges
    void insertEdge(GraphNode *targetNode, float weight, bool isDirected);
    void removeEdge(GraphNode *targetNode, bool isDirected);
    void removeAllEdges();

    // Miscellaneous
    void incrementInDegree();
    void decrementInDegree();
    void incrementOutDegree();
    void decrementOutDegree();

};

#endif // GRAPHNODE_H_INCLUDED