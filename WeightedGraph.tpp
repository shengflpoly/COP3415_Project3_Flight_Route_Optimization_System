#include "WeightedGraph.hpp"
#include <iostream>
#include <queue>
#include "MinHeap.hpp"
#include <climits>


template <typename T>
void WeightedGraph<T>::insertVertex(const T& v) {
    if (getVertexIndex(v) != -1) {
        //std::cout << "insertVertex: vertex already exist\n";
        return;
    }

    vertices.push_back(v);
    std::vector<Edge> tmp; 
    edges.push_back(tmp); //insert empty vector to the edges
}

// TODO
template <typename T>
void WeightedGraph<T>::insertEdge(const T& v1, const T& v2, int distance, int cost, bool direct) {
    int i1 = getVertexIndex(v1);
    int i2 = getVertexIndex(v2);
    if (i1 == -1 || i2 == -1) {
        std::cout << "insertEdge: incorrect vertices\n";
        return;
    }

    if (!hasEdge(i1, i2)) {
        // directed graph
        edges[i1].push_back(Edge(i2, distance, cost));
        if (!direct && i1 != i2) {
            edges[i2].push_back(Edge(i1, distance, cost));
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

// TODO
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

// TODO
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

// TODO
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

// TODO
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

// TODO
template <typename T>
int WeightedGraph<T>::shortestPath(const T& src, const T& dest) const {
    // Find indices
    int i_src = getVertexIndex(src);
    int i_dest = getVertexIndex(dest);
    std::vector<bool> visited(vertices.size(), false);
    std::vector<int> prev(vertices.size(), -1);

    // Check edge case
    if (i_src == -1 || i_dest == -1) {
        std::cout << "shortest route from " << src << " to " << dest << ": None\n";
        return -1;
    }
    if (i_src == i_dest) {
        return 0;
    }
    
    // Create distances vector
    std::vector<int> distances(vertices.size(), INT_MAX); // distances from source to all other nodes
    std::vector<int> cost(vertices.size(), INT_MAX);
    // Set initial distances
    distances[i_src] = 0;
    cost[i_src] = 0;

    // Perform BFS and update distances
    MinHeap<Edge> heap;
    heap.insert(Edge(i_src, 0, 0, 0));

    while (!heap.empty()) {
        Edge smallest = heap.deleteMin();
        int cur = smallest.neighbor;
        
        if (visited[cur]) continue;
        visited[cur] = true;
        //if (cur == i_dest) return distances[cur];

        // Check the distance (if the distance is smaller - update the distance)
        // Insert the edge into the heap
        for (const Edge& e : edges[cur]) {
            int visit = e.neighbor;
            //int weight = control ? e.distance : e.cost; //control, e.distance. !control, e.cost
            int newDist = distances[cur] + e.distance;
            int newCost = cost[cur] + e.cost;
            if (!visited[visit] && newDist < distances[visit]) {
                distances[visit] = newDist;
                cost[visit] = newCost;
                prev[visit] = cur;
                heap.insert(Edge(visit, e.distance, e.cost, distances[visit]));
            }
        }
    }
    std::vector<int> path;
    std::cout << "Shortest route from " << src << " to " << dest << ": ";
    for (int at = i_dest; at != -1; at = prev[at]) {
        path.push_back(at);
    }
    for (int i = path.size() - 1; i >= 0; i--) {
        std::cout << vertices[path[i]];
        if (i != 0) std::cout << " -> ";
    }

    std::cout << ". The lenght is " << distances[i_dest] << ". The cost is " << cost[i_dest] << ".\n";
    return distances[i_dest]; 
}

template <typename T>
void WeightedGraph<T>::kruskal() {
    struct KEdge {
        int u, v, cost;
    };

    std::vector<KEdge> allEdges;

    for (int i = 0; i < vertices.size(); i++) {
        for (const Edge& e : edges[i]) {
            if (i < e.neighbor) {
                allEdges.push_back({i, e.neighbor, e.cost});
            }
        }
    }

    int total = 0;
    int n = vertices.size();
    std::vector<int> parent(n);
    std::vector<int> rank(n, 0);

    for (int i = 0; i < n; i++) parent[i] = i;

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

    for (const auto& e : allEdges) {
        int rootU = findSet(e.u, parent);
        int rootV = findSet(e.v, parent);

        if (rootU != rootV) {
            unionSet(rootU, rootV, parent, rank);

            std::cout << vertices[e.u] << " - " << vertices[e.v] << "\t" << e.cost << "\n";

            total += e.cost;
        }
    }
    std::cout << "Total cost of MST: " << total << "\n";
}

template <typename T>
int WeightedGraph<T>::findSet(int x, std::vector<int>& parent) {
    if (parent[x] != x) parent[x] = findSet(parent[x], parent);
    return parent[x];
}

template <typename T>
void WeightedGraph<T>::unionSet(int a, int b, std::vector<int>& parent, std::vector<int>& rank) {
    int rootA = findSet(a, parent);
    int rootB = findSet(b, parent);

    if (rootA != rootB) {
        if (rank[rootA] < rank[rootB]) parent[rootA] = rootB;
        else if (rank[rootA] > rank[rootB]) parent[rootB] = rootA;
        else {
            parent[rootB] = rootA;
            rank[rootA]++;
        }
    }
}
