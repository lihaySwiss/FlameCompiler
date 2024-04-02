#pragma once
#include "Stack.hpp"
#include "Token.hpp"
#include "Production.hpp"
#include <vector>
#include <fstream>
#include <sstream>
#include <unordered_map>


enum actionTableKeys{
    FOR_ACTION = 0,
    LPARAN_ACTION,
    SEMI_COLON_ACTION,
    RPARAN_ACTION,
    LBRACE_ACTION,
    RBRACE_ACTION,
    IF_ACTION,
    ELSE_ACTION,
    WHILE_ACTION,
    IDENTIFIER_ACTION,
    ASSIGNMENT_ACTION,
    INT_ACTION,
    BOOL_ACTION,
    CHAR_ACTION,
    NUMBER_ACTION,
    EQUAL_ACTION,
    NOTEQ_ACTION,
    SMALLER_ACTION,
    BIGGER_ACTION,
    SMALLER_EQUAL_ACTION,
    BIGGER_EQUAL_ACTION,
    MINUS_ACTION,
    PLUS_ACTION,
    MULT_ACTION,
    DIV_ACTION,
    UNARY_PLUS_ACTION,
    UNARY_MINUS_ACTION,
    OR_ACTION,
    AND_ACTION,
    SHR_ACTION,
    SHL_ACTION,
    EOF_ACTION,
}typedef actionTableKeys;

const string terminal_words[] = {
    "FOR", "(", ";", ")", "{", "}", "IF", "ELSE", "WHILE", "IDENTIFIER", "ASSIGNMENT", "INT", 
    "BOOL", "CHAR", "NUMBER", "EQUAL", "NOTEQ", "SMALLER", "BIGGER", "SMALLER_EQUAL","BIGGER_EQUAL", 
    "MINUS", "PLUS", "MULT", "DIV", "UNARY_PLUS", "UNARY_MINUS", "OR", "AND", "SHR", "SHL", "EOF"};


struct ParseTree{
    int value;
    std::vector<ParseTree> children;
}typedef ParseTree;

class Parser{
private:
    Stack<ParseTree> ASTack;
    Stack<int> tokenStack;
    std::list<Token> inputList;
    std::vector<Production> productions;
    std::vector<std::vector<std::string>> actionGoTable;
    
    //to convert between token id and action table keys we can use a hashtable which maps token id to action table keys
    std::unordered_map<int, actionTableKeys> tokenActions;



public:
    Parser();
    void setInputList(list<Token> inputList){this->inputList = inputList;}
    void generateParseTables();
    ParseTree parse();
    void makeMap();
    void printAST(ParseTree& root, string prefix, bool isLast);

};