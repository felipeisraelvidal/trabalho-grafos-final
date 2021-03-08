#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <set>
#include <list>
#include <vector>
#include <time.h>
#include <unistd.h>
#include "Graph.h"
#include "AuxGraphEdge.h"
#include "Hash.h"

Graph::Graph(bool isDirected, bool isWeightedEdges, bool isWeightedNodes) {
    m_order = 0;

    m_isDirected = isDirected;
    m_isWeightedEdges = isWeightedEdges;
    m_isWeightedNodes = isWeightedNodes;

    m_numberOfGroups = 0;
    
    firstNode = nullptr;
    lastNode = nullptr;

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

// Setters
void Graph::setNumberOfGroups(int qtd) {
    m_numberOfGroups = qtd;
}

// Insertion and removal
void Graph::insertNode(int id, float weight, int groupId) {
    if (!searchNode(id)) {
        GraphNode *newNode = new GraphNode(id);
        newNode->setWeight(weight);

        if (groupId != -1) {
            newNode->setGroupId(groupId);
        }

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
        insertNode(id, 0, -1);
    }
    if (!searchNode(targetId)) {
        insertNode(targetId, 0, -1);
    }

    if (m_isDirected) { // insert edge between id and targetId only
        if (!hasEdgeBetween(id, targetId)) {
            GraphNode *node = getNode(id);
            GraphNode *targetNode = getNode(targetId);

            node->insertEdge(targetNode, weight, m_isDirected);

            // Insert new edge in list
            AuxGraphEdge newEdge(id, targetId, weight);
            m_edges.push_back(newEdge);

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
            this->m_edges.push_back(newEdge);

            edgesCount++;
        }
        if (!hasEdgeBetween(targetId, id)) {
            targetNode->insertEdge(node, weight, m_isDirected);

            // Insert new edge in list
            AuxGraphEdge newEdge(targetId, id, weight);
            this->m_edges.push_back(newEdge);

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
Graph* Graph::inducedSubgraph(std::ofstream &output) {
    // Enter params
    int tam;
    std::cout << "Size of nodes list: ";
    std::cin >> tam;
    int *vet = (int *)malloc(sizeof(int) * tam);
    for (int i = 0; i < tam; i++) {
        std::cout << "Node " << i + 1 << ": ";
        std::cin >> vet[i];
    }

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
    std::cout << graphType << " SubgrafoInduzido {\n";

    for (int i = 0; i < m_order; i++) {
        check[i] = 0;
        nodes[i] = node->getId();
        node = node->getNextNode();
    }

    for (int i = 0; i < tam; i++) {
        if (!searchNode(vet[i])) {
            std::cout << "Elemento " << vet[i] << " nao pertence ao grafo. Não eh possivel gerar o subgrafo induzido.\n";

            delete check;
            delete nodes;
            return nullptr;
        }

        if (subgraph->searchNode(vet[i]) == false) {
            subgraph->insertNode(vet[i], 0, -1);
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

                                output << "\t" << vet[i] << " " << nodesSeparator << " " << vet[j] << "\n";
                                std::cout << "\t" << vet[i] << " " << nodesSeparator << " " << vet[j] << "\n";
                            }
                            edge = edge->getNextEdge();
                        }
                    }
                }
            }
        }
    }

    output << "}\n";
    std::cout << "}\n";

    delete check;
    delete nodes;

    return subgraph;
}

// Breadth-first search
void Graph::bfs(std::ofstream &output) {
    if (m_isDirected) {
        std::cout << "Imprimir caminhamento em largura\n";
        int startingNode;
        std::cout << "Enter the starting node: ";
        std::cin >> startingNode;

        int *map = (int *)malloc(sizeof(int *) * m_order);
        bool *visited = (bool *)malloc(sizeof(bool *) * m_order);
        int index;

        GraphNode *node = firstNode;

        for (int i = 0; i < m_order; i++, node = node->getNextNode()) {
            map[i] = node->getId();
            visited[i] = false;
        }

        std::queue<int> queue;
        index = getIndexFromArray<int>(map, m_order, startingNode);
        visited[index] = true;
        queue.push(startingNode);

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
    } else {
        std::cout << "The graphs needs to be directed.\n";
    }
}

// Dijkstra
int Graph::minDistance(Hash<int> *dist, Hash<bool> *visited) {
    int min = INT_MAX, minIndex;
    GraphNode *node = firstNode;
    while (node != nullptr) {
        if (visited->get(node->getId()) == false && dist->get(node->getId()) <= min) {
            min = dist->get(node->getId());
            minIndex = node->getId();
        }
        node = node->getNextNode();
    }
    return -1;
}

