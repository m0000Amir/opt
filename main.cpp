#include <iostream>
#include <fstream>
#include <vector>
#include <random>

const int MIN_ROAD_WIDTH = 1;
const int MAX_ROAD_WIDTH = 100;

struct Road {
    int from;
    int to;
    int width;
};

void generateConnectedGraph(int numNodes, int numEdges, const std::string& fileName) {
    if (numEdges < numNodes - 1 || numEdges > numNodes * (numNodes - 1) / 2) {
        std::cout << "Invalid number of edges for a connected graph." << std::endl;
        return ;
    }

    std::ofstream file(fileName);
    file << numNodes << " " << numEdges << std::endl;

    // prepare randomizers for edges and weights
    std::random_device rd;
    std::mt19937  gen(rd());
    std::uniform_int_distribution<> weightDist(MIN_ROAD_WIDTH,MAX_ROAD_WIDTH);

    // shuffle all nodes to get random connected graph
    std::vector<int> nodes(numNodes);
    std::iota(nodes.begin(), nodes.end(), 0);
    std::shuffle(nodes.begin(), nodes.end(), gen);

    // connected graph has `numNodes - 1` nodes
    for (int i = 0; i < numNodes - 1; i++) {
        int from = nodes[i];
        int to = nodes[i + 1];
        int weight = weightDist(gen);
        file << from << " " << to << " " << weight << std::endl;
        std::cout << from << "--" << to << std::endl;
    }

    // add remaining edges
    for (int i = 0; i < numEdges - (numNodes - 1); i++) {
        int from = nodes[i % numNodes];
        int to = nodes[(i + 1) % numNodes];
        int weight = weightDist(gen);
        file << from << " " << to << " " << weight << std::endl;
    }
    file.close();
}

std::vector<Road> readGraphFromFile(const std::string& fileName) {
    std::ifstream file(fileName);
    int numNodes, numRoads;
    file >> numNodes >> numRoads;
    std::vector<Road> graph(numRoads);
    for (int i = 0; i < numRoads; i++) {
        file >> graph[i].from >> graph[i].to >> graph[i].width;
    }
    return graph;
}

void getSolution(const std::string& fileName, int capitalId, int ZodangaId) {
    std::cout << "capitalId = " << capitalId << "; " << "ZodangaId = " << ZodangaId;
    std::vector<Road> graph = readGraphFromFile(fileName);

    int a = 1;
}

int main(int argc, char* argv[]) {
    if (argc < 5) {
        std::cerr << "Insufficient arguments" << std::endl;
        return 1;
    }

    std::string command = argv[1];
    if (command == "-g") {
        int numCities = std::stoi(argv[2]);
        int numRoads = std::stoi(argv[3]);
        std::string fileName = argv[4];
        generateConnectedGraph(numCities, numRoads, fileName);
    } else if (command == "-t") {
        int capitalId = std::stoi(argv[2]);
        int ZodangaId = std::stoi(argv[3]);
        std::string fileName = argv[4];
        getSolution(fileName, capitalId, ZodangaId);
    } else {
        std::cerr << "Invalid command" << std::endl;
        return 1;
    }

    return 0;
}