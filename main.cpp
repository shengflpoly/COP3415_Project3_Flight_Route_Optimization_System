#include "WeightedGraph.hpp"

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <string>
#include <regex>

void runFromFile(const char* file_name, WeightedGraph<Edge>& graph);
std::vector<std::string> split(const std::string& s);
void runQuery(const std::vector<std::string>& query, WeightedGraph<Edge>& graph);
int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Error: incorrect number of arguments\n";
        return 1;
    }

    WeightedGraph<Edge> graph;
    try {
        runFromFile(argv[1], graph);
    }
    catch(std::logic_error& ex) {
        std::cout << ex.what() << std::endl;
    }
    return 0;
}

void runFromFile(const char* file_name, WeightedGraph<Edge>& graph) {
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
    std::regex del(",");

    std::sregex_token_iterator it(s.begin(), s.end(), del, -1);

    std::sregex_token_iterator end;

    while (it != end) {
        out.push_back(*it);
        ++it;
    }

    return out;
}

void runQuery(const std::vector<std::string>& query, WeightedGraph<Edge>& graph) {
    
}
