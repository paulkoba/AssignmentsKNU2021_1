#include <iostream>
#include <fstream>
#include "binaryTree.h"
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

int main() {
    binaryTree tree;

    while(true) {
        std::cout << "Please enter a new word: ";
        Node n;
        std::cin >> n.from;
        std::cout << "Please enter its translation: ";
        std::cin >> n.to;
        std::cout << "Please enter its frequency: ";
        std::cin >> n.counter;

        tree.insert(n.from, n.to, n.counter);

        std::cout << "Do you wish to proceed? (Y/N) ";
        char ch = 0;
        std::cin >> ch;
        while(ch != 'y' && ch != 'n' && ch != 'Y' && ch != 'N') {
            std::cout << "Please try again. ";
            std::cin >> ch;
        }

        if(ch == 'n' || ch == 'N') break;
    }

    std::ofstream out("before");
    printDOT(tree.root, out);

    std::cout << "Please enter a line of text: ";
    customString text;
    std::cin >> text;
    text.append(' ');

    customStack<customString> str;
    customString current;

    for(char ch : text) {
        if(isspace(ch)) {
            if(current.size() != 0) str.push(current);
            current = "";
        } else {
            current.append(ch);
        }
    }

    for(customString& s : str) {
        Node* n = findNode(tree.root, s);
        if(!n) continue;
        ++(n->counter);
    }

    customStack<Node*> t;

    flatten(tree.root, t);

    mergeSort(t, 0, t.size() - 1, [] (Node* a, Node* b) { return a->counter > b->counter; });

    binaryTree tree2;

    for(auto g : t) tree2.insert(g->from, g->to, g->counter);

    std::ofstream out2("after");
    printDOT(tree2.root, out2);

    return 0;
}
