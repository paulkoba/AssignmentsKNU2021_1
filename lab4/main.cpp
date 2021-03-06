#include "rpn.h"
#include "exprTree.h"

#include <fstream>

void dfs1(exprTreeNode* root, std::ofstream& file) {
    if(!root) return;
    file << "A" << root << " [" << "label=\"" << root->value << "\"]\n";
    dfs1(root->left, file);
    dfs1(root->right, file);
}

void dfs2(exprTreeNode* root, std::ofstream& file) {
    if(!root) return;
    if(root->left) {
        file << "A" << root << " -> " << "A" << root->left << "\n";
    }

    if(root->right) {
        file << "A" << root << " -> " << "A" << root->right << "\n";
    }

    dfs2(root->left, file);
    dfs2(root->right, file);
}

void saveToGraphViz(exprTreeNode* root) {
    std::ofstream out("output");

    out << "digraph {\n";

    dfs1(root, out);
    dfs2(root, out);

    out << "}\n";
}

int main() {
    customString comm;

    std::cout << "Please enter expression: \n";
    std::cin >> comm;

    auto d = RPN::generateRPN(comm);
    for(auto& i : d) {
        std::cout << i.value << "\n";
    }
    auto tree = exprTree(d);
    auto result = findDifferential(tree.root);
    saveToGraphViz(result);

    return 0;
}
