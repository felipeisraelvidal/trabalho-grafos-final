#ifndef GRAPHEDGE_H_INCLUDED
#define GRAPHEDGE_H_INCLUDED

#include <iostream>
#include <memory>

class GraphEdge {
private:
    int m_id;
    int m_targetId;
    float m_weight;
    
    GraphEdge *nextEdge;

public:
    GraphEdge(int id, int targetId);

    // Getters
    int getId();
    int getTargetId();
    float getWeight();
    
    GraphEdge* getNextEdge();

    // Setters
    void setWeight(float weight);
    void setNextEdge(GraphEdge *edge);

};

#endif // GRAPHEDGE_H_INCLUDED