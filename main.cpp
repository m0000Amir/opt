#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <random>

const int MIN_ROAD_WIDTH = 1;
const int MAX_ROAD_WIDTH = 100;

struct Road {
    int from;
    int to;
    int width;
};

struct Graph {
    std::vector<Road> edges;
    int numNodes;
    int numEdges;
    int minWidth;
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

    // Add remaining edges
    for (int i = 0; i < numEdges - (numNodes - 1); i++) {
        int from = nodes[i % numNodes];
        int to = nodes[(i + 1) % numNodes];
        int weight = weightDist(gen);
        file << from << " " << to << " " << weight << std::endl;
    }
    file.close();
}

Graph readGraphFromFile(const std::string& fileName) {
    Graph graph;
    std::ifstream file(fileName);
//    int numNodes, numRoads;
    int minWidth = INT_MAX;
    file >> graph.numNodes >> graph.numEdges;
    std::vector<Road> edges(graph.numEdges);
    for (int i = 0; i < graph.numEdges; i++) {
        file >> edges[i].from >> edges[i].to >> edges[i].width;
        minWidth = std::min(minWidth, edges[i].width);
    }
    graph.edges = edges;
    graph.minWidth = minWidth;
    std::cout << "minWidth = " <<graph.minWidth << std::endl;
    return graph;
}

std::vector<std::vector<int>> getAdjacencyMatrix(Graph& graph) {
    std::vector<std::vector<int>> adjacency(
            graph.numNodes,
            std::vector<int>(graph.numNodes, 0));

    for (const auto& edge : graph.edges) {
        if (edge.width > adjacency[edge.from][edge.to] && edge.width > graph.minWidth) {
            adjacency[edge.from][edge.to] = edge.width;
            adjacency[edge.to][edge.from] = edge.width;
        }
    }
    return adjacency;
}

int widestPath(Graph& graph, int start, int end) {
    std::vector<Road> roads = graph.edges;
    std::vector<std::vector<int>> adjacency = getAdjacencyMatrix(graph);

    std::vector<int> maxWidth(graph.numNodes, INT_MIN);
    maxWidth[start] = INT_MAX;

    std::queue<int> q;
    q.push(start);

    // Array to keep track of the previous node in the path
    std::vector<int> previous(adjacency.size(), -1);

    // BFS method
    while (!q.empty()) {
        int curr = q.front();
        q.pop();

//        std::cout << curr << std::endl;

        for (int i = 0; i < graph.numNodes; i++) {
            int width = std::min(maxWidth[curr], adjacency[curr][i]);

            if (adjacency[curr][i] > 0 && width > maxWidth[i]) {
//                std::cout << width << " " << graph[curr][i] << std::endl;
                maxWidth[i] = width;
                previous[i] = curr;
                q.push(i);
            }
        }
    }

    // Reconstruct the widest path
    std::vector<int> path;
    int currNode = end;
    while (currNode != -1) {
        path.push_back(currNode);
        if (previous[currNode] != -1) {
            int width = adjacency[previous[currNode]][currNode];
            std::cout << width << std::endl;
        }
        currNode = previous[currNode];
    }
    reverse(path.begin(), path.end());

    return maxWidth[end];
}

void getSolution(const std::string& fileName, int capitalId, int ZodangaId) {
    std::cout << "capitalId = " << capitalId << "; " << "ZodangaId = " << ZodangaId << std::endl;
    Graph graph = readGraphFromFile(fileName);

    int max_width = widestPath(graph, capitalId, ZodangaId);
    std::cout << "max_width = " << max_width << std::endl;
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