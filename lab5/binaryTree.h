//
// Created by fail on 3/31/21.
//

#ifndef LAB5_BINARYTREE_H
#define LAB5_BINARYTREE_H

#include <utility>

#include "customStack.h"
#include "customString.h"

struct Node {
    Node* left = nullptr;
    Node* right = nullptr;
    customString from;
    customString to;
    std::size_t counter = 0;
};

static Node* copyNode(Node* other) {
    if(!other) return nullptr;

    return new Node {copyNode(other->left), copyNode(other->right), other->from, other->to, other->counter };
}

void flatten(Node* root, customStack<Node*>& st) {
    if(!root) return;
    flatten(root->left, st);
    st.push(root);
    flatten(root->right, st);
}

static Node* findNode(Node* other, customString& str) {
    if(!other) return nullptr;

    int result = 0;
    for(std::size_t i = 0; i < str.size() && i < other->from.size(); ++i) {
        if(str[i] < other->from[i]) {
            result = -1;
            break;
        }

        if(str[i] > other->from[i]) {
            result = -1;
            break;
        }
    }
    if(result == 0) result = str.size() < other->from.size();

    if(result == 0) return other;
    if(result > 0) return findNode(other->left, str);

    return findNode(other->right, str);
}

static void deleteNode(Node* root) {
    if(!root) return;
    deleteNode(root->left);
    deleteNode(root->right);

    delete root;
}

static Node* insert(Node* root, const customString& from, const customString& to, std::size_t counter) {
    customString t = from;
    if(!root) {
        return new Node {nullptr, nullptr, from, to, counter};
    }

    if(strcmp(root->from.data(), t.data()) <= 0) {
        root->left = insert(root->left, from, to, counter);
    } else {
        root->right = insert(root->right, from, to, counter);
    }

    return root;
}

static void print(Node* root) {
    if(!root) {
        std::cout << "{}";
        return;
    }

    std::cout << "{" << root->from << " -> " << root->to << " " << root->counter << ", leftChild: ";
    print(root->left);
    std::cout << ", rightChild: ";
    print(root->right);
    std::cout << "}";
}

void dfs1(std::ostream& ss, Node* root) {
    if(!root) return;
    ss << "\"" << root << "\"" << " [label=\"" << root->from << " -> " << root->to << " (" << root->counter << ")\"];\n";
    dfs1(ss, root->left);
    dfs1(ss, root->right);
}

void dfs2(std::ostream& ss, Node* root) {
    if(!root) return;

    if(root->left) ss <<  "\"" << root << "\"" <<  " -> " <<  "\"" << root->left << "\"" <<  ";\n";
    if(root->right) ss <<  "\"" << root << "\"" << " -> " <<  "\"" << root->right << "\"" << ";\n";

    dfs2(ss, root->left);
    dfs2(ss, root->right);
}

void printDOT(Node* root, std::ostream& ss) {
    ss << "digraph {\n";
    dfs1(ss, root);
    dfs2(ss, root);
    ss << "}\n";
}

struct binaryTree {
private:
    explicit binaryTree(Node* r) : root(r) {};

public:
    Node* root = nullptr;
    binaryTree() : root(nullptr) {};

    binaryTree& operator=(const binaryTree& other) {
        if(&other == this) return *this;
        if(root) deleteNode(root);
        root = copyNode(other.root);
        return *this;
    }

    binaryTree(const binaryTree& other) {
        *this = other;
    }

    ~binaryTree() {
        deleteNode(root);
    }

    void insert(const customString& from, const customString& to, std::size_t counter) {
        root = ::insert(root, from, to, counter);
    }
};

#endif //LAB5_BINARYTREE_H
