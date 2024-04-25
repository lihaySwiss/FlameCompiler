#pragma once
#include "Parser.hpp"
#include <stdio.h>
#include <unordered_set> 

class Symbol
{
public:
    string name;
    int type;
    int scope;
    int line;

    bool operator==(const Symbol &other) const;
    Symbol(string name, int type, int scope, int line);

    struct HashFunction
    {
        size_t operator()(const Symbol &symbol) const
        {
            return hash<string>()(symbol.name);
        }
    };

};