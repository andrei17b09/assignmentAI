#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>
#include <chrono> 

using namespace std;
using namespace std::chrono;

// Custom comparator for the priority queue
struct PathCost {
    vector<int> path;
    int cost;
    int heuristic;
    bool operator>(const PathCost& other) const {
        return (cost + heuristic) > (other.cost + other.heuristic);
    }
};

//function to calculate the MST cost using Prim's algorithm
int calculateMSTCost(const vector<vector<int>>& graph, const vector<int>& path) {
    int n = graph.size();
    vector<bool> visited(n, false);
    for (int node : path) {
        visited[node] = true;
    }

    // Find an unvisited node to start the MST
    int start = -1;
    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            start = i;
            break;
        }
    }

    if (start == -1) return 0; // No nodes to form an MST

    vector<int> minEdge(n, INT_MAX);
    minEdge[start] = 0;
    int mstCost = 0;

    for (int i = 0; i < n; ++i) {
        int u = -1;
        for (int v = 0; v < n; ++v) {
            if (!visited[v] && (u == -1 || minEdge[v] < minEdge[u])) {
                u = v;
            }
        }

        if (u == -1 || minEdge[u] == INT_MAX) break; // Unreachable node (disconnected graph)

        visited[u] = true;
        mstCost += minEdge[u];

        for (int v = 0; v < n; ++v) {
            if (!visited[v] && graph[u][v] < minEdge[v]) {
                minEdge[v] = graph[u][v];
            }
        }
    }

    return mstCost;
}

// A* Search function to find the minimum cost Hamiltonian cycle using MST heuristic
pair<int, vector<int>> astarTSP(const vector<vector<int>>& graph) {
    int n = graph.size();
    priority_queue<PathCost, vector<PathCost>, greater<PathCost>> pq;
    pq.push({ {0}, 0, calculateMSTCost(graph, {0}) });  // Start from the first node with cost 0

    int minCost = INT_MAX;
    vector<int> minPath;

    while (!pq.empty()) {
        PathCost current = pq.top();
        pq.pop();

        vector<int> path = current.path;
        int currentCost = current.cost;

        if (path.size() == n) {
            currentCost += graph[path.back()][path[0]];  // Complete the cycle
            if (currentCost < minCost) {
                minCost = currentCost;
                minPath = path;
            }
        }
        else {
            for (int i = 1; i < n; ++i) {  // Start nodes from 1 since 0 is the starting node
                if (find(path.begin(), path.end(), i) == path.end()) {
                    vector<int> newPath = path;
                    newPath.push_back(i);
                    int newCost = currentCost + graph[path.back()][i];
                    int heuristic = calculateMSTCost(graph, newPath);
                    pq.push({ newPath, newCost, heuristic });
                }
            }
        }
    }

    return { minCost, minPath };
}

int main() {
    vector<vector<int>> graph = {
        {0, 23, 48, 75, 40, 13, 78, 70, 36, 6},
        {23, 0, 80, 66, 10, 87, 14, 17, 16, 79},
        {48, 80, 0, 8, 78, 73, 67, 7, 58, 46},
        {75, 66, 8, 0, 22, 10, 82, 32, 95, 31},
        {40, 10, 78, 22, 0, 34, 58, 52, 50, 39},
        {13, 87, 73, 10, 34, 0, 87, 6, 89, 23},
        {78, 14, 67, 82, 58, 87, 0, 70, 50, 61},
        {70, 17, 7, 32, 52, 6, 70, 0, 5, 82},
        {36, 16, 58, 95, 50, 89, 50, 5, 0, 24},
        {6, 79, 46, 31, 39, 23, 61, 82, 24, 0}
    };

    auto start = high_resolution_clock::now();
    auto result = astarTSP(graph);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    int minCost = result.first;
    vector<int> minPath = result.second;

    cout << "The minimum cost is: " << minCost << endl;
    cout << "The path of the minimum cost is: ";
    for (int node : minPath) {
        cout << node << " ";
    }
    cout << minPath[0] << endl;  // To show the cycle returning to the start node
    cout << "Execution time: " << duration.count();

    return 0;
}
