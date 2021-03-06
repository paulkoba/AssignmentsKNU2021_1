#pragma once

#include <algorithm>
#include <limits>
#include <cmath>

#include "customString.h"
#include "customStack.h"

static const char* OPERATOR_UNARY_MINUS = "Unary -";
static const char* OPERATOR_PLUS = "+";
static const char* OPERATOR_MINUS = "-";
static const char* OPERATOR_MULTIPLICATION = "*";
static const char* OPERATOR_POWER = "^^";
static const char* OPERATOR_DIVISION = "/";
static const char* OPERATOR_MODULUS = "%";
static const char* OPERATOR_LEFTBRACKET = "(";
static const char* OPERATOR_RIGHTBRACKET = ")";
static const char* OPERATOR_NIL = ";";
static const char* OPERATOR_LESSTHAN = "<";
static const char* OPERATOR_GREATERTHAN = ">";
static const char* OPERATOR_BITWISEAND = "&";
static const char* OPERATOR_AND = "&&";
static const char* OPERATOR_BITWISEOR = "|";
static const char* OPERATOR_OR = "||";
static const char* OPERATOR_XOR = "^";
static const char* OPERATOR_EQUALS = "=";
static const char* OPERATOR_NOT = "!";
static const char* OPERATOR_LOG = "LOG";
static const char* OPERATOR_EXP = "EXP";

struct RPN {
    static inline bool isUnary(customString& op) {
        return (op == OPERATOR_UNARY_MINUS || op == OPERATOR_LOG || op == OPERATOR_EXP);
    }

    enum SubexpressionType {
        SUBEXPRESSION_VARIABLE, SUBEXPRESSION_CONSTANT, SUBEXPRESSION_OPERATOR, SUBEXPRESSION_STRING, SUBEXPRESSION_NIL, SUBEXPRESSION_KEYWORD
    };

    struct Token {
        SubexpressionType type = SUBEXPRESSION_NIL;
        customString value = "";
    };

    RPN() = delete;
    static customStack<Token> generateRPN(customString& command) {
        customStack<Token> rpn = tokenize(command);
        rpn = normalize(rpn);
        rpn = convertToRPN(rpn);

        return rpn;
    }

private:

    static inline Token nullToken() { return Token{ SUBEXPRESSION_NIL, "" }; };

    static inline bool isOperator(char ch) {
        return ch == '-' || ch == '+' || ch == '/' || ch == '*' || ch == '=' ||
               ch == '!' || ch == '&' || ch == '|' || ch == ',' || ch == '^' ||
               ch == '%' || ch == '(' || ch == ')' || ch == '<' || ch == '>' || ch == ';';
    }

    static inline bool isBracket(char ch) {
        return ch == '(' || ch == ')';
    }

    static int64_t precedence(customString op) {
        if (op == OPERATOR_EXP) return 2;
        if (op == OPERATOR_UNARY_MINUS) return 1;
        if (op == OPERATOR_PLUS) return 5;
        if (op == OPERATOR_MINUS) return 5;
        if (op == OPERATOR_MULTIPLICATION) return 4;
        if (op == OPERATOR_POWER) return 1;
        if (op == OPERATOR_DIVISION) return 4;
        if (op == OPERATOR_MODULUS) return 4;
        if (op == OPERATOR_LESSTHAN) return 7;
        if (op == OPERATOR_GREATERTHAN) return 7;
        if (op == OPERATOR_BITWISEAND) return 9;
        if (op == OPERATOR_BITWISEOR) return 10;
        if (op == OPERATOR_AND) return 11;
        if (op == OPERATOR_OR) return 12;
        if (op == OPERATOR_EQUALS) return 8;
        return std::numeric_limits<int64_t>::max();
    }

    static bool leftAssociative(customString op) {
        if (op == OPERATOR_DIVISION || op == OPERATOR_MINUS || op == OPERATOR_MULTIPLICATION ||
            op == OPERATOR_PLUS || op == OPERATOR_MODULUS) return true;
        if (op == OPERATOR_POWER || op == OPERATOR_UNARY_MINUS || op == OPERATOR_EXP || op == OPERATOR_LOG) return false;
        return true; //default if to true if op not found because most operators are left associative.
    }

    static customStack<Token> convertToRPN(customStack<Token>& infix) {
        customStack<Token> rpn;
        customStack<Token> operators;

        for (size_t i = 0; i < infix.size(); ++i) {
            if (infix[i].type == SUBEXPRESSION_OPERATOR && infix[i].value == OPERATOR_LEFTBRACKET) {
                operators.push(infix[i]);
                continue;
            }

            if (infix[i].type == SUBEXPRESSION_OPERATOR && infix[i].value == OPERATOR_RIGHTBRACKET) {
                while (!(operators.peek().type == SUBEXPRESSION_OPERATOR && operators.peek().value == OPERATOR_LEFTBRACKET)) {
                    rpn.push(operators.pop());
                }
                operators.pop();
                continue;
            }

            if (infix[i].type != SUBEXPRESSION_OPERATOR) {
                rpn.push(infix[i]);
            }
            else {
                while (!operators.empty() && (
                        (leftAssociative(infix[i].value) && precedence(infix[i].value) >= precedence(operators.peek().value)) ||
                        (!leftAssociative(infix[i].value) && precedence(infix[i].value) > precedence(operators.peek().value)))) {
                    rpn.push(operators.pop());
                }
                operators.push(infix[i]);
            }
        }

        while (!operators.empty()) {
            if (!isBracket(operators.peek().value[0])) {
                rpn.push(operators.pop());
            }
            else {
                operators.pop();
            }
        }

        return rpn;
    }

    static customStack<Token> normalize(customStack<Token>& infix) {
        infix = convertMinuses(infix);

        customStack<Token> output;

        if (!(infix[0].type == SUBEXPRESSION_OPERATOR && infix[0].value == OPERATOR_PLUS)) {
            output.push(infix[0]);
        }

        for (int i = 1; i < infix.size(); ++i) {
            if (!(infix[i].type == SUBEXPRESSION_OPERATOR && infix[i].value == OPERATOR_PLUS &&
                  infix[i - 1].type == SUBEXPRESSION_OPERATOR)) {
                output.push(infix[i]);
            }
        }

        return output;
    }

    static customStack<Token> convertMinuses(customStack<Token>& infix) {
        customStack<Token> output;

        Token unary_minus;
        unary_minus.type = SUBEXPRESSION_OPERATOR;
        unary_minus.value = OPERATOR_UNARY_MINUS;

        if (infix[0].type == SUBEXPRESSION_OPERATOR && infix[0].value == OPERATOR_MINUS) {
            output.push(unary_minus);
        }
        else {
            output.push(infix[0]);
        }

        for (size_t i = 1; i < infix.size(); ++i) {
            if (infix[i].type == SUBEXPRESSION_OPERATOR && infix[i].value == OPERATOR_MINUS
                && infix[i - 1].type == SUBEXPRESSION_OPERATOR ) {
                output.push(unary_minus);
            }
            else {
                output.push(infix[i]);
            }
        }
        return output;
    }

    static customStack<Token> tokenize(customString& str) {
        customStack<Token> output;
        Token currToken = nullToken();

        for (char ch : str) {
            // Check if ch is enclosing string literal.
            if (ch == '"') {
                if (currToken.type == SUBEXPRESSION_STRING) {
                    output.push(currToken);

                    currToken = nullToken();
                }
                else if (currToken.type == SUBEXPRESSION_NIL || currToken.type == SUBEXPRESSION_OPERATOR) {
                    if(currToken.type != SUBEXPRESSION_NIL) output.push(currToken);

                    currToken.type = SUBEXPRESSION_STRING;
                    currToken.value = "";
                }
                    // Substrings like 123"", aaa"", or SELECT"" can never appear in valid requests.
                else { // Malformed expression.
                    std::cout << "Unexpected symbol " << ch << "\n";
                    return customStack<Token>();
                }
            }
                // Check if inside string literal.
            else if (currToken.type == SUBEXPRESSION_STRING) {
                currToken.value.append(ch);
            }
                // Check if ch is space character
            else if(isspace(ch)) {
                // Start new token if neccessary.
                if (currToken.type != SUBEXPRESSION_NIL) {
                    output.push(currToken);
                    currToken = nullToken();
                }
            }
                // Check if ch is acceptable character for a variable name.
            else if(isalpha(ch) || (currToken.type == SUBEXPRESSION_VARIABLE && (isalnum(ch) || ch == '_' || ch == '$'))) {
                if (currToken.type == SUBEXPRESSION_VARIABLE) {
                    currToken.value.append(ch);
                }
                else {
                    if (currToken.type != SUBEXPRESSION_NIL) output.push(currToken);
                    currToken.type = SUBEXPRESSION_VARIABLE;
                    currToken.value = "";
                    currToken.value.append(ch);
                }
            }
                // Check if ch is part of a constant number
            else if (isdigit(ch)) {
                if (currToken.type == SUBEXPRESSION_CONSTANT) {
                    currToken.value.append(ch);
                }
                else {
                    if(currToken.type != SUBEXPRESSION_NIL) output.push(currToken);
                    currToken.type = SUBEXPRESSION_CONSTANT;
                    currToken.value = "";
                    currToken.value.append(ch);
                }
            }
            else if (isOperator(ch)) {
                //Handle && and || operators.
                if (currToken.type == SUBEXPRESSION_OPERATOR && currToken.value.size() == 1 && currToken.value[0] == ch) {
                    currToken.value.append(ch);
                }
                else {
                    if (currToken.type != SUBEXPRESSION_NIL) output.push(currToken);
                    currToken.type = SUBEXPRESSION_OPERATOR;
                    currToken.value = "";
                    currToken.value.append(ch);
                }
            }
            else { // Malformed expression.
                std::cout << "Unexpected symbol " << ch << "\n";
                return customStack<Token>();
            }
        }

        if (currToken.type != SUBEXPRESSION_NIL) {
            output.push(currToken);
        }


        for(auto& i : output) {
            if(i.value == "EXP" && i.type == SUBEXPRESSION_VARIABLE) i.type = SUBEXPRESSION_OPERATOR;
            if(i.value == "LOG" && i.type == SUBEXPRESSION_VARIABLE) i.type = SUBEXPRESSION_OPERATOR;
        }

        return output;
    }
};