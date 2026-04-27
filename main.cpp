#include "WeightedGraph.hpp"

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <string>


void runFromFile(const char* file_name, WeightedGraph<std::string>& graph);
std::vector<std::string> split(const std::string& s);
void runQuery(const std::vector<std::string>& query, WeightedGraph<std::string>& graph);

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Error: incorrect number of arguments\n";
        return 1;
    }

    WeightedGraph<std::string> graph;
    try {
        runFromFile(argv[1], graph);
    }
    catch(std::logic_error& ex) {
        std::cout << ex.what() << std::endl;
    }

    int Choice = 0;
    printf("Make a choice.\n");
    printf("1: Graph Construction & Data Handling.\n");
    printf("2: Shortest Path (Single Pair).\n");
    printf("3: Shortest Paths to State.\n");
    printf("4: Shortest Path with Exact Stops.\n");
    printf("5: Airport Connectivity Count.\n");
    printf("6: Undirected Graph Construction.\n");
    printf("7: Prim's Algorithm (MST).\n");
    printf("8: Kruskal's Algorithm (MSF).\n\n");
    std::cin >> Choice;
    printf("\n\n\n");
    switch(Choice){
        //Task 1
        case 1: 
            graph.print();
            break;
        //Task 2
        case 2: 
            graph.shortestPath("IAD", "MIA");
            break;
        //Task 3 TBD
        case 3: 
            printf("Whoops!");
            break;
        //Task 4 TBD
        case 4: 
            printf("Whoops!");
            break;
        //Task 5 TBD
        case 5: 
            printf("Whoops!");
            break;
        //Task 6 TBD
        case 6: 
            printf("Whoops!");
            break;
        //Task 7 TBD
        case 7: 
            printf("Whoops!");
            break;
        //Task 8
        case 8: 
            graph.kruskal();
            break;
        //Err
        default: 
            printf("Whoops!");
            break;
    }

    // Task 6
    WeightedGraph<std::string> undirected = graph.buildUndirectedGraph();
    
    // Task 7
    undirected.prim();

    return 0;
}

void runFromFile(const char* file_name, WeightedGraph<std::string>& graph) {
    std::ifstream in;

    in.open(file_name);

    if (in.fail()) {
        std::cerr << file_name << " could not be opened\n";
        throw std::logic_error("File cound not be opened\n");
    }

    std::string line;
    std::vector<std::string> words;
    while (getline(in, line)) {
        words = split(line);
        runQuery(words, graph);
    }

    in.close();
}

std::vector<std::string> split(const std::string& s) {
    std::vector<std::string> out;
    std::string cur;
    bool quote = false;

    for (char c : s) {
        if (c == '"') {
            quote = !quote;
        }
        else if (c == ',' && (quote == false)) {
            out.push_back(cur);
            cur.clear();
        }
        else {
            cur += c;
        }
    }
    out.push_back(cur);
    return out;
}

void runQuery(const std::vector<std::string>& query, WeightedGraph<std::string>& graph) {
    if (query[0] == "Origin_airport") return;

    std::string origin = query[0];
    std::string dest = query[1];
    int distance = std::stoi(query[4]);
    int cost = std::stoi(query[5]);

    graph.insertVertex(origin);
    graph.insertVertex(dest);
    graph.insertEdge(origin, dest, distance, cost, true);
}
