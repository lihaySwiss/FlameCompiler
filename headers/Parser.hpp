#pragma once
#include "Stack.hpp"
#include "Token.hpp"
#include "Production.hpp"
#include "types.hpp"
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
    VOID_ACTION,
    NUMBER_ACTION,
    LITERAL_ACTION,
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
    RETURN_ACTION,
    GIVE_ACTION,
    COMMA_ACTION,
    TRUE_ACTION,
    FALSE_ACTION,
    PRINT_ACTION,
    EOF_ACTION,
}typedef actionTableKeys;

const string terminal_words[] = {
    "FOR_ACTION", "LPARAN_ACTION", "SEMI_COLON_ACTION", "RPARAN_ACTION","LBRACE_ACTION", 
    "RBRACE_ACTION", "IF_ACTION", "ELSE_ACTION", "WHILE_ACTION","IDENTIFIER_ACTION", "ASSIGNMENT_ACTION", 
    "INT_ACTION", "BOOL_ACTION", "CHAR_ACTION", "VOID_ACTION", "NUMBER_ACTION", "LITERAL" ,"EQUAL_ACTION", 
    "NOTEQ_ACTION", "SMALLER_ACTION","BIGGER_ACTION", "SMALLER_EQUAL_ACTION", "BIGGER_EQUAL_ACTION",
    "MINUS_ACTION","PLUS_ACTION", "MULT_ACTION", "DIV_ACTION", "UNARY_PLUS_ACTION", "UNARY_MINUS_ACTION",
    "OR_ACTION", "AND_ACTION", "SHR_ACTION", "SHL_ACTION", "RETURN_ACTION", "GIVE_ACTION", "COMMA_ACTION", 
    "TRUE_ACTION", "FALSE_ACTION" ,"EOF_ACTION"};


class ParseTree{
public:
    ParseTree* root;
    int value;
    int size;
    Token token;
    std::vector<ParseTree> children;
    types type;

    ParseTree()
    {
        this->root = NULL; // create a new token
        this->children = {}; // assign an empty vector to children
        this->size = 0; // size of the tree
    }

    ParseTree(Token *root)
    {
        this->root = new ParseTree; // create a new token
        Token *newRoot = new Token(); // create a new token
        newRoot->token = root->token; // set the token value
        newRoot->type = root->type; // set the token type
        newRoot->loc = root->loc; // set the line number
        newRoot->type = root->type;
        this->root->token = *newRoot;
        this->children = {}; // children of the root
        this->size = 0; // size of the tree
    }

};

class Parser{
private:
    Stack<ParseTree> parStack;                              //stack to store the parse tree
    Stack<int> tokenStack;                                  //stack to store the tokens
    std::list<Token> inputList;                             //list of tokens
    std::vector<Production> productions;                    //list of productions
    std::vector<std::vector<std::string>> actionGoTable;    //action go table
    
    //to convert between token id and action table keys we can use a hashtable which maps token id to action table keys
    std::unordered_map<int, actionTableKeys> tokenActions;

public:
    /// @brief Constructor
    Parser();

    /// @brief Function to set the rule list
    void setRuleList();                     

    /// @brief Function to set the input list
    /// @param inputList
    void setInputList(list<Token> inputList){this->inputList = inputList;}
    
    /// @brief Function to read the productions from the file
    void generateParseTables();

    /// @brief Function to parse the input list
    ParseTree parse();

    /// @brief Function to print the parse tree
    void makeMap();
    
    /// @brief Function to print the parse tree
    /// @param root
    /// @param prefix
    /// @param isLast
    void printAST(ParseTree& root, string prefix, bool isLast);

};