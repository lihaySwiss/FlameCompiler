#pragma once
#include <vector>
#include <iostream> 
#include "Lexer.hpp"

enum non_terminal{
    PROGRAM = 32,
    STATEMENT,
    FOR_STATEMENT,
    IF_STATEMENT,
    WHILE_STATEMENT,
    DECELERATION_STATEMENT,
    ASSIGNMENT,
    TYPE,
    EXPRESSION,
    FACTOR,
    CONDITION,
    RELATIONAL_OPERATOR,
    AROP,
    UNARY_EXPRESSION,
    BITOP
};


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