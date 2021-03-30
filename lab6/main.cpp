#include <iostream>
#include <fstream>

#include "graph.h"

int main() {
    graph a(25);
    std::ifstream input("input");

    input >> a;
    graph b = a.maxSpanningTree();
    std::cout << b;

    return 0;
}
