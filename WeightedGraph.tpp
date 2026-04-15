#include "WeightedGraph.hpp"
#include <iostream>
#include <queue>

template <typename T>
void WeightedGraph<T>::insertVertex(const T& v) {
    if (getVertexIndex(v) != -1) {
        std::cout << "insertVertex: vertex already exist\n";
        return;
    }

    vertices.push_back(v);
    std::vector<int> tmp; // TODO
    edges.push_back(tmp); //insert empty vector to the edges
}

// TODO
template <typename T>
void WeightedGraph<T>::insertEdge(const T& v1, const T& v2) {
    int i1 = getVertexIndex(v1);
    int i2 = getVertexIndex(v2);
    if (i1 == -1 || i2 == -1) {
        std::cout << "insertEdge: incorrect vertices\n";
        return;
    }

    if (!hasEdge(i1, i2)) {
        edges[i1].push_back(i2);
        if (i1 != i2) {
            edges[i2].push_back(i1);
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
            std::cout << vertices[edges[i][j]] << ' ';
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

    for (int i : edges[i1]) {
        if (i == i2) {
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
    for (int j : edges[i]) {
        if (!visited[j]) {
            DFS(j, visited);
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
        for (int j : edges[cur]) {
            if (!discovered[j]) {
                where_to_go.push(j);
                discovered[j] = true;
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

    // Check edge case
    if (i_src == -1 || i_dest == -1) {
        std::cout << "shortestPath: incorrect indices";
        return -1;
    }
    if (i_src == i_dest) {
        return 0;
    }
    // Create distances vector
    std::vector<int> distances(vertices.size()); // distances from source to all other nodes
    // Set initial distances
    for (int i = 0; i < distances.size(); i++) {
        distances[i] = (i == i_src) ? 0 : -1;
    }

    // Perform BFS and update distances
    std::queue<int> q;
    q.push(i_src);

    while (!q.empty()) {
        int cur = q.front();
        q.pop();

        // Check the neighbors of current node
        for (int i : edges[cur]) {
            if (distances[i] == -1) {
                distances[i] = distances[cur] + 1;
                q.push(i);
            }
            if (i == i_dest) {
                return distances[i];
            }
        }
    }

    return -1; // No path exists
}