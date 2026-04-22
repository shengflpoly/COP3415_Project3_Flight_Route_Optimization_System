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

    graph.print();

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
    graph.insertEdge(origin, dest, distance, cost);
}
