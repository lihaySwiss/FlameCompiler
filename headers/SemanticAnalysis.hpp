#pragma once
#include "Symbol.hpp"
#include <unordered_set>

class SemanticAnalysis
{
private:
    unordered_set<Symbol, Symbol::HashFunction> symbolTable;
    ParseTree* parseTree;

    unordered_map<string, int> mapOfTypes = {
    {"int", INT},
    {"bool", BOOL},
    {"char", CHAR},};

public:
    SemanticAnalysis(ParseTree* parseTree);
    void createSymbolTable(ParseTree* parseTree, int scope);
    unordered_set<Symbol, Symbol::HashFunction> getSymbolTable();
    ParseTree* semantic(ParseTree* parseTree, int scope);
    string getSymbolType(int type);
    bool isCmpOp(string token);
    int identifyType(Token *token);
    void printSymbolTable();
};