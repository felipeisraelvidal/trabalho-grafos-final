#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <string>
#include <vector>
#include "Graph.h"
#include "Hash.h"

void startReadingTxt(Graph *graph, std::ifstream &input_file) {
    int order;
    int idNodeSource;
    int idNodeTarget;

    input_file >> order;

    if (!graph->isWeightedEdges() && !graph->isWeightedNodes()) {
        while (input_file >> idNodeSource >> idNodeTarget) {
            graph->insertEdge(idNodeSource, idNodeTarget, 0);
        }
    } else if (graph->isWeightedEdges() && !graph->isWeightedNodes()) {
        float edgeWeight;

        while (input_file >> idNodeSource >> idNodeTarget >> edgeWeight) {
            graph->insertEdge(idNodeSource, idNodeTarget, edgeWeight);
        }
    } else if (graph->isWeightedNodes() && !graph->isWeightedEdges()) {
        float nodeSourceWeight, nodeTargetWeight;

        while (input_file >> idNodeSource >> nodeSourceWeight >> idNodeTarget >> nodeTargetWeight) {
            graph->insertEdge(idNodeSource, idNodeTarget, 0);
            graph->getNode(idNodeSource)->setWeight(nodeSourceWeight);
            graph->getNode(idNodeTarget)->setWeight(nodeTargetWeight);
        }
    } else if (graph->isWeightedNodes() && graph->isWeightedEdges()) {
        float nodeSourceWeight, nodeTargetWeight, edgeWeight;

        while (input_file >> idNodeSource >> nodeSourceWeight >> idNodeTarget >> nodeTargetWeight) {
            graph->insertEdge(idNodeSource, idNodeTarget, edgeWeight);
            graph->getNode(idNodeSource)->setWeight(nodeSourceWeight);
            graph->getNode(idNodeTarget)->setWeight(nodeTargetWeight);
        }
    }
}

void startReadingOtherFileExtension(Graph *graph, std::string &input_file_name, std::ifstream &input_file) {
    int numberOfGroups;
    int numberOfNodes;

    std::cout << "Enter the number of groups: ";
    std::cin >> numberOfGroups;

    std::cout << "Enter the number of nodes: ";
    std::cin >> numberOfNodes;

    graph->setNumberOfGroups(numberOfGroups);

    int groupId;
    for (int i = 0; i < numberOfNodes; i++) {
        input_file >> groupId;
        std::cout << "Node: " << i << " - group: " << groupId << "\n";
        graph->insertNode(i, 0, groupId);
    }

    int nodeId;
    int targetId;
    float edgeWeight;

    while (input_file >> nodeId >> targetId >> edgeWeight) {
        graph->insertEdge(nodeId, targetId, edgeWeight);
    }
}

void leitura(Graph *graph, std::string &input_file_name, std::ifstream &input_file) {

    // Get file extension
    std::string fileExtension = input_file_name.substr(input_file_name.find_first_of(".") + 1);
    std::cout << input_file_name << "\n";
    std::cout << fileExtension << "\n";

    if (fileExtension == "txt") {
        startReadingTxt(graph, input_file);
    } else {
        startReadingOtherFileExtension(graph, input_file_name, input_file);
    }
}

int menu() {
    int selection;

    std::cout << "MENU\n";
    std::cout << "-------------------------------------------------\n";
    std::cout << "[1] Subgrafo induzido por conjunto de vértices\n";
    std::cout << "[2] Caminho Mínimo entre dois vértices - Dijkstra\n";
    std::cout << "[3] Caminho Mínimo entre dois vértices - Floyd\n";
    std::cout << "[4] Árvore Geradora Mínima de Kruskal\n";
    std::cout << "[5] Árvore Geradora Mínima de Prim\n";
    std::cout << "[6] Imprimir caminhamento em largura\n";
    std::cout << "[7] Imprimir ordenacao topológica\n";
    std::cout << "[8] Algoritmo Guloso\n";
    std::cout << "[9] Algoritmo Guloso Randomizado\n";
    std::cout << "[10] Algoritmo Guloso Randomizado Reativo\n";
    std::cout << "[0] Sair\n";

    std::cin >> selection;

    return selection;
}

