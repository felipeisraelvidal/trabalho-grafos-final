#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <stack>
#include <list>
#include "Graph.h"
#include "AuxGraphEdge.h"
#include "Hash.h"

Graph::Graph(bool isDirected, bool isWeightedEdges, bool isWeightedNodes) {
    m_order = 0;

    m_isDirected = isDirected;
    m_isWeightedEdges = isWeightedEdges;
    m_isWeightedNodes = isWeightedNodes;

    edgesCount = 0;
}

Graph::~Graph() {
    GraphNode *node = firstNode;
    while (node != nullptr) {
        node->removeAllEdges();

        GraphNode *auxNode = node->getNextNode();
        delete node;
        node = auxNode;
    }
}

// Getters
int Graph::getOrder() {
    return m_order;
}
    
int Graph::isDirected() {
    return m_isDirected;
}

int Graph::isWeightedEdges() {
    return m_isWeightedEdges;
}

int Graph::isWeightedNodes() {
    return m_isWeightedNodes;
}

int Graph::numberOfEdges() {
    return edgesCount;
}

GraphNode* Graph::getFirstNode() {
    return firstNode;
}

GraphNode* Graph::getLastNode() {
    return lastNode;
}

int Graph::getDegree() {
    return 0;
}

GraphNode* Graph::getNode(int id) {
    if (firstNode != nullptr) {
        GraphNode *node = firstNode;
        while (node != nullptr) {
            if (node->getId() == id) {
                return node;
            }
            node = node->getNextNode();
        }
    }
    return nullptr;
}

GraphEdge* Graph::getEdgeBetween(int id, int targetId) {
    GraphNode *node = firstNode;
    while (node != nullptr) {
        GraphEdge *edge = node->getFirstEdge();
        while (edge != nullptr) {
            if (node->getId() == id && edge->getTargetId() == targetId) {
                return edge;
            }
            edge = edge->getNextEdge();
        }
        node = node->getNextNode();
    }
    return nullptr;
}

// Insertion and removal
void Graph::insertNode(int id, float weight) {
    if (!searchNode(id)) {
        GraphNode *newNode = new GraphNode(id);
        newNode->setWeight(weight);

        if (lastNode != nullptr) { // nodes list is not empty
            lastNode->setNextNode(newNode);
            lastNode = newNode;
        } else { // nodes list is empty
            firstNode = newNode;
            lastNode = newNode;
        }

        m_order++;
    }
}

void Graph::insertEdge(int id, int targetId, float weight) {
    if (!searchNode(id)) {
        insertNode(id, 0);
    }
    if (!searchNode(targetId)) {
        insertNode(targetId, 0);
    }

    if (m_isDirected) { // insert edge between id and targetId only
        if (!hasEdgeBetween(id, targetId)) {
            GraphNode *node = getNode(id);
            GraphNode *targetNode = getNode(targetId);

            node->insertEdge(targetNode, weight, m_isDirected);

            // Insert new edge in list
            AuxGraphEdge newEdge(id, targetId, weight);
            edges.push_back(newEdge);

            edgesCount++;
        }
    } else { // insert edge between id and targetId and reverse
        GraphNode *node = getNode(id);
        GraphNode *targetNode = getNode(targetId);

        if (!hasEdgeBetween(id, targetId)) {
            // insert edge between id and targetId
            node->insertEdge(targetNode, weight, m_isDirected);

            // Insert new edge in list
            AuxGraphEdge newEdge(id, targetId, weight);
            edges.push_back(newEdge);

            edgesCount++;
        }
        if (!hasEdgeBetween(targetId, id)) {
            targetNode->insertEdge(node, weight, m_isDirected);

            // Insert new edge in list
            AuxGraphEdge newEdge(targetId, id, weight);
            edges.push_back(newEdge);

            edgesCount++;
        }
    }
}

void Graph::removeNode(int id) {

}

// Miscellaneous
bool Graph::hasEdgeBetween(int id, int targetId) {
    GraphNode *node = firstNode;
    while (node != nullptr) {
        GraphEdge *edge = node->getFirstEdge();
        while (edge != nullptr) {
            if (node->getId() == id && edge->getTargetId() == targetId) {
                return true;
            }
            edge = edge->getNextEdge();
        }
        node = node->getNextNode();
    }
    return false;
}

bool Graph::searchNode(int id) {
    if (firstNode != nullptr) {
        GraphNode *node = firstNode;
        while (node != nullptr) {
            if (node->getId() == id) {
                return true;
            }
            node = node->getNextNode();
        }
    }
    return false;
}

