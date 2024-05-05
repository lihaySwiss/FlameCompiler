#pragma once
#include "Symbol.hpp"
#include <unordered_set>

class SemanticAnalysis
{
private:
    unordered_set<Symbol, Symbol::HashFunction> symbolTable;    // Symbol table
    ParseTree* parseTree;                                       // Parse tree
    bool errors = false;                                        // Flag to indicate if there are any errors

    unordered_map<string, int> mapOfTypes = {
    {"int", INT},
    {"bool", BOOL},
    {"char", CHAR},};

public:
    /// @brief Constructor
    /// @param parseTree 
    SemanticAnalysis(ParseTree* parseTree);
    
    /// @brief Function to create the symbol table
    /// @param parseTree 
    /// @param scope 
    void createSymbolTable(ParseTree* parseTree, int scope);
    
    /// @brief getter for the symbol table
    /// @return std::unordered_set<Symbol, Symbol::HashFunction>
    unordered_set<Symbol, Symbol::HashFunction> getSymbolTable();
    
    /// @brief Functions to perform semantic analysis
    ///  @param tree
    /// @param scope
    /// @return ParseTree*
    ParseTree* semantic(ParseTree *tree, int scope);
    
    /// @brief Function to get the type of the symbol
    /// @param type 
    /// @return 
    string getSymbolType(int type);
    
    /// @brief Function to check if the token is a comparison operator
    /// @param token 
    /// @return bool value to indicate if the token is a comparison operator
    bool isCmpOp(string token);
    
    /// @brief Function to identify the type of the token
    /// @param token 
    /// @return type of the token
    int identifyType(Token *token);

    /// @brief Function to check if there are any errors in the code
    /// @return bool value to indicate if there are any errors
    bool getErrors() { return errors; }

    /// @brief Function to print the symbol table
    void printSymbolTable();
};