#include "WeightedGraph.hpp"
#include <iostream>
#include <queue>
#include "MinHeap.hpp"
#include <climits>
#include <algorithm>
#include <algorithm>

template <typename T>
void WeightedGraph<T>::insertVertex(const T& v) {
    if (getVertexIndex(v) != -1) {
        return;
    }

    vertices.push_back(v);
    edges.push_back(std::vector<Edge>());
    states.push_back("");
}

template <typename T>
void WeightedGraph<T>::insertEdge(const T& v1, const T& v2, int distance, int cost, bool direct) {
    int i1 = getVertexIndex(v1);
    int i2 = getVertexIndex(v2);

    if (i1 == -1 || i2 == -1) {
        std::cout << "insertEdge: incorrect vertices\n";
        return;
    }

    if (!hasEdge(i1, i2)) {
        edges[i1].push_back(Edge(i2, distance, cost, distance));

        if (!direct && i1 != i2) {
            edges[i2].push_back(Edge(i1, distance, cost, distance));
        }
    }
}   

template <typename T>
int WeightedGraph<T>::getVertexIndex(const T& ver) const {
    for(int i = 0; i < vertices.size(); i++) {
        if (vertices[i] == ver) {
            return i;
        }
    }

    return -1;
}


template <typename T>
void WeightedGraph<T>::print() const {
    for (int i = 0; i < vertices.size(); i++) {
        std::cout << "{ " << vertices[i] << ": ";
        for(int j = 0; j < edges[i].size(); j++) {
            std::cout << "(" << vertices[edges[i][j].neighbor] << ", dist=" << edges[i][j].distance << ", cost=" << edges[i][j].cost << ")";
        }
        std::cout << " }\n";
    }
}


template <typename T>
bool WeightedGraph<T>::hasEdge(int i1, int i2) const {
    if (i1 < 0 || i1 >= edges.size()) {
        return false;
    }

    for (const Edge& e : edges[i1]) {
        if (e.neighbor == i2) {
            return true;
        }
    }

    return false;
}

template <typename T>
void WeightedGraph<T>::DFS() const {
    if (vertices.empty()) {
        return;
    }
    std::vector<bool> visited(vertices.size(), false);
    DFS(0, visited);
}


template <typename T>
void WeightedGraph<T>::DFS(int i, std::vector<bool>& visited) const {
    visited[i] = true;
    std::cout << vertices[i] << " -> ";

    // Look through all the neighbours
    for (const Edge& e : edges[i]) {
        if (!visited[e.neighbor]) {
            DFS(e.neighbor, visited);
        }
    }
}

template <typename T>
void WeightedGraph<T>::BFS(int start) const {
    if (vertices.empty() || start < 0 || start >= vertices.size()) {
        return;
    }

    std::vector<bool> discovered(vertices.size(), false);
    std::queue<int> where_to_go;

    where_to_go.push(start);
    discovered[start] = true;

    while (!where_to_go.empty()) {
        int cur = where_to_go.front();
        std::cout << vertices[cur];
        where_to_go.pop();

        // Explore the neighbors
        for (const Edge& e : edges[cur]) {
            if (!discovered[e.neighbor]) {
                where_to_go.push(e.neighbor);
                discovered[e.neighbor] = true;
            }
        }

    }   

}

