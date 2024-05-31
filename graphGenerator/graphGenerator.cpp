#include <iostream>
#include <vector>
#include <cstdlib>  
#include <ctime>   
using namespace std;

// Function to generate a random graph with up to 10 nodes
vector<vector<int>> generateRandomGraph(int numNodes) {
    if (numNodes > 10) {
        numNodes = 10;  // Limit the number of nodes to 10
    }

    vector<vector<int>> graph(numNodes, vector<int>(numNodes));

    // Seed the random number generator
    srand(time(0));

    for (int i = 0; i < numNodes; ++i) {
        for (int j = i + 1; j < numNodes; ++j) {
            int weight = rand() % 100 + 1;  // Random weight between 1 and 100
            graph[i][j] = weight;
            graph[j][i] = weight;
        }
    }

    // Set diagonal to 0 (no self-loops)
    for (int i = 0; i < numNodes; ++i) {
        graph[i][i] = 0;
    }

    return graph;
}

int main() {
    int numNodes;
    cout << "Enter the number of nodes (up to 10): ";
    cin >> numNodes;

    vector<vector<int>> graph = generateRandomGraph(numNodes);

    cout << "Generated graph adjacency matrix:" << endl;
    for (int i = 0; i < graph.size(); ++i) {
        cout << "{";
        for (int j = 0; j < graph[i].size(); ++j) {
            cout << graph[i][j];
            if (j < graph[i].size() - 1) {
                cout << ", ";
            }
        }
        cout << "}";
        if (i < graph.size() - 1) {
            cout << ",";  // Add a comma after each row except the last one
        }
        cout << endl;
    }

    return 0;
}