void selectOption(Graph *graph, int selection, std::ofstream &output_file) {
    switch (selection) {
    case 1: {
        std::cout << "Subgrafo induzido por conjunto de vértices\n";
        graph->inducedSubgraph(output_file);
        break;
    }
    case 2: {
        std::cout << "Caminho Mínimo entre dois vértices - Dijkstra\n";
        graph->dijkstra(output_file);
        break;
    }
    case 3: {
        std::cout << "Caminho Mínimo entre dois vértices - Floyd\n";
        // graph->floyd();
        break;
    }
    case 4: {
        std::cout << "Árvore Geradora Mínima de Kruskal\n";
        graph->kruskal(output_file);
        break;
    }
    case 5: {
        std::cout << "Árvore Geradora Mínima de Prim\n";
        graph->prim(output_file);
        break;
    }
    case 6: {
        graph->bfs(output_file);
        break;
    }
    case 7: {
        std::cout << "Imprimir ordenacao topológica\n";
        graph->topologicalSort(output_file);
        break;
    }
    case 8: {
        std::cout << "Algoritmo Guloso\n";
        graph->greedy();
        break;
    }
    case 9: {
        std::cout << "Algoritmo Guloso Randomizado\n";
        graph->randomizedGreedy();
        break;
    }
    case 10: {
        std::cout << "Algoritmo Guloso Randomizado Reativo\n";
        break;
    }
    default:
        break;
    }
}

void mainMenu(Graph *graph, std::ofstream &output_file) {
    int selection = 1;
    while (selection != 0) {
        // system("clear");
        selection = menu();

        if (output_file.is_open()) {
            selectOption(graph, selection, output_file);
        } else {
            std::cout << "Unable to open the output file.\n";
        }

        output_file << "\n";
    }
}

int main(int argc, char const *argv[]) {

    // Graph graph(false, false, false);
    // graph.insertEdge(1, 2, 0);
    // graph.insertEdge(1, 4, 0);
    // graph.insertEdge(2, 3, 0);

    // graph.print();

    // std::cout << "Number of nodes: " << graph.getOrder() << "\n";
    // std::cout << "Number of edges: " << graph.numberOfEdges() << "\n";

    // int target = 1;
    // std::cout << "Node " << target << ":\n";
    // std::cout << "Degree: " << graph.getNode(target)->getDegree() << "\n";
    // std::cout << "In degree: " << graph.getNode(target)->getInDegree() << "\n";
    // std::cout << "Out degree: " << graph.getNode(target)->getOutDegree() << "\n";

    // Hash hash(5);
    // hash.insert(10, 5);
    // hash.insert(24, 4);
    // hash.insert(1, 25);
    // hash.insert(36, 2);
    // hash.insert(42, 30);

    // hash.print();

    // int result = hash.get(42);
    // std::cout << result << "\n";

    if (argc != 6) {
        std::cout << "ERROR: Expecting: ./<program_name> <input_file> <output_file> <directed> <weighted_edge> <weighted_node>\n";
        return 1;
    }

    std::string program_name(argv[0]);
    std::string input_file_name(argv[1]);

    std::string instance;
    if (input_file_name.find("v") <= input_file_name.size()) {
        std::string instance = input_file_name.substr(input_file_name.find("v"));
        std::cout << "Running" << program_name << " with instance" << instance << "...\n";
    }

    //Abrindo arquivo de entrada
    std::ifstream input_file;
    std::ofstream output_file;
    input_file.open(argv[1], std::ios::in);
    output_file.open(argv[2], std::ios::out | std::ios::trunc);

    int isDirected = atoi(argv[3]);
    int isWeightedEdge = atoi(argv[4]);
    int isWeightedNode = atoi(argv[5]);

    Graph graph(isDirected, isWeightedEdge, isWeightedNode);

    if (input_file.is_open()) {
        leitura(&graph, input_file_name, input_file);
        
        graph.print();
        std::cout << "Order: " << graph.getOrder() << "\n";

        mainMenu(&graph, output_file);
    } else {
        std::cout << "Unable to open " << argv[1];
    }

    // Closing the input file
    input_file.close();

    // Closing the output file
    output_file.close();

    return 0;
}