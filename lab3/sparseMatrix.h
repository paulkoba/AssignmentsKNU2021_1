//
// Created by fail on 2/25/21.
//

#ifndef LAB3_SPARSEMATRIX_H
#define LAB3_SPARSEMATRIX_H

#include <cassert>
#include <iostream>
#include <tuple>

#include "customUnorderedMap.h"

struct intPairHasher {
    std::size_t operator()(std::pair<int, int> a) {
        return std::hash<int>()(a.first) ^ std::hash<int>()(a.second);
    }
};

typedef customStack<std::tuple<std::size_t, std::size_t, int>> linearSparseRepresentation;
typedef customStack<customStack<std::pair<std::size_t, int>>> indexSparseRepresentation;

struct sparseMatrix {
    customUnorderedMap<std::pair<int, int>, int, intPairHasher> matrix;
    std::size_t rows = 0, cols = 0;

    sparseMatrix() = delete;
    sparseMatrix(std::size_t a, std::size_t b) noexcept : rows(a), cols(b) {};

    int& operator[](std::pair<int, int> p) {
        return matrix[p];
    }

    void print(std::ostream& os = std::cout) {
        for(std::size_t i = 0; i < rows; ++i) {
            for(std::size_t j = 0; j < cols; ++j) {
                if(matrix.contains({i, j})) {
                    os << matrix[{i, j}] << " ";
                } else {
                    os << 0 << " ";
                }
            }
            os << "\n";
        }
    }

    sparseMatrix operator+(sparseMatrix& other) {
        assert(other.cols == cols && other.rows == rows);

        sparseMatrix output(rows, cols);

        for(auto& p : matrix) {
            output[p.first] += p.second;
        }

        for(auto& p : other.matrix) {
            output[p.first] += p.second;
        }

        return output;
    }

    sparseMatrix operator*(sparseMatrix& other) {
        assert(other.rows == cols);
        sparseMatrix output(rows, other.cols);
        for(auto& elem : matrix) {
            for(std::size_t i = 0; i < other.cols; ++i) {
                if(other.matrix.contains({elem.first.second, i})) {
                    output[{elem.first.first, i}] += elem.second * other[{elem.first.second, i}];
                }
            }
        }

        return output;
    }

    friend std::ostream& operator<< (std::ostream& out, sparseMatrix& a) {
        out << a.matrix.size() << " " << a.rows << " " << a.cols << "\n";

        for(auto& elem : a.matrix) {
            out << elem.first.first << " " << elem.first.second << " " << elem.second <<"\n";
        }

        return out;
    }

    friend std::istream& operator>> (std::istream& in, sparseMatrix& a) {
        std::size_t count = 0, r = 0, c = 0;
        in >> count >> r >> c;
        sparseMatrix output(r, c);

        for(std::size_t i = 0; i < count; ++i) {
            in >> r >> c;
            int val = 0;
            in >> val;

            output[{r, c}] = val;
        }

        a = output;

        return in;
    }

    linearSparseRepresentation toLinearSparse() {
        linearSparseRepresentation output;

        for(auto& item : matrix) {
            output.push({item.first.first, item.first.second, item.second});
        }

        return output;
    }

    indexSparseRepresentation toIndexSparse() {
        indexSparseRepresentation output(rows);

        for(auto& item : matrix) {
            output[item.first.first].push({item.first.second, item.second});
        }

        return output;
    }
};

#endif //LAB3_SPARSEMATRIX_H