template <typename T>
int WeightedGraph<T>::shortestPath(const T& src, const T& dest, bool print) const {
    int i_src = getVertexIndex(src);
    int i_dest = getVertexIndex(dest);

    if (i_src == -1 || i_dest == -1) {
        if (print) {
            std::cout << "Shortest route from " << src << " to " << dest << ": None\n";
        }
        return -1;
    }

    std::vector<bool> visited(vertices.size(), false);
    std::vector<int> prev(vertices.size(), -1);
    std::vector<int> distances(vertices.size(), INT_MAX);
    std::vector<int> cost(vertices.size(), INT_MAX);

    distances[i_src] = 0;
    cost[i_src] = 0;

    MinHeap<Edge> heap;
    heap.insert(Edge(i_src, 0, 0, 0));

    while (!heap.empty()) {
        Edge smallest = heap.deleteMin();
        int cur = smallest.neighbor;

        if (visited[cur]) continue;
        visited[cur] = true;

        for (const Edge& e : edges[cur]) {
            int next = e.neighbor;

            if (!visited[next] && distances[cur] + e.distance < distances[next]) {
                distances[next] = distances[cur] + e.distance;
                cost[next] = cost[cur] + e.cost;
                prev[next] = cur;

                heap.insert(Edge(next, e.distance, e.cost, distances[next]));
            }
        }
    }

    if (distances[i_dest] == INT_MAX) {
        if (print) {
            std::cout << "Shortest route from " << src << " to " << dest << ": None\n";
        }
        return -1;
    }

    if (print) {
        std::vector<int> path;

        for (int at = i_dest; at != -1; at = prev[at]) {
            path.push_back(at);
        }

        std::cout << "Shortest route from " << src << " to " << dest << ": ";

        for (int i = path.size() - 1; i >= 0; i--) {
            std::cout << vertices[path[i]];
            if (i != 0) {
                std::cout << " -> ";
            }
        }

        std::cout << ". The length is " << distances[i_dest]
                  << ". The cost is " << cost[i_dest] << ".\n";
    }

    return distances[i_dest];
}

// Task 8
template <typename T>
void WeightedGraph<T>::kruskal() {
    // Create a structure to store edges in u (source), v (destination), cost
    struct KEdge {
        int u, v, cost;
    };

    // Collect all edges and store them into a vector and process later
    std::vector<KEdge> allEdges;
    for (int i = 0; i < vertices.size(); i++) {
        for (const Edge& e : edges[i]) {
            if (i < e.neighbor) {
                allEdges.push_back({i, e.neighbor, e.cost});
            }
        }
    }
    // initialize the total cost and number of vertices
    int total = 0;
    int n = vertices.size();

    // initialize the parent and rank vector for the disjoint set and use of tree property later
    std::vector<int> parent(n);
    std::vector<int> rank(n, 0);
    // set vertices to be their own parents that separate into sets
    for (int i = 0; i < n; i++) parent[i] = i;
    // Sort edges by cost from the least to most
    for (int i = 0; i < allEdges.size() - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < allEdges.size(); j++) {
            if (allEdges[j].cost < allEdges[minIdx].cost) {
                minIdx = j;
            }
        }
        std::swap(allEdges[i], allEdges[minIdx]);
    }

    std::cout << "\nMinimum Spanning Tree:\n";
    std::cout << "Edge\t\tWeight\n";
    // run through the edges to link
    for (const auto& e : allEdges) {
        int rootU = findSet(e.u, parent);
        int rootV = findSet(e.v, parent);

        if (rootU != rootV) {
            // unite sets
            unionSet(rootU, rootV, parent, rank);

            std::cout << vertices[e.u] << " - " << vertices[e.v] << "\t" << e.cost << "\n";

            total += e.cost;
        }
    }
    std::cout << "Total cost of MST: " << total << "\n";
}

// Helper function for Kruskal
template <typename T>
int WeightedGraph<T>::findSet(int x, std::vector<int>& parent) {
    // use recursive functionality to find the root
    if (parent[x] != x) parent[x] = findSet(parent[x], parent);
    return parent[x];
}

// Helper function for Kruskal
template <typename T>
void WeightedGraph<T>::unionSet(int a, int b, std::vector<int>& parent, std::vector<int>& rank) {
    // retrieve the root for each set
    int rootA = findSet(a, parent);
    int rootB = findSet(b, parent);

    // If rootA and rootB are equal, then its the same set and will create a cycle. Therefore, rootA != rootB
    if (rootA != rootB) {
        // use tree property to unite the sets
        if (rank[rootA] < rank[rootB]) parent[rootA] = rootB;
        else if (rank[rootA] > rank[rootB]) parent[rootB] = rootA;
        else {
            parent[rootB] = rootA;
            rank[rootA]++;
        }
    }
}

