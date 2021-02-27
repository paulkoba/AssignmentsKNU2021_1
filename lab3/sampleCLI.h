//
// Created by fail on 2/27/21.
//

#ifndef LAB3_SAMPLECLI_H
#define LAB3_SAMPLECLI_H

#include <fstream>

#include "sparseMatrix.h"
#include "lzw/lzw.h"

namespace sampleCLI {
    static const char* filename = "data";
    static const char* filenameA = "data.z";
    static const char* filenameT = "data.temp";

    void fillMatrix(sparseMatrix& spr) {
        while(true) {
            std::size_t row = 0, col = 0;
            int value = 0;
            std::cout << "Enter row: ";
            std::cin >> row;
            std::cout << "Enter column: ";
            std::cin >> col;
            std::cout << "Enter value: ";
            std::cin >> value;

            spr[{row, col}] = value;

            std::cout << "Do you want to continue (Y/N)?\n";
            char ch = 0;
            std::cin >> ch;
            if(ch == 'N' || ch == 'n') break;
        }
    }

    void run() {
        std::size_t rows = 0, cols = 0;
        std::cout << "Please enter N:";
        std::cin >> rows;
        std::cout << "Please enter M:";
        std::cin >> cols;

        sparseMatrix data(rows, cols);

        std::cout << "Filling matrix with elements...\n";
        fillMatrix(data);

        while(true) {
            char ch = 0, in = 0;

            std::cout << "Enter 'a' to save matrix to file.\n"
                         "Enter 'b' to load matrix from file.\n"
                         "Enter 'c' to add another matrix to current matrix.\n"
                         "Enter 'd' to multiply current matrix by another one.\n"
                         "Enter 'e' to view current matrix in sparse view.\n"
                         "Enter 'f' to view current matrix in human-friendly way.\n"
                         "Enter 'q' to quit.\n";

            std::cin >> ch;

            switch (ch) {
                case 'a':
                    std::cout << "Do you want to compress data?(Y/N)\n";
                    std::cin >> in;

                    if(in == 'N' || in == 'n') {
                        std::ofstream out(filename);
                        out << data;
                    } else {
                        std::ofstream temp(filenameT);
                        temp << data;
                        temp.close();
                        FILE* tempFileC = fopen(filenameT, "rb+");
                        FILE* outputFile = fopen(filenameA, "wb+");

                        LZWEncodeFile(tempFileC, outputFile);

                        fclose(tempFileC);
                        fclose(outputFile);

                        remove(filenameT);
                    }

                    break;
                case 'b':
                    std::cout << "Do you read compressed data?(Y/N)\n";
                    std::cin >> in;

                    if(in == 'N' || in == 'n') {
                        std::ifstream in(filename);
                        in >> data;
                    } else {
                        FILE* tempFileC = fopen(filenameT, "wb+");
                        FILE* outputFile = fopen(filenameA, "rb+");

                        LZWDecodeFile(outputFile, tempFileC);

                        fclose(tempFileC);
                        fclose(outputFile);
                        std::ifstream t(filenameT);
                        t >> data;
                        t.close();
                        remove(filenameT);
                    }
                    break;
                case 'c':
                {
                    sparseMatrix add(data.rows, data.cols);
                    fillMatrix(add);

                    data = data + add;
                }
                    break;
                case 'd':
                {
                    std::cout << "Please enter N:";
                    std::cin >> rows;
                    std::cout << "Please enter M:";
                    std::cin >> cols;

                    sparseMatrix mul(data.rows, data.cols);

                    fillMatrix(mul);

                    data = data * mul;
                }
                    break;
                case 'e':
                    std::cout << data;
                    break;
                case 'f':
                    data.print();
                    break;
                case 'q':
                    return;
                default:
                    std::cout << "Invalid character.\n";
                    break;
            }
        }
    }
}

#endif //LAB3_SAMPLECLI_H
