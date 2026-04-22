#ifndef WEIGHTEDGRAPH_HPP
#define WEIGHTEDGRAPH_HPP
#include <vector>

class Edge {
public:
    int neighbor;
    int distance;
    int cost;

    Edge(int n, int d, int c) : neighbor(n), distance(d), cost(c) {}

    bool operator<(const Edge& other) const {
        return distance < other.distance;
    }
};

template <typename T>
class WeightedGraph {
public:

    void insertVertex(const T& v);
    void insertEdge(const T& v1, const T& v2, int distance, int cost);
    void print() const;

    void DFS() const;
    void BFS(int start = 0) const;
    int shortestPath(const T& src, const T& dest, bool control) const;
    bool isConnected() const;


private:
    std::vector<T> vertices; 
    std::vector<std::vector<Edge>> edges; 

    
    int getVertexIndex(const T& vs) const;

    bool hasEdge(int i1, int i2) const;
    void DFS(int i, std::vector<bool>& visited) const;
};

#include "WeightedGraph.tpp"

#endif