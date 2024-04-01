#pragma once
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <list>
#include "Token.hpp"
#include "DFA.hpp"

#define COMMENT_SIGN '#'
#define QUOTES '\''
#define DOUBLE_QUOTES '\"'

using namespace std;

class Lexer
{

private:
    std::string _lastCollectedValue;
    list<Token> tokenList;
    std::string path;
    DFA *dfa;
    
public:
    
    Lexer(std::string path);

    void readFromFile();

    std::string returnTokenString(int code);

    Token analyze(std::string currWord);

    list<Token> getTokenList() { return this->tokenList; }
};
