#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <chrono>

using namespace std;
using namespace std::chrono;

struct PathCost {
    vector<int> path;
    int cost;
    bool operator>(const PathCost& other) const {
        return cost > other.cost;
    }
};

// Uniform Cost Search function to find the minimum cost Hamiltonian cycle
pair<int,vector<int>> ucs(const vector<vector<int>>& graph) {
    int n = graph.size();
    priority_queue<PathCost, vector<PathCost>, greater<PathCost>> pq;
    pq.push({ {0}, 0 });  // Start from the first node with cost 0

    int minCost = INT_MAX;
    static vector<int> minPath;

    while (!pq.empty()) {
        PathCost current = pq.top();
        pq.pop();

        vector<int> path = current.path;
        int currentCost = current.cost;

        if (path.size() == n) {
            currentCost += graph[path.back()][path[0]];
            path.push_back(path[0]);// Complete the cycle
            if (currentCost < minCost)
            {
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
                    pq.push({ newPath, newCost });
                }
            }
        }
    }

    return { minCost,minPath };
}

pair<int,vector<int>> bfs(const vector<vector<int>>& graph) {
    int n = graph.size();
    vector<int> nodes(n);
    static vector<int> minPath;
    for (int i = 0; i < n; ++i) {
        nodes[i] = i;
    }

    queue<vector<int>> q;
    q.push({ 0 });

    int minCost = INT_MAX;

    while (!q.empty())
    {
        vector<int> path = q.front();
        q.pop();
        if (path.size() == n)
        {
            int costPath= 0;
            for (int i = 0; i < path.size()-1; i++)
            {
                costPath += graph[path[i]][path[i + 1]];
            }
            costPath += graph[path.back()][path[0]];
            path.push_back(path[0]);
            if (costPath < minCost)
            {
                minCost = costPath;
                minPath = path;
            }
        }
        else
        {
            for(int i=0;i<n;i++)
                if (find(path.begin(), path.end(), nodes[i]) == path.end())
                {
                    vector<int> newPath = path;
                    newPath.push_back(nodes[i]);
                    q.push(newPath);
                }
        }
    }
    return { minCost,minPath };
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

    pair<int, vector<int>> thebfs, theucs;
    auto start = high_resolution_clock::now();
    thebfs = bfs(graph);
    // Stop measuring time
    auto stop = high_resolution_clock::now();
    auto duration_a = duration_cast<milliseconds>(stop - start);

    start = high_resolution_clock::now();
    theucs = ucs(graph);
    stop = high_resolution_clock::now();
    auto duration_b = duration_cast<milliseconds>(stop - start);

    cout << "The minimum cost is " << thebfs.first << " , the execution time is " << duration_a.count() << " and the path is : ";
    for (auto i : thebfs.second)
        cout << i << " ";
    cout << " (for bfs)" << endl << "The minimum cost is " << theucs.first << " , the execution time is " << duration_b.count() << " and the path is : ";
    for (auto i : theucs.second)
        cout << i << " ";
    cout<<" (for ucs)\n";

    return 0;
}
