#include <iostream>
#include <fstream>
#include <vector>

void generateGraph(int numNodes, int numEdges, const std::string& filename) {
    std::cout << numNodes << " " << numEdges;

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
        generateGraph(numCities, numRoads, fileName);
    }

    return 0;
}