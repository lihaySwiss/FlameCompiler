#pragma once
#include "Parser.hpp"
#include <stdio.h>
#include <unordered_set> 

class Symbol
{
public:
    string name;    // Name of the symbol
    int type;      // Type of the symbol
    int scope;    // Scope of the symbol
    int line;    // Line of the symbol

    /// @brief  Function to compare two symbols
    /// @param other 
    /// @return 
    bool operator==(const Symbol &other) const;
    Symbol(string name, int type, int scope, int line);

    // hash function for the symbol table
    struct HashFunction
    {
        size_t operator()(const Symbol &symbol) const
        {
            return hash<string>()(symbol.name);
        }
    };

};