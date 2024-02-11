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

std::vector<Road> readGraphFromFile(const std::string& fileName) {
    std::ifstream file(fileName);
    int numNodes, numRoads;
    file >> numNodes >> numRoads;
    std::vector<Road> roads(numRoads);
    for (int i = 0; i < numRoads; i++) {
        file >> roads[i].from >> roads[i].to >> roads[i].width;
    }
    return roads;
}

int widestPath(std::vector<Road>& roads, int start, int end) {
    int n = roads.size();
    std::vector<std::vector<int>> graph(n, std::vector<int>(n, 0));

    for (const auto& road : roads) {
        graph[road.from][road.to] = road.width;
        graph[road.to][road.from] = road.width;
    }

    std::vector<int> max_width(n, INT_MIN);
    max_width[start] = INT_MAX;

    std::queue<int> q;
    q.push(start);

    // Array to keep track of the previous node in the path
    std::vector<int> previous(graph.size(), -1);

    // BFS method
    while (!q.empty()) {
        int curr = q.front();
        q.pop();

//        std::cout << curr << std::endl;


        for (int i = 0; i < n; i++) {
            int width = std::min(max_width[curr], graph[curr][i]);

            if (graph[curr][i] > 0 && width > max_width[i]) {
//                std::cout << width << " " << graph[curr][i] << std::endl;
                max_width[i] = width;
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
            int width = graph[previous[currNode]][currNode];
            std::cout << width << std::endl;
        }
        currNode = previous[currNode];
    }
    reverse(path.begin(), path.end());

    return max_width[end];
}

void getSolution(const std::string& fileName, int capitalId, int ZodangaId) {
    std::cout << "capitalId = " << capitalId << "; " << "ZodangaId = " << ZodangaId << std::endl;
    std::vector<Road> roads = readGraphFromFile(fileName);

    int max_width = widestPath(roads, capitalId, ZodangaId);
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