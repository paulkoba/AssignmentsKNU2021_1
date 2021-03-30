//
// Created by fail on 3/30/21.
//

#ifndef LAB7_SORTS_H
#define LAB7_SORTS_H

#include "customStack.h"

template<typename T, typename P>
void merge(customStack<T>& arr, std::size_t l, std::size_t m, std::size_t r, P comparator) {
    std::size_t sizeT1 = m - l + 1;
    std::size_t sizeT2 = r - m;

    customStack<T> L(sizeT1), R(sizeT2);

    for(std::size_t i = 0; i < sizeT1; ++i) L[i] = arr[l + i];
    for(std::size_t i = 0; i < sizeT2; ++i) R[i] = arr[m + 1 + i];

    std::size_t i = 0, j = 0, k = l;

    while(i < sizeT1 && j < sizeT2) {
        if(comparator(L[i], R[j])) {
            arr[k] = L[i];
            ++i;
        } else {
            arr[k] = R[j];
            ++j;
        }
        ++k;
    }

    while(i < sizeT1) arr[k++] = L[i++];
    while(j < sizeT2) arr[k++] = R[j++];
}

template<typename T, typename P>
void mergeSort(customStack<T>& arr, std::size_t l, std::size_t r, P comparator) {
    if(l >= r) return;

    mergeSort(arr, l, (l + r) / 2, comparator);
    mergeSort(arr, (l + r) / 2 + 1, r, comparator);

    merge(arr, l, (l + r) / 2, r, comparator);
}

template<typename T, typename P>
void insertionSort(customStack<T>& arr, P comparator) {
    customStack<T> output;
    for(std::size_t i = 0; i < arr.size(); ++i) {
        output.push(arr[i]);
        std::size_t j = output.size() - 1;
        while(j - 1 != std::numeric_limits<std::size_t>::max()) {
            if(comparator(output[j - 1], output[j])) break;
            std::swap(output[j], output[j - 1]);
            --j;
        }
    }
    arr = output;
}

template<typename T, typename P>
void bubbleSort(customStack<T>& arr, P comparator) {
    for(std::size_t i = 0; i < arr.size(); ++i) {
        for(std::size_t j = 0; j < arr.size(); ++j) {
            if(comparator(arr[i],arr[j])) std::swap(arr[i],arr[j]);
        }
    }
}

template<typename T, typename P>
std::size_t partition(customStack<T>& arr, std::size_t low, std::size_t high, P comparator) {
    T p = arr[high];

    std::size_t i = low;

    for(std::size_t j = low; j < high; ++j) {
        if(comparator(arr[j], p)) {
            std::swap(arr[j], arr[i++]);
        }
    }

    std::swap(arr[i], arr[high]);
    return i;
}

template<typename T, typename P>
void quickSort(customStack<T>& arr, std::size_t low, std::size_t high, P comparator) {
    if(low >= high) return;

    std::size_t p = partition(arr, low, high, comparator);

    if(p) quickSort(arr, low, p - 1, comparator);
    quickSort(arr, p + 1, high, comparator);
}

template<typename T, typename P>
void shakerSort(customStack<T>& arr, P comparator) {
    std::size_t change = arr.size();
    std::size_t left = 0, right = arr.size() - 1;

    do {
        for(std::size_t i = left; i < right; ++i) {
            if(comparator(arr[i + 1], arr[i])) {
                std::swap(arr[i], arr[i + 1]);
                change = i;
            }
        }
        right = change;

        for(std::size_t i = right; i > left; --i) {
            if(comparator(arr[i], arr[i - 1])){
                std::swap(arr[i], arr[i - 1]);
                change = i;
            }
        }
        left = change;
    } while (left < right);
}

template<typename T, typename P>
void shellSort(customStack<T>& arr, P comparator) {
    for(std::size_t g = arr.size() / 2; g > 0; g /= 2) {
        for(std::size_t i = g; i < arr.size(); ++i) {
            T t = arr[i];

            std::size_t j = i;
            for(; j >= g && comparator(t, arr[j - g]); j -= g) {
                arr[j] = arr[j - g];
            }
            arr[j] = t;
        }
    }
}


#endif //LAB7_SORTS_H