// Task 7:
template <typename T>
void WeightedGraph<T>::prim() const
{
    int n = vertices.size();
    if(n == 0) {
        std::cout << "Graph is empty. Cannot form MST.\n";
        return;
    }
     // inMST tracks what is already in the MST, key is the min cost to connect each vertex to MST
    std::vector<bool> inMST(n, false);
    std::vector<int> key(n, INT_MAX);
    std::vector<int> parent(n, -1);
   
    key[0] = 0;

  
    MinHeap<Edge> heap;
    heap.insert(Edge(0, 0, 0, 0));

    int verticesAdded = 0;

    while (!heap.empty()) {
        Edge minEdge = heap.deleteMin();
        int u = minEdge.neighbor;

        if (inMST[u]) continue;

        inMST[u] = true;
        verticesAdded++;

        for (const Edge& e : edges[u]) {
            int v = e.neighbor;

            if (!inMST[v] && e.cost < key[v]){
                key[v] = e.cost;
                parent[v] = u;
                heap.insert(Edge(v, 0, e.cost, e.cost));
            }
        }
    }

    // The graph is disconnected if we didnt reach all of the verticies
    if (verticesAdded< n) {
        std::cout << "\nThe graph is disconnected. A Minimum Spanning Tree cannot be formed.\n";
        return;
    }

    // THis prints MST edges and total cost
    int totalCost = 0;
    std::cout << "\nMinimum Spanning Tree (Prim's Algorithm):\n";
    std::cout << "Edge\t\tWeight\n";

    for (int i = 1; i < n; i++) {
        if (parent[i] != -1) {
            std::cout << vertices[parent[i]] << " - " << vertices[i]
                      << "\t\t" << key[i] << "\n";
            totalCost += key[i];
        }
    }

    std::cout << "Total Cost of MST: " << totalCost << "\n";
}



// Task 6:
template <typename T>
WeightedGraph<T> WeightedGraph<T>::buildUndirectedGraph() const
{
    WeightedGraph<T> undirected;
    int n = vertices.size();

    // This copies all vertices into the new graph
    for(int i = 0; i < n; i++) {
        undirected.insertVertex(vertices[i]);
    }

    // This is Pass 1 which cheacks for each directed edge u -> v where u < v,
    for (int u = 0; u < n; u++) {
        for (const Edge& e : edges[u]) {
            int v = e.neighbor;

            // This line stops a pair from being proccessed twice
            if (u >= v) continue;

            int costUV = e.cost;

            // Check if the reverse edge exists
            int costVU = -1;
            for (const Edge& rev : edges[v]) {
                if (rev.neighbor == u) {
                    costVU = rev.cost;
                    break;
                }
            }

            // If both directions exist pick the min cost
            int minCost = costUV;
            if (costVU != -1 && costVU < minCost) {
                minCost = costVU;
            }

            undirected.insertEdge(vertices[u], vertices[v], 0, minCost, false);
        }
    }

    // This is Pass 2 which handles edges v -> u where v > u but u -> v does NOT exist
   
    for (int v=0; v < n; v++) {
        for (const Edge& e : edges[v]) {
            int u = e.neighbor;

            if (v<= u) continue;

            bool forwardExists = false;
            for (const Edge& fwd : edges[u]) {
                if (fwd.neighbor == v) {
                    forwardExists = true;
                    break;
                }
            }

            if (!forwardExists) {
                undirected.insertEdge(vertices[u], vertices[v], 0, e.cost, false);
            }
        }
    }

    std::cout<< "\nUndirected graph created from directed graph.\n";
    std::cout << "Number of vertices: " << undirected.vertices.size() << "\n";

    // This counts undirected edges 
    int edgeCount = 0;
    for (int i = 0; i < (int)undirected.edges.size(); i++){
        edgeCount += undirected.edges[i].size();
    }
    std::cout << "Number of undirected edges: " << edgeCount / 2 << "\n";

    return undirected;
}

//
template <typename T>
void WeightedGraph<T>::setState(const T& airport, const std::string& state) {
    int index = getVertexIndex(airport);
    if (index != -1) {
        states[index] = state;
    }
}

//Task 3
template <typename T>
void WeightedGraph<T>::shortPathToState(const T& src, const std::string& destState) const {
    int i_src = getVertexIndex(src);

    if (i_src == -1) {
        std::cout << "Origin airport " << src << " does not exist.\n";
        return;
    }

    bool foundAirport = false;
    bool foundPath = false;

    for (int i = 0; i < vertices.size(); i++) {
        if (states[i] == destState) {
            foundAirport = true;

            int d = shortestPath(src, vertices[i], false); // EDIT
            if (d != -1) {
                shortestPath(src, vertices[i], true); // EDIT
                foundPath = true;
            }
        }
    }

    if (!foundAirport) {
        std::cout << "No airports found in state " << destState << ".\n";
    }
    else if (!foundPath) {
        std::cout << "No paths exist from " << src << " to state " << destState << ".\n";
    }
}

template <typename T>
void WeightedGraph<T>::shortPathWithStopHelper(
    int cur,
    int dest,
    int edgesLeft,
    std::vector<bool>& visited,
    std::vector<int>& path,
    int currentDistance,
    int currentCost,
    int& bestDistance,
    int& bestCost,
    std::vector<int>& bestPath
) const {
    if (edgesLeft == 0) {
        if (cur == dest && currentDistance < bestDistance) {
            bestDistance = currentDistance;
            bestCost = currentCost;
            bestPath = path;
        }
        return;
    }

    for (const Edge& e : edges[cur]) {
        int next = e.neighbor;

        if (!visited[next]) {
            visited[next] = true;
            path.push_back(next);

            shortPathWithStopHelper(
                next, dest, edgesLeft - 1,
                visited, path,
                currentDistance + e.distance,
                currentCost + e.cost,
                bestDistance, bestCost, bestPath
            );

            path.pop_back();
            visited[next] = false;
        }
    }
}

//Task 4
template <typename T>
void WeightedGraph<T>::shortPathWithStop(const T& src, const T& dest, int stops) const {
    int i_src = getVertexIndex(src);
    int i_dest = getVertexIndex(dest);

    if (i_src == -1 || i_dest == -1) {
        std::cout << "Invalid airports.\n";
        return;
    }

    int edgesLeft = stops + 1;

    std::vector<bool> visited(vertices.size(), false);
    std::vector<int> path, bestPath;

    int bestDistance = INT_MAX;
    int bestCost = 0;

    visited[i_src] = true;
    path.push_back(i_src);

    shortPathWithStopHelper(
        i_src, i_dest, edgesLeft,
        visited, path, 0, 0,
        bestDistance, bestCost, bestPath
    );

    if (bestPath.empty()) {
        std::cout << "No path exists.\n";
        return;
    }

    std::cout << "Path: ";
    for (int i = 0; i < bestPath.size(); i++) {
        std::cout << vertices[bestPath[i]];
        if (i != bestPath.size() - 1) std::cout << " -> ";
    }

    std::cout << "\nDistance: " << bestDistance
              << " Cost: " << bestCost << "\n";
}

//Task 5
template <typename T>
void WeightedGraph<T>::airportConnect() const {
    struct Info {
        T airport;
        int in = 0;
        int out = 0;
        int total = 0;
    };

    std::vector<Info> list(vertices.size());

    for (int i = 0; i < vertices.size(); i++) {
        list[i].airport = vertices[i];
    }

    for (int i = 0; i < edges.size(); i++) {
        for (const Edge& e : edges[i]) {
            list[i].out++;
            list[e.neighbor].in++;
        }
    }

    for (auto& a : list) {
        a.total = a.in + a.out;
    }

    std::sort(list.begin(), list.end(), [](const Info& a, const Info& b) { 
        return a.total > b.total; 
    });

    for (const auto& a : list) {
        std::cout << a.airport
                  << " In:" << a.in
                  << " Out:" << a.out
                  << " Total:" << a.total << "\n";
    }
}