#pragma once
#include <vector>
#include <iostream> 
#include "Lexer.hpp"

#define NT_OFFSET 39

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
    BOOL_TYPE,
};

const string non_terminal_words[] = { 
    "PROGRAM", "STATEMENT_LIST", "STATEMENT", "FOR_STATEMENT", "IF_STATEMENT",
    "WHILE_STATEMENT", "DECELERATION_STATEMENT","ASSIGNMENT","TYPE","VOID_TYPE","EXPRESSION",
    "FACTOR","CONDITION","RELATIONAL_OPERATOR","AROP","UNARY_EXPRESSION","BITOP", 
    "RETURN_STATEMENT", "FUNCTION", "INPUT_VAR_LIST", "INPUT_VAR", "BOOL_TYPE"};

//example: production for type 
// left-> Token(NON-TERMINAL) right -> (Token(ID_INT))
// left-> Token(NON-TERMINAL) right -> (Token(ID_CHAR))
// left-> Token(NON-TERMINAL) right -> (Token(ID_BOOL))

class Production{
    public:
    Production(){};         //default constructor
    non_terminal left;      //non-terminal
    std::vector<int> right; //vector of terminals
    
    /// @brief Constructor for Production
    /// @param left 
    /// @param right 
    Production(non_terminal left, std::vector<int> right){
        this->left = left;
        this->right = right;
    }
};