#include <iostream>
#include <fstream>

#include "graph.h"

int main() {
	graph gr(10);

	std::ofstream input("input");
	for(int i = 0; i < 10; ++i) {
		for(int j = 0; j < 10; ++j) {
			gr.addEdge(i, j, rand() % 90 + 10);
		}
	}

	input << gr;

	return 0;
}