void Graph::auxDijkstra(Hash<float> *dist, Hash<bool> *visited, Hash<int> *previous, Hash<float> *previousWeight, int currentId) {
    GraphNode *node = getNode(currentId);
    if (node != nullptr) {
        GraphEdge *edge = node->getFirstEdge();

        int edgeIndex;
        while (edge != nullptr) {
            edgeIndex = edge->getTargetId();

            if (dist->get(edgeIndex) != -1) {
                if (m_isWeightedEdges) {
                    if (dist->get(edgeIndex) > dist->get(currentId) + edge->getWeight()) {
                        dist->updateKey(edgeIndex, dist->get(currentId) + edge->getWeight());
                        previous->updateKey(edgeIndex, currentId);
                        previousWeight->updateKey(edgeIndex, edge->getWeight());
                    }
                } else {
                    if (dist->get(edgeIndex) > dist->get(currentId) + 1) {
                        dist->updateKey(edgeIndex, dist->get(currentId) + 1);
                        previous->updateKey(edgeIndex, currentId);
                        previousWeight->updateKey(edgeIndex, edge->getWeight());
                    }
                }
            } else {
                if (m_isWeightedEdges) {
                    dist->updateKey(edgeIndex, dist->get(currentId) + edge->getWeight());
                    previous->updateKey(edgeIndex, currentId);
                    previousWeight->updateKey(edgeIndex, edge->getWeight());
                } else {
                    dist->updateKey(edgeIndex, dist->get(currentId) + 1);
                    previous->updateKey(edgeIndex, currentId);
                    previousWeight->updateKey(edgeIndex, edge->getWeight());
                }
            }
            edge = edge->getNextEdge();
        }

        int min = -1;

        GraphNode *auxNode = firstNode;
        while (auxNode != nullptr) {
            if (min == -1) {
                if (visited->get(auxNode->getId())) {
                    if (dist->get(auxNode->getId()) != -1) {
                        min = dist->get(auxNode->getId());
                        currentId = auxNode->getId();
                    }
                }
            }
            auxNode = auxNode->getNextNode();
        }

        if (min != -1) {
            GraphNode *auxNode2 = firstNode;
            while (auxNode2 != nullptr) {
                if (visited->get(auxNode2->getId())) {
                    if (dist->get(auxNode2->getId()) != -1) {
                        if (dist->get(auxNode2->getId()) < min) {
                            min = dist->get(auxNode2->getId());
                            currentId = auxNode2->getId();
                        }
                    }
                }
                auxNode2 = auxNode2->getNextNode();
            }
            visited->updateKey(currentId, false);
            auxDijkstra(dist, visited, previous, previousWeight, currentId);
        }
    }
}

