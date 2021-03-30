//
// Created by fail on 3/30/21.
//

#ifndef LAB5_CUSTOMPRIORITYQUEUE_H
#define LAB5_CUSTOMPRIORITYQUEUE_H

#include "customStack.h"

template<typename T>
struct customPriorityQueue {
    T top() {
        return data[0];
    }

    void pop() {
        if(data.size() == 0) return;
        if(data.size() == 1) {
            data.pop();
            return;
        }

        data[0] = data[data.size() - 1];
        data.pop();

        heapify(0);
    }

    void push(T val) {
        data.push(val);

        std::size_t pos = data.size() - 1;

        while(pos != 0 && data[getParent(pos)] < data[pos]) {
            std::swap(data[getParent(pos)], data[pos]);
            pos = getParent(pos);
        }
    }
    customStack<T> data;

    bool empty() { return data.empty(); }
private:

    void heapify(std::size_t pos) {
        std::size_t l = getLeft(pos);
        std::size_t r = getRight(pos);

        std::size_t s = pos;

        if(l < data.size() && data[l] > data[s]) s = l;
        if(r < data.size() && data[r] > data[s]) s = r;

        if(s != pos) {
            std::swap(data[pos], data[s]);
            heapify(s);
        }
    }

    static std::size_t getParent(std::size_t t) {
        return (t - 1) / 2;
    }

    static std::size_t getLeft(std::size_t t) {
        return 2 * t + 1;
    }

    static std::size_t getRight(std::size_t t) {
        return 2 * t + 2;
    }
};

#endif //LAB5_CUSTOMPRIORITYQUEUE_H
