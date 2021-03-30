#include <iostream>
#include <fstream>
#include <cmath>
#include <tuple>

#include "graph.h"
#include "sorts.h"

//Technically UB
template<typename T>
std::ostream& operator <<(std::ostream& os, const std::tuple<T, T, T>& f) {
    os << "{" << std::get<0>(f) << ", " << std::get<1>(f) << ", " << std::get<2>(f) << "}";
    return os;
}

//Technically UB
template<typename T, typename S>
std::ostream& operator <<(std::ostream& os, const std::pair<T, S>& f) {
    os << "{" << f.first << ", " << f.second << "}";
    return os;
}

template<typename T>
std::ostream& operator <<(std::ostream& os, customStack<T>& f) {
    if(f.empty()) {
        os << "{}";
        return os;
    }

    os << "{" << f[0];
    for(std::size_t i = 1; i < f.size(); ++i) os << ", " << f[i];
    os << "}";

    return os;
}

void printByNEdges(graph& gr) {
    struct cmp {
        bool operator() (std::pair<std::size_t, uint64_t>& a, std::pair<std::size_t, uint64_t>& b) {
            return a.second < b.second;
        }
    };

    customStack<std::pair<std::size_t, uint64_t>> counts(gr.size());
    for(std::size_t i = 0; i < gr.size(); ++i) {
        counts[i].first = i;
        for(std::size_t j = 0; j < gr.size(); ++j) {
            counts[i].second += (gr[i][j] != 0);
        }
    }
    insertionSort(counts, cmp());
    std::cout << "Vertices sorted by number of outgoing edges: " << counts << "\n";
}


void printByLengthOfEdges(graph& gr) {
    struct cmp {
        bool operator() (std::pair<std::size_t, uint64_t>& a, std::pair<std::size_t, uint64_t>& b) {
            return a.second < b.second;
        }
    };

    customStack<std::pair<std::size_t, uint64_t>> counts(gr.size());
    for(std::size_t i = 0; i < gr.size(); ++i) {
        counts[i].first = i;
        for(std::size_t j = 0; j < gr.size(); ++j) {
            counts[i].second += gr[i][j];
        }
    }
    bubbleSort(counts, cmp());
    std::cout << "Vertices sorted by number of outgoing edges: " << counts << "\n";
}

template<typename P>
void printByCoords(customStack<std::pair<int64_t, int64_t>> arr, P comp) {
    shakerSort(arr, comp);
    std::cout << "Vertices sorted by coords: " << arr << "\n";
}

template<typename P>
void printByEdgeCenterCoords(customStack<std::pair<int64_t, int64_t>> arr, P comp) {
    customStack<std::pair<std::pair<std::size_t, std::size_t>, std::pair<std::size_t, std::size_t>>> d;

    for(std::size_t i = 0; i < arr.size(); ++i) {
        for(std::size_t j = 0; j < i; ++j) {
            d.push({{(arr[i].first - arr[j].first) / 2, (arr[i].second - arr[j].second) / 2}, {i, j}});
        }
    }

    mergeSort(d, comp);
    std::cout << "Edges sorted by P (format: {{centerX, centerY}, {V #1, V #2}}): " << d << "\n";
}

void printByEdgeLength(graph& gr) {
    customStack<std::pair<uint64_t, std::pair<std::size_t, uint64_t>>> counts;
    for(std::size_t i = 0; i < gr.size(); ++i) {
        for(std::size_t j = 0; j < i; ++j) {
            if(gr[i][j] == 0) continue;
            std::pair<uint64_t, std::pair<std::size_t, uint64_t>> t = {gr[i][j], {i, j}};
            counts.push(t);
        }
    }
    quickSort(counts, 0, counts.size() - 1, std::less<>());
    std::cout << "Edges sorted by lengths (format: {edgeLength, {vertex #1, vertex # 2}}): " << counts << "\n";
}

void printByArea(graph& gr) {
    customStack<std::pair<uint64_t, std::tuple<std::size_t, std::size_t, std::size_t>>> counts;

    for(std::size_t i = 0; i < gr.size(); ++i) {
        for(std::size_t j = 0; j < i; ++j) {
            for(std::size_t k = 0; k < j; ++k) {
                if(!gr[i][j] || !gr[i][k] || !gr[j][k]) continue;

                double u = (double)(gr[i][j] + gr[i][k] + gr[j][k]) / 2;
                if(u < gr[i][j] || u < gr[i][k] || u < gr[j][k]) continue;
                counts.push({std::sqrt(u * (u - gr[i][j]) * (u - gr[i][k]) * (u - gr[j][k])), std::make_tuple(gr[i][j], gr[j][k], gr[i][k])});
            }
        }
    }

    shellSort(counts, std::less<>());
    std::cout << "Edges sorted by area (format: {area, {V #1, V #2, V #3}}): " << counts << "\n";
}

int main() {
    graph gr(10);
    std::ifstream str("input");

    str >> gr;

    printByNEdges(gr);
    printByLengthOfEdges(gr);
    printByEdgeLength(gr);
    printByArea(gr);

    return 0;
}