void Graph::dijkstra(std::ofstream &output) {
    // Enter params
    int src, dest;
    std::cout << "Source: ";
    std::cin >> src;
    std::cout << "Destination: ";
    std::cin >> dest;

    Hash<float> dist(m_order);
    Hash<bool> visited(m_order);
    Hash<int> previous(m_order);
    Hash<float> previousWeight(m_order);

    GraphNode *no = firstNode;
    for (int i = 0; i < m_order; i++, no = no->getNextNode()) {
        if (no->getId() == src) {
            dist.insert(no->getId(), 0);
            visited.insert(no->getId(), false);
        } else {
            dist.insert(no->getId(), -1);
            visited.insert(no->getId(), true);
        }
        previous.insert(no->getId(), -1);
        previousWeight.insert(no->getId(), -1);
    }

    auxDijkstra(&dist, &visited, &previous, &previousWeight, src);

    if (dist.get(dest) != -1) {
        std::vector<std::pair<int, float> > finalList;

        finalList.insert(finalList.begin() + 0, std::make_pair(dest, previousWeight.get(dest)));

        int path = previous.get(dest);
        while (path != -1) {
            finalList.insert(finalList.begin() + 0, std::make_pair(path, previousWeight.get(path)));
            path = previous.get(path);
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

        output << graphType << " Dijkstra {\n";
        std::cout << graphType << " Dijkstra {\n";

        for (int i = 0; i < finalList.size(); i++) {
            if (finalList[i + 1].first != 0) {
                output << "\t" << finalList[i].first << " " << nodesSeparator << " " << finalList[i + 1].first << " [label" << "=" << finalList[i + 1].second << "]\n";
                std::cout << "\t" << finalList[i].first << " " << nodesSeparator << " " << finalList[i + 1].first << " [label" << "="  << finalList[i + 1].second << "]\n";

            }
        }

        std::cout << "}\n";
        output << "}\n";
    } else {
        std::cout << "An error occurred\n";
    }
}

// Floyd
int** Graph::generateFloydMatrix() {
    int n = m_order;
    int** distancias = (int **) malloc(sizeof(int *) * n);
    for(int i=0; i < n; i++){
        distancias[i] = (int *) malloc(sizeof(int) * n);
    }

    for(int i = 0; i<n; i++) {
        for(int j = 0; j<n; j++) {
            distancias[i][j] = 0;
        }
    }

    for(int i = 0; i<n; i++) {
        for(int j = 0; j<n; j++) {
            GraphEdge *edge = getEdgeBetween(i + 1, j + 1);
            if (edge != nullptr) {
                std::cout << edge->getId() << " - " << edge->getTargetId() << " (" << edge->getWeight() << ")" << "\n";;
                distancias[i][j] = edge->getWeight();
            }
        }
    }

    for(int aux = 0; aux < n; aux++) {
        for(int i = 0; i < n; i++) {
            for(int j = 0; j<n; j++) {
                if ((distancias[i][aux] * distancias[aux][j] != 0) && (i != j)){
                    if ((distancias[i][aux] + distancias[aux][j] < distancias[i][j]) || (distancias[i][j] == 0)){
                        distancias[i][j] = distancias[i][aux] + distancias[aux][j];
                    }
                }
            }
        }
    }

    return distancias;
}

void Graph::floyd(std::ofstream &output) {
    int src, dest;
    std::cout << "Source: ";
    std::cin >> src;
    std::cout << "Destination: ";
    std::cin >> dest;

    int** dist = generateFloydMatrix();

    output << "Floyd\n";
    output << "Dist between " << src << " and " << dest << ": " << dist[src - 1][dest - 1] << "\n";
}

// Kruskal
int Graph::findInSubtree(int subtree[], int i) {
    if (subtree[i] == -1) {
        return i;
    }
    return findInSubtree(subtree, subtree[i]);
}

void Graph::joinSubtrees(int *subtree, int u, int v) {
    int u_set = findInSubtree(subtree, u);
    int v_set = findInSubtree(subtree, v);
    subtree[u_set] = v_set;
}

void Graph::kruskal(std::ofstream &output) {
    std::vector<AuxGraphEdge> vetEdges;

    float weight = 0;

    sort(m_edges.begin(), m_edges.end());

    int *subtrees = (int *)malloc(sizeof(int) * m_order);
    memset(subtrees, -1, sizeof(int) * m_order);

    std::string graphType;
    std::string nodesSeparator;
    if (this->m_isDirected) {
        graphType = "digraph";
        nodesSeparator = "->";
    } else {
        graphType = "graph";
        nodesSeparator = "--";
    }

    output << graphType << " Kruskal {\n";
    std::cout << graphType << " Kruskal {\n";

    for (int i = 0; i < m_edges.size(); i++) {
        int u = findInSubtree(subtrees, m_edges.at(i).getSourceId());
        int v = findInSubtree(subtrees, m_edges.at(i).getDestinationId());

        if (u != v) {
            vetEdges.push_back(m_edges.at(i));
            joinSubtrees(subtrees, u, v);
        }
    }

    for (int i = 0; i < vetEdges.size(); i++) {
        int v1 = vetEdges.at(i).getSourceId();
        int v2 = vetEdges.at(i).getDestinationId();

        weight += vetEdges.at(i).getWeight();

        std::cout << "\t" << v1 << " " << nodesSeparator << " " << v2 << " [label" << "=" << vetEdges[i].getWeight() << "]\n";
        output << "\t" << v1 << " " << nodesSeparator << " " << v2 << " [label" << "=" << vetEdges[i].getWeight() << "]\n";
    }

    std::cout << "}\n";
    output << "}\n";
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
    int *visitedNodes = (int *)malloc(sizeof(int) * m_order);

    AuxGraphEdge *solution = (AuxGraphEdge *)malloc(sizeof(AuxGraphEdge *) * edgesCount);

    std::vector<AuxGraphEdge> vetEdges;
    for (int i = 0; i < edgesCount; i++) {
        AuxGraphEdge newEdge(INT_MAX, INT_MAX, INT_MAX);
        vetEdges.push_back(newEdge);
    }

    GraphNode *node = firstNode;
    while (node != nullptr) {
        i = -1;
        visitedNodes[nodesCount] = node->getId();
        nodesCount++;
        for (GraphEdge *a = node->getFirstEdge(); a != nullptr; a = a->getNextEdge()) {
            // add edge if the target id was not visited yet
            if (!isNodeVisited(visitedNodes, a->getTargetId(), m_order)) {
                i++;
                vetEdges[i].setSourceId(a->getId());
                vetEdges[i].setDestinationId(a->getTargetId());
                vetEdges[i].setWeight(a->getWeight());
            }
        }

        std::sort(vetEdges.begin(), vetEdges.end());

        if (i == -1) {
            break;
        }

        solution[edgesCountSolution] = vetEdges.at(0);
        edgesCountSolution++;
        node = getNode(vetEdges.at(0).getDestinationId());
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
    std::cout << graphType << " Prim {\n";

    for (int l = 0; l < edgesCountSolution; l++) {
        saida << "\t" << solution[l].getSourceId() << " " << nodesSeparator << " " << solution[l].getDestinationId() << " [label" << "=" << solution[l].getWeight() << "]\n";
        std::cout << "\t" << solution[l].getSourceId() << " " << nodesSeparator << " " << solution[l].getDestinationId() << " [label" << "=" << solution[l].getWeight() << "]\n";
        sumWeights += solution[l].getWeight();
    }
    
    std::cout << "Somatorio dos Pesos: " << sumWeights << "\n";

    saida << "}\n";
    std::cout << "}\n";
}

// Topological sort
void Graph::topologicalSortUtil(int nodeId, Hash<bool>*visited, int *result, int *cont) {
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
    if (m_isDirected) {
        Hash<bool> hash(m_order);

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

        return result;
    } else {
        std::cout << "The graphs needs to be directed.\n";
        return nullptr;
    }
}

// Greedy
void Graph::greedy(std::ofstream &output) {

    clock_t tInicio, tFim, tDecorrido;

    tInicio = clock();

    std::vector<AuxGraphEdge> vetEdges = m_edges;
    std::sort(vetEdges.begin(), vetEdges.end());

    std::vector<AuxGraphEdge> resultado;
    float sumWeight = 0;

    int *listGroups = (int *)malloc(sizeof(int) * m_numberOfGroups);

    int *subgroups = (int *)malloc(sizeof(int) * m_numberOfGroups);
    memset(subgroups, 0, sizeof(int) * m_numberOfGroups);

    int *subtrees = (int *)malloc(sizeof(int) * m_order);
    memset(subtrees, -1, sizeof(int) * m_order);
    
    int num = m_numberOfGroups;
    std::vector<AuxGraphEdge> L;
    while (num > 1) {
        AuxGraphEdge edge = vetEdges.front();

        // remover aresta do vetEdges
        vetEdges.erase(vetEdges.begin());
        if (!m_isDirected) {
            for (int i = 0; i < vetEdges.size(); i++) {
                if (vetEdges.at(i).getSourceId() == edge.getDestinationId() && vetEdges.at(i).getDestinationId() == edge.getSourceId()) {
                    vetEdges.erase(vetEdges.begin() + i);
                }
            }
        }

        int u = findInSubtree(subtrees, edge.getSourceId());
        int v = findInSubtree(subtrees, edge.getDestinationId());

        if (u != v) {
            GraphNode *nodeU = getNode(edge.getSourceId());
            GraphNode *nodeV = getNode(edge.getDestinationId());
            if (nodeU != nullptr && nodeV != nullptr) {
                int gu = nodeU->getGroupId();
                int gv = nodeV->getGroupId();

                if ((subgroups[gu - 1] == nodeU->getId() || subgroups[gu - 1] == 0) && (subgroups[gv - 1] == nodeV->getId() || subgroups[gv - 1] == 0)) {
                    
                    resultado.push_back(edge);
                    sumWeight += edge.getWeight();
                    joinSubtrees(subtrees, u, v);

                    num--;
                    if (subgroups[gu - 1] == 0) {
                        subgroups[gu - 1] = nodeU->getId();
                    }
                    if (subgroups[gv - 1] == 0) {
                        subgroups[gv - 1] = nodeV->getId();
                    }
                }
            }
        }
    }

    tFim = clock();

    tDecorrido = ((tFim - tInicio) / (CLOCKS_PER_SEC / 1000));

    output << "Time: " << tDecorrido << "ms\n";

    output << "Total Weight: " << sumWeight << "\n";
    float averageWeight = sumWeight / float(resultado.size());
    output << "Average Weight: " << averageWeight << "\n";

    output << "\n";

    // output << "graph Greedy {\n";
    // for (int i = 0; i < resultado.size(); i++) {
    //     output << "\t" << resultado.at(i).getSourceId() << " -- " << resultado.at(i).getDestinationId() << " [label=" << resultado.at(i).getWeight() << "]\n";
    // }
    // output << "}\n";
}

std::vector<AuxGraphEdge> Graph::constroiLCR(std::vector<AuxGraphEdge> edges, float alpha) {
    float c_min = edges.begin()->getWeight();
    float c_max = edges.back().getWeight();
    float c_total = (c_min + (alpha * (c_max-c_min)));

    std::vector<AuxGraphEdge> result;
    for (int i = 0; i < edges.size(); i++) {
        if (edges[i].getWeight() <= c_total) {
            result.push_back(edges[i]);
        }
    }

    return result;
}

int Graph::randomEdge(std::vector<AuxGraphEdge> edges) {
    std::srand(time(NULL));
    int index = rand() % (edges.size());
    return index;
}

void Graph::randomizedGreedy(float *alphaValues, int tam, int repeat, std::ofstream &output) {
    for (int i = 0; i < tam; i++) {
        float alpha = alphaValues[i];

        if (i != 0) {
            output << "\n";
        }

        output << "α: " << alpha << "\n";

        output << "\n";

        for (int j = 0; j < repeat; j++) {

            sleep(1);

            clock_t tInicio, tFim, tDecorrido;

            tInicio = clock();

            std::vector<AuxGraphEdge> vetEdges = m_edges;
            std::sort(vetEdges.begin(), vetEdges.end());

            std::vector<AuxGraphEdge> resultado;
            float sumWeight = 0;

            int *listGroups = (int *)malloc(sizeof(int) * m_numberOfGroups);

            int *subgroups = (int *)malloc(sizeof(int) * m_numberOfGroups);
            memset(subgroups, 0, sizeof(int) * m_numberOfGroups);

            int *subtrees = (int *)malloc(sizeof(int) * m_order);
            memset(subtrees, -1, sizeof(int) * m_order);
            
            int num = m_numberOfGroups;
            std::vector<AuxGraphEdge> L;
            while (num > 1) {
                L = constroiLCR(vetEdges, alpha);

                int index = randomEdge(L);
                AuxGraphEdge edge = L.at(index);

                // std::cout << "Checking edge: " << edge.getSourceId() << " - " << edge.getDestinationId() << " (" << edge.getWeight() << ")";
                // fflush(stdout);

                // remover aresta do vetEdges
                for (int i = 0; i < vetEdges.size(); i++) {
                    if (vetEdges.at(i).getSourceId() == edge.getSourceId() && vetEdges.at(i).getDestinationId() == edge.getDestinationId()) {
                        vetEdges.erase(vetEdges.begin() + i);
                    }
                    if (vetEdges.at(i).getSourceId() == edge.getDestinationId() && vetEdges.at(i).getDestinationId() == edge.getSourceId()) {
                        vetEdges.erase(vetEdges.begin() + i);
                    }
                }

                int u = findInSubtree(subtrees, edge.getSourceId());
                int v = findInSubtree(subtrees, edge.getDestinationId());

                if (u != v) {
                    GraphNode *nodeU = getNode(edge.getSourceId());
                    GraphNode *nodeV = getNode(edge.getDestinationId());
                    if (nodeU != nullptr && nodeV != nullptr) {
                        int gu = nodeU->getGroupId();
                        int gv = nodeV->getGroupId();

                        if ((subgroups[gu - 1] == nodeU->getId() || subgroups[gu - 1] == 0) && (subgroups[gv - 1] == nodeV->getId() || subgroups[gv - 1] == 0)) {
                            
                            resultado.push_back(edge);
                            sumWeight += edge.getWeight();
                            joinSubtrees(subtrees, u, v);

                            num--;
                            if (subgroups[gu - 1] == 0) {
                                subgroups[gu - 1] = nodeU->getId();
                            }
                            if (subgroups[gv - 1] == 0) {
                                subgroups[gv - 1] = nodeV->getId();
                            }
                        }
                    }
                }
            }

            output << "-> Execution " << (j + 1) << "\n\n";

            tFim = clock();
            tDecorrido = ((tFim - tInicio) / (CLOCKS_PER_SEC / 1000));

            output << "Time: " << tDecorrido << "ms\n";

            output << "Total Weight: " << sumWeight << "\n";
            float averageWeight = sumWeight / float(resultado.size());
            output << "Average Weight: " << averageWeight << "\n";

            output << "\n";

            // output << "graph RandomizedGreedy {\n";
            // for (int i = 0; i < resultado.size(); i++) {
            //     output << "\t" << resultado.at(i).getSourceId() << " -- " << resultado.at(i).getDestinationId() << " [label=" << resultado.at(i).getWeight() << "]\n";
            // }
            // output << "}\n";

        }
        
        output << "\n==============================\n";
    }
}