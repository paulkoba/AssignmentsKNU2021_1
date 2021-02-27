cmake_minimum_required(VERSION 3.17)
project(lab3)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra")

add_executable(lab3 main.cpp customUnorderedMap.h customStack.h sparseMatrix.h customString.h sampleCLI.h lzw/lzw.h lzw/lzwencode.cpp lzw/lzwdecode.cpp lzw/bitfile.cpp lzw/bitfile.h lzw/lzwlocal.h)