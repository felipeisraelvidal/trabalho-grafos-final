#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include <iostream>
#include <fstream>
#include <vector>
#include "GraphNode.h"
#include "Hash.h"
#include "AuxGraphEdge.h"

class Graph {
private:
    int m_order;

    bool m_isDirected;
    bool m_isWeightedEdges;
    bool m_isWeightedNodes;

    GraphNode *firstNode;
    GraphNode *lastNode;

    int edgesCount;

    int m_numberOfGroups;

    std::vector<AuxGraphEdge> m_edges;

    // Dijkstra
    int minDistance(Hash<int> *dist, Hash<bool> *visited);
    void auxDijkstra(Hash<float> *dist, Hash<bool> *visited, Hash<int> *previous, Hash<float> *previousWeight, int atual);

    // Floyd
    int** generateFloydMatrix();

    // Kruskal
    int findInSubtree(int subtree[], int i); // searches the subtrees for element i
    void joinSubtrees(int *subtree, int u, int v);

    // Prim
    bool isNodeVisited(int *visitedNodes, int id, int tam);

    // Miscellaneous
    template<class T>    
    int getIndexFromArray(T *vet, int tam, T val) {
        for (int i = 0; i < tam; i++) {
            if (vet[i] == val) {
                return i;
            }
        }
        return -1;
    }

    // Topological sort
    void topologicalSortUtil(int nodeId, Hash<bool> *visited, int *result, int *cont);

    // Greedy
    std::vector<AuxGraphEdge> constroiLCR(std::vector<AuxGraphEdge> edges, float alpha);
    int randomEdge(std::vector<AuxGraphEdge> edges);

public:
    Graph(bool isDirected, bool isWeightedEdges, bool isWeightedNodes);
    ~Graph();

    // Getters
    int getOrder(); // returns the order of the graph
    
    int isDirected(); // checks if it is oriented
    int isWeightedEdges(); // checks if it is weighted at the edges
    int isWeightedNodes(); // checks if it is weighted at the node

    int numberOfEdges(); // returns the number of edges

    GraphNode* getFirstNode(); // returns a pointer to the first node
    GraphNode* getLastNode(); // returns a pointer to then last node

    int getDegree();

    GraphNode* getNode(int id); // returns a pointer to the node with id
    GraphEdge* getEdgeBetween(int id, int targetId); // returns a pointer to the edge between id and targetId

    // Setters
    void setNumberOfGroups(int qtd);

    // Insertion and removal
    void insertNode(int id, float weight, int groupId);
    void insertEdge(int id, int targetId, float weight);
    void removeNode(int id);

    // Miscellaneous
    bool hasEdgeBetween(int id, int targetId); // checks if the graph has edges between the id and the targetId
    bool searchNode(int id); // checks if there is a node with the id
    void print(); // print the graph

    // Induced subgraph
    Graph* inducedSubgraph(std::ofstream &output);

    // Breadth-first search (Busca em Largura)
    void bfs(std::ofstream &output);

    // Dijkstra
    void dijkstra(std::ofstream &output);

    // Floyd
    void floyd(std::ofstream &output);

    // Kruskal
    void kruskal(std::ofstream &output);

    // Prim
    void prim(std::ofstream &output);

    // Topological sort
    int* topologicalSort(std::ofstream &output);

    // Greedy
    void greedy(std::ofstream &output);
    void randomizedGreedy(float *alphaValues, int tam, int repeat, std::ofstream &output);

};

#endif // GRAPH_H_INCLUDED