#ifndef WEIGHTEDGRAPH_HPP
#define WEIGHTEDGRAPH_HPP
#include <vector>

class Edge {
public:
    int neighbor;
    int distance;
    int cost;
    int weight;

    Edge(int n, int d = 0, int c = 0, int w = 0) : neighbor(n), distance(d), cost(c), weight(w) {}

    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

template <typename T>
class WeightedGraph {
public:

    void insertVertex(const T& v);
    void insertEdge(const T& v1, const T& v2, int distance, int cost, bool direct = true);
    void print() const;

    void DFS() const;
    void BFS(int start = 0) const;
    int shortestPath(const T& src, const T& dest) const;
    bool isConnected() const;

    void kruskal();
    // Task 6
    WeightedGraph<T> buildUndirectedGraph() const;
    
    // Task 7
    void prim() const;

private:
    std::vector<T> vertices; 
    std::vector<std::vector<Edge>> edges; 

    
    int getVertexIndex(const T& vs) const;

    bool hasEdge(int i1, int i2) const;
    void DFS(int i, std::vector<bool>& visited) const;

    int findSet(int x, std::vector<int>& parent);
    void unionSet(int a, int b, std::vector<int>& parent, std::vector<int>& rank);
};

#include "WeightedGraph.tpp"

#endif