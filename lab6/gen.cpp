#include <iostream>
#include <fstream>

#include "graph.h"

int main() {
	graph gr(25);

	std::ofstream input("input");
	for(int i = 0; i < 25; ++i) {
		for(int j = 0; j < 25; ++j) {
			gr.addEdge(i, j, rand() % 90 + 10);
		}
	}

	input << gr;

	return 0;
}
