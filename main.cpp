#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <random>

const int MIN_ROAD_WIDTH = 10;
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

struct Path {
    int from;
    int to;
    std::vector<int> track;
    std::vector<int> minMaxWidth;
    int minWidth;
};

void generateConnectedGraph(int numNodes, int numEdges, const std::string& fileName) {
    if (numEdges < numNodes - 1 || numEdges > numNodes * (numNodes - 1) / 2) {
        std::cout << "Invalid number of edges for a connected graph." << std::endl;
        return ;
    }

    std::ofstream file(fileName);
    file << numNodes << " " << numEdges << std::endl;

    // Prepare randomizers for edges and weights
    std::random_device rd;
    std::mt19937  gen(rd());
    std::uniform_int_distribution<> weightDist(MIN_ROAD_WIDTH,MAX_ROAD_WIDTH);

    // Shuffle all nodes to get random connected graph
    std::vector<int> nodes(numNodes);
    std::iota(nodes.begin(), nodes.end(), 0);
    std::shuffle(nodes.begin(), nodes.end(), gen);

    // Connected graph has `numNodes - 1` nodes
    for (int i = 0; i < numNodes - 1; i++) {
        int from = nodes[i];
        int to = nodes[i + 1];
        int weight = weightDist(gen);
        file << from << " " << to << " " << weight << std::endl;
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
    int minWidth = INT_MAX;
    file >> graph.numNodes >> graph.numEdges;
    std::vector<Road> edges(graph.numEdges);
    for (int i = 0; i < graph.numEdges; i++) {
        file >> edges[i].from >> edges[i].to >> edges[i].width;
        minWidth = std::min(minWidth, edges[i].width);
    }
    graph.edges = edges;
    graph.minWidth = minWidth;
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

void reconstructWidestPath(Graph& graph, Path& path) {
    std::vector<std::vector<int>> adjacency = getAdjacencyMatrix(graph);
    std::vector<int> route;

    int currNode = path.to;
    while (currNode != -1) {
        route.push_back(currNode);
        if (path.track[currNode] != -1) {
            int width = adjacency[path.track[currNode]][currNode];
            std::cout << width << std::endl;
        }
        currNode = path.track[currNode];
    }
    reverse(route.begin(), route.end());

    std::cout << "(" << path.from << ") ";
    for (int i = 1; i < route.size(); i++) {
        std::cout << "-" << adjacency[route[i - 1]][route[i]] << "-";
        std::cout << " (" << route[i] << ") ";
    }
}

Path widestPath(Graph& graph, int start, int end) {
    std::vector<Road> roads = graph.edges;
    std::vector<std::vector<int>> adjacency = getAdjacencyMatrix(graph);

    std::vector<int> minMaxWidth(graph.numNodes, INT_MIN);
    minMaxWidth[start] = INT_MAX;

    std::queue<int> q;
    q.push(start);

    // Array to keep track of the previous node in the path
    std::vector<int> previous(adjacency.size(), -1);

    // BFS method to find path from start to end
    while (!q.empty()) {
        int curr = q.front();
        q.pop();

        for (int i = 0; i < graph.numNodes; i++) {
            int width = std::min(minMaxWidth[curr], adjacency[curr][i]);

            if (adjacency[curr][i] > 0 && width > minMaxWidth[i]) {
                minMaxWidth[i] = width;
                previous[i] = curr;
                q.push(i);
            }
        }
    }

    Path widestPath = {
            start,
            end,
            previous,
            minMaxWidth,
            minMaxWidth[end]
    };

    return widestPath;
}

void getSolution(const std::string& fileName, int capitalId, int ZodangaId, bool isReconstruct = false) {
    std::cout << "Capital Id is " << capitalId << "; " << "Zodanga Id is " << ZodangaId << std::endl;
    Graph graph = readGraphFromFile(fileName);
    std::cout << "Minimum of road width is " <<graph.minWidth << std::endl;

    Path path = widestPath(graph, capitalId, ZodangaId);
    if (path.minWidth != INT_MIN) {
        // The maximum feasible width is equal to the difference between
        // the minimum edge width of the widest path and the minimum width
        // of the entire graphs.
        std::vector<int> feasibleWidth = {1, path.minWidth - graph.minWidth};
        std::cout << "Feasible range of width is [" << feasibleWidth[0] << " : " << feasibleWidth[1] << "]" << std::endl;
    } else {
        std::cout << "There is not feasible width range." << "\n";
        std::cout << "Widest path consist of road, which equal to minimal width of road." << "\n";
        std::cout << "width `ra` will not be equal to width `rb`" << "\n";
    }
    if (isReconstruct) reconstructWidestPath(graph, path);
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