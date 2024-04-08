#pragma once
#include <vector>
#include <iostream> 
#include "Lexer.hpp"

#define NT_OFFSET 37

enum non_terminal{
    PROGRAM = NT_OFFSET,
    STATEMENT_LIST,
    STATEMENT,
    FOR_STATEMENT,
    IF_STATEMENT,
    WHILE_STATEMENT,
    DECELERATION_STATEMENT,
    ASSIGNMENT,
    TYPE,
    VOID_TYPE,
    EXPRESSION,
    FACTOR,
    CONDITION,
    RELATIONAL_OPERATOR,
    AROP,
    UNARY_EXPRESSION,
    BITOP,
    RETURN_STATEMENT,
    FUNCTION,
    INPUT_VAR_LIST,
    INPUT_VAR,
};

const string non_terminal_words[] = { 
    "PROGRAM", "STATEMENT_LIST", "STATEMENT", "FOR_STATEMENT", "IF_STATEMENT",
    "WHILE_STATEMENT", "DECELERATION_STATEMENT","ASSIGNMENT","TYPE","EXPRESSION",
    "FACTOR","CONDITION","RELATIONAL_OPERATOR","AROP","UNARY_EXPRESSION","BITOP", 
    "RETURN_STATEMENT", "FUNCTION", "INPUT_VAR_LIST", "INPUT_VAR"};

//example: production for type 
// left-> Token(NON-TERMINAL) right -> (Token(ID_INT))
// left-> Token(NON-TERMINAL) right -> (Token(ID_CHAR))
// left-> Token(NON-TERMINAL) right -> (Token(ID_BOOL))

class Production{
    public:
    Production(){};
    non_terminal left;
    std::vector<int> right;
    
    Production(non_terminal left, std::vector<int> right){
        this->left = left;
        this->right = right;
    }
};