void Graph::print() {
    if (firstNode != nullptr) {
        GraphNode *node = firstNode;
        while (node != nullptr) {
            if (m_isWeightedNodes) {
                std::cout << node->getId() << "(" << node->getWeight() << "):";
            } else {
                std::cout << node->getId() << ":";
            }
            
            GraphEdge *edge = node->getFirstEdge();
            while (edge != nullptr) {
                if (m_isWeightedEdges) {
                    std::cout << " -> edge: " << edge->getTargetId() << " (" << edge->getWeight() << ")";
                } else {
                    std::cout << " -> edge: " << edge->getTargetId();
                }
                edge = edge->getNextEdge();
            }
            
            node = node->getNextNode();
            
            if (node != nullptr) {
                std::cout << "\n";
            }
        }
        std::cout << "\n";
    } else {
        std::cout << "Empty Graph.\n";
    }
}

// Induced subgraph
Graph* Graph::inducedSubgraph(int *vet, int tam, std::ofstream &output) {
    int *check = (int *)malloc(sizeof(int *) * m_order);
    int *nodes = (int *)malloc(sizeof(int *) * m_order);

    Graph *subgraph = new Graph(m_isDirected, m_isWeightedEdges, m_isWeightedNodes);
    
    GraphNode *node = firstNode;

    std::string graphType;
    std::string nodesSeparator;
    if (m_isDirected) {
        graphType = "digraph";
        nodesSeparator = "->";
    } else {
        graphType = "graph";
        nodesSeparator = "--";
    }

    output << graphType << " SubgrafoInduzido {\n";

    for (int i = 0; i < m_order; i++) {
        check[i] = 0;
        nodes[i] = node->getId();
        node = node->getNextNode();
    }

    for (int i = 0; i < tam; i++) {
        std::cout << "teste: " << vet[i] << "\n";
        if (!searchNode(vet[i])) {
            std::cout << "Elemento " << vet[i] << " nao pertence ao grafo. Não eh possivel gerar o subgrafo induzido.\n";

            delete check;
            delete nodes;
            return nullptr;
        }

        if (subgraph->searchNode(vet[i]) == false) {
            subgraph->insertNode(vet[i], 0);
        }

        int ind = getIndexFromArray<int>(nodes, m_order, vet[i]);
        if (check[ind] == 0) {
            check[ind] = 1;
        } else {
            std::cout << "Elementos repetidos no vetor.\n";
            delete check;
            delete nodes;
            return nullptr;
        }

        GraphNode *v = getNode(vet[i]);
        GraphEdge *edge;
        for (int j = 0; j < tam; j++) {
            if (vet[i] != vet[j]) {
                bool aux = hasEdgeBetween(v->getId(), vet[j]);
                if (aux == true) {
                    if (subgraph->hasEdgeBetween(vet[i], vet[j]) == false) {
                        edge = v->getFirstEdge();
                        while (edge != nullptr) {
                            if (edge->getTargetId() == vet[j]) {
                                subgraph->insertEdge(vet[i], vet[j], edge->getWeight());
                                // saida << "\t" << vet[i] << " " << nodesSeparator << " " << vet[j] << "\n";
                            }
                            edge = edge->getNextEdge();
                        }
                    }
                }
            }
        }
    }

    output << "}";
    subgraph->print();

    delete check;
    delete nodes;

    return subgraph;
}

// Breadth-first search
void Graph::bfs(int id, std::ofstream &output) {
    int *map = (int *)malloc(sizeof(int *) * m_order);
    bool *visited = (bool *)malloc(sizeof(bool *) * m_order);
    int index;

    GraphNode *node = firstNode;

    for (int i = 0; i < m_order; i++, node = node->getNextNode()) {
        map[i] = node->getId();
        visited[i] = false;
    }

    std::queue<int> queue;
    index = getIndexFromArray<int>(map, m_order, id);
    visited[index] = true;
    queue.push(id);

    std::string graphType;
    std::string nodesSeparator;
    if (this->m_isDirected) {
        graphType = "digraph";
        nodesSeparator = "->";
    } else {
        graphType = "graph";
        nodesSeparator = "--";
    }

    output << graphType << " BuscaLargura {\n";
    std::cout << graphType << " BuscaLargura {\n";

    GraphNode *auxNode;

    while (!queue.empty()) {
        auxNode = getNode(queue.front());

        GraphEdge *auxEdge = auxNode->getFirstEdge();
        while (auxEdge != nullptr) {
            index = getIndexFromArray<int>(map, m_order, auxEdge->getTargetId());
            if (visited[index] == false) {
                output << "\t" << auxNode->getId() << " " << nodesSeparator << " " << auxEdge->getTargetId() << "\n";
                std::cout << "\t" << auxNode->getId() << " " << nodesSeparator << " " << auxEdge->getTargetId() << "\n";
                visited[index] = true;
                queue.push(auxEdge->getTargetId());
            } else {
                output << "\t" << auxNode->getId() << " " << nodesSeparator << " " << auxEdge->getTargetId() << " [style=dotted];\n";
                std::cout << "\t" << auxNode->getId() << " " << nodesSeparator << " " << auxEdge->getTargetId() << " [style=dotted];\n";
            }
            auxEdge = auxEdge->getNextEdge();
        }
        queue.pop();
    }
    output << "}\n";
    std::cout << "}\n";
}

// Prim
bool Graph::isNodeVisited(int *visitedNodes, int id, int tam) {
    for (int i = 0; i < tam; i++) {
        if (visitedNodes[i] == id) {
            return true;
        }
    }
    return false;
}

void Graph::prim(std::ofstream &saida) {
    int nodesCount = 0, edgesCountSolution = 0, i;
    float sumWeights = 0;
    int *visitedNodes = (int *)malloc(sizeof(int *) * m_order);

    AuxGraphEdge *edgesList = (AuxGraphEdge *)malloc(sizeof(AuxGraphEdge *) * edgesCount);
    AuxGraphEdge *solution = (AuxGraphEdge *)malloc(sizeof(AuxGraphEdge *) * edgesCount);

    GraphNode *node = firstNode;
    while (node != nullptr) {
        i = -1;
        visitedNodes[nodesCount] = node->getId();
        nodesCount++;
        for (GraphEdge *a = node->getFirstEdge(); a != nullptr; a = a->getNextEdge()) {
            //só adiciona arestas caso o no de destino ainda nao foi explorado
            if (!isNodeVisited(visitedNodes, a->getTargetId(), m_order)) {
                i++;
                edgesList[i].setSourceId(a->getId());
                edgesList[i].setDestinationId(a->getTargetId());
                edgesList[i].setWeight(a->getWeight());
            }
        }

        // Sort edges list
        AuxGraphEdge aux;
        for (int j = 0; j < i; j++) {
            for (int k = j + 1; k < i; k++) {
                if (edgesList[j].getWeight() > edgesList[k].getWeight()) {
                    aux = edgesList[j];
                    edgesList[j] = edgesList[k];
                    edgesList[k] = aux;
                }
            }
        }

        //Caso a lista de Arestas possiveis for vazio chegou no ultimo no da solução
        if (i == -1) {
            break;
        }

        solution[edgesCountSolution] = edgesList[0];
        edgesCountSolution++;
        node = getNode(edgesList[0].getDestinationId());
    }

    std::string graphType;
    std::string nodesSeparator;
    if (this->m_isDirected) {
        graphType = "digraph";
        nodesSeparator = "->";
    } else {
        graphType = "graph";
        nodesSeparator = "--";
    }

    saida << graphType << " Prim {\n";
    const std::string quote = "\"";

    for (int l = 0; l < edgesCountSolution; l++) {
        std::cout << "[" << solution[l].getSourceId() << " -> " << solution[l].getDestinationId() << "] - " << solution[l].getWeight() << "\n";
        saida << "\t" << solution[l].getSourceId() << " " << nodesSeparator << " " << solution[l].getDestinationId() << " [label" << quote << solution[l].getWeight() << quote << "]\n";
        sumWeights += solution[l].getWeight();
    }
    // cout << "Somatorio dos Pesos: " << somatorioPesos << endl;

    saida << "}\n";
}

// Topological sort
void Graph::topologicalSortUtil(int nodeId, Hash *visited, int *result, int *cont) {
    visited->updateKey(nodeId, true);

    GraphNode *node = getNode(nodeId);
    if (node != nullptr) {
        GraphEdge *edge = node->getFirstEdge();
        while (edge != nullptr) {
            if (visited->get(edge->getTargetId()) == false) {
                topologicalSortUtil(edge->getTargetId(), visited, result, cont);
            }
            edge = edge->getNextEdge();
        }
    }
    result[*cont - 1] = nodeId;
    *cont -= 1;
}

int* Graph::topologicalSort(std::ofstream &output) {
    Hash hash(m_order);

    GraphNode *node = firstNode;
    while (node != nullptr) {
        hash.insert(node->getId(), false);
        node = node->getNextNode();
    }

    int *result = (int *)malloc(sizeof(int) * m_order);
    int cont = m_order;

    node = firstNode;
    while (node != nullptr) {
        if (hash.get(node->getId()) == 0) {
            topologicalSortUtil(node->getId(), &hash, result, &cont);
        }
        node = node->getNextNode();
    }

    output << "Topological Sort:\n";

    for (int i = 0; i < m_order; i++) {
        std::cout << result[i] << " ";
        output << result[i] << "\n";
    }
    std::cout << "\n";

    return NULL;
}
