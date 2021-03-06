//
// Created by fail on 3/6/21.
//

#ifndef LAB4_EXPRTREE_H
#define LAB4_EXPRTREE_H

#include "rpn.h"

struct exprTreeNode {
    customString value = "";
    RPN::SubexpressionType type = RPN::SUBEXPRESSION_NIL;
    exprTreeNode* left = nullptr;
    exprTreeNode* right = nullptr;
    exprTreeNode* parent = nullptr;
    exprTreeNode(customString a, RPN::SubexpressionType e = RPN::SUBEXPRESSION_NIL, exprTreeNode* b = nullptr, exprTreeNode* c = nullptr, exprTreeNode* d = nullptr)
        : value(a), left(b), right(c), parent(d), type(e) {};
};

struct exprTree {
    exprTreeNode* root = nullptr;

    exprTree() = delete;
    explicit exprTree(customStack<RPN::Token> data) {
        root = generateExpressionTree(data);
    }

    static exprTreeNode* generateExpressionTree(customStack<RPN::Token>& a) {
        if(a.size() == 0) return nullptr;
        auto temp = a.pop();

        auto output = new exprTreeNode(temp.value, temp.type);

        if(temp.type == RPN::SUBEXPRESSION_OPERATOR) {
            if(!RPN::isUnary(temp.value)) {
                output->right = generateExpressionTree(a);
                if(output->right) output->right->parent = output;
            }
            output->left = generateExpressionTree(a);
            if(output->left) output->left->parent = output;
        }

        return output;
    }
};

exprTreeNode* findDifferential(exprTreeNode* root) {
    if(!root) return nullptr;

    switch (root->type) {
        case RPN::SUBEXPRESSION_CONSTANT:
            return new exprTreeNode("0", RPN::SUBEXPRESSION_CONSTANT);
        case RPN::SUBEXPRESSION_VARIABLE:
        {
            auto out = customString("d").append(root->value);
            return new exprTreeNode(out, RPN::SUBEXPRESSION_CONSTANT);
        }
        case RPN::SUBEXPRESSION_NIL:
            return nullptr;
        case RPN::SUBEXPRESSION_OPERATOR:
            if(root->value == OPERATOR_PLUS) {
                auto* sum = new exprTreeNode(OPERATOR_PLUS, RPN::SUBEXPRESSION_OPERATOR);
                sum->left = findDifferential(root->left);
                sum->right = findDifferential(root->right);

                return sum;
            }

            if(root->value == OPERATOR_MINUS) {
                auto* sub = new exprTreeNode(OPERATOR_MINUS, RPN::SUBEXPRESSION_OPERATOR);
                sub->left = findDifferential(root->left);
                sub->right = findDifferential(root->right);

                return sub;
            }

            if(root->value == OPERATOR_UNARY_MINUS) {
                auto* sub = new exprTreeNode(OPERATOR_UNARY_MINUS, RPN::SUBEXPRESSION_OPERATOR);
                sub->left = findDifferential(root->left);

                return sub;
            }

            if(root->value == OPERATOR_MULTIPLICATION) {
                auto* sum = new exprTreeNode(OPERATOR_PLUS, RPN::SUBEXPRESSION_OPERATOR);
                auto* prod1 = new exprTreeNode(OPERATOR_MULTIPLICATION, RPN::SUBEXPRESSION_OPERATOR);
                auto* prod2 = new exprTreeNode(OPERATOR_MULTIPLICATION, RPN::SUBEXPRESSION_OPERATOR);

                sum->left = prod1;
                sum->right = prod2;

                prod2->left = root->left;
                prod2->right = findDifferential(root->right);

                prod1->left = root->right;
                prod1->right = findDifferential(root->left);

                return sum;
            }

            if(root->value == OPERATOR_DIVISION) {
                auto* sub = new exprTreeNode(OPERATOR_MINUS, RPN::SUBEXPRESSION_OPERATOR);
                auto* div = new exprTreeNode(OPERATOR_DIVISION, RPN::SUBEXPRESSION_OPERATOR);
                auto* mul1 = new exprTreeNode(OPERATOR_MULTIPLICATION, RPN::SUBEXPRESSION_OPERATOR);
                auto* mul2 = new exprTreeNode(OPERATOR_MULTIPLICATION, RPN::SUBEXPRESSION_OPERATOR);
                auto* pow = new exprTreeNode(OPERATOR_POWER, RPN::SUBEXPRESSION_OPERATOR);

                sub->left = mul1;
                sub->right = mul2;
                mul2->left = findDifferential(root->right);
                mul2->right = root->left;
                mul1->left = findDifferential(root->left);
                mul1->right = root->right;
                div->left = sub;
                div->right = pow;
                pow->left = root->right;
                pow->right = new exprTreeNode("2", RPN::SUBEXPRESSION_CONSTANT);

                return div;
            }

            if(root->value == OPERATOR_POWER) {
                auto* mul1 = new exprTreeNode(OPERATOR_MULTIPLICATION, RPN::SUBEXPRESSION_OPERATOR);
                auto* pow = new exprTreeNode(OPERATOR_POWER, RPN::SUBEXPRESSION_OPERATOR);
                auto* sum = new exprTreeNode(OPERATOR_PLUS, RPN::SUBEXPRESSION_OPERATOR);
                auto* mul2 = new exprTreeNode(OPERATOR_MULTIPLICATION, RPN::SUBEXPRESSION_OPERATOR);
                auto* mul3 = new exprTreeNode(OPERATOR_MULTIPLICATION, RPN::SUBEXPRESSION_OPERATOR);
                auto* mul4 = new exprTreeNode(OPERATOR_MULTIPLICATION, RPN::SUBEXPRESSION_OPERATOR);
                auto* sub = new exprTreeNode(OPERATOR_MINUS, RPN::SUBEXPRESSION_OPERATOR);
                auto* log = new exprTreeNode(OPERATOR_LOG, RPN::SUBEXPRESSION_OPERATOR);

                pow->left = root->left;
                pow->right = sub;
                sub->left = root->right;
                sub->right = new exprTreeNode("1", RPN::SUBEXPRESSION_CONSTANT);
                mul1->left = root->right;
                mul1->right = findDifferential(root->left);
                sum->left = mul1;
                sum->right = mul3;
                mul3->left = mul2;
                mul3->right = findDifferential(root->right);
                mul2->left = root->left;
                mul2->right = log;
                log->left = root->left;
                mul4->left = pow;
                mul4->right = sum;
                return mul4;
            }

            if(root->value == OPERATOR_LOG) {
                auto* inv = new exprTreeNode(OPERATOR_DIVISION, RPN::SUBEXPRESSION_OPERATOR);
                inv->left = findDifferential(root->left);
                inv->right = root->left;
                return inv;
            }

            if(root->value == OPERATOR_EXP) {
                auto* mul1 = new exprTreeNode(OPERATOR_MULTIPLICATION, RPN::SUBEXPRESSION_OPERATOR);
                mul1->left = root;
                mul1->right = findDifferential(root->left);
                return mul1;
            }

        default:
            return nullptr;
    }
}

#endif //LAB4_EXPRTREE_H
