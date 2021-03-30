//
// Created by fail on 3/30/21.
//

#ifndef LAB5_GRAPH_H
#define LAB5_GRAPH_H

#include <cstdint>
#include <iostream>

#include "customPriorityQueue.h"
#include "customStack.h"

//Weighted undirected graph.
//Size is set on creation
struct [[maybe_unused]] graph {
private:
    customStack<customStack<std::uint64_t>> data;
    std::size_t size = 0;
public:
    graph() = delete;
    ~graph() = default;
    graph(const graph& other) = default;
    graph(graph&& other) = default;
    graph& operator=(const graph& other) = default;
    graph& operator=(graph&& other) = default;

    [[maybe_unused]] explicit graph(std::size_t cSize) {
        size = cSize;
        for(std::size_t i = 0; i < size; ++i) data.push(customStack<std::uint64_t>(size));
    }

    [[maybe_unused]] void addEdge(std::size_t i, std::size_t j, std::uint64_t weight) {
        if(i >= size || j >= size) return;
        if(i == j) return;
        data[i][j] = weight;
        data[j][i] = weight;
    }

    [[maybe_unused]] void removeEdge(std::size_t i, std::size_t j) {
        if(i >= size || j >= size) return;
        data[i][j] = 0;
        data[j][i] = 0;
    }

    customStack<std::uint64_t>& operator[] (std::size_t i) { return data[i]; }

    friend std::ostream& operator<< (std::ostream& out, graph& a) {
        out << a.size << "\n";

        for(std::size_t i = 0; i < a.size; ++i) {
            for(std::size_t j = 0; j < a.size; ++j) {
                out << a[i][j] << " ";
            }
            out << "\n";
        }

        return out;
    }

    graph maxSpanningTree() {
        customPriorityQueue<std::pair<uint64_t, std::pair<size_t, size_t>>> q;
        customStack<bool> inserted(size);
        std::size_t visitedCount = 1;
        graph nGraph(size);

        for(std::size_t i = 0; i < size; ++i) {
            if(data[0][i] != 0) q.push({data[0][i], {0, i}});
        }

        while(visitedCount <= size) {
            if(q.empty()) break;
            auto res = q.top();
            q.pop();

            if(inserted[res.second.second]) continue;
            inserted[res.second.second] = true;
            nGraph.addEdge(res.second.first, res.second.second, res.first);

            for(std::size_t i = 0; i < size; ++i) {
                if(data[res.second.second][i] != 0) q.push({data[res.second.second][i], {res.second.second, i}});
            }
            ++visitedCount;
        }

        return nGraph;
    }

    friend std::istream& operator>> (std::istream& in, graph& a) {
        in >> a.size;
        a.data = customStack<customStack<std::uint64_t>>();

        for(std::size_t i = 0; i < a.size; ++i) a.data.push(customStack<std::uint64_t>(a.size));

        for(std::size_t i = 0; i < a.size; ++i) {
            for(std::size_t j = 0; j < a.size; ++j) {
                in >> a[i][j];
            }
        }

        return in;
    }
};

#endif //LAB5_GRAPH_H
