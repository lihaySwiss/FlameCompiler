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
    std::string _lastCollectedValue; // Last collected value
    list<Token> tokenList;         // List of tokens
    std::string path;           // Path to the file
    DFA *dfa;                // Deterministic Finite Automata

public:
    // Constructor
    Lexer(std::string path); 

    // function to read DFA from file
    void readFromFile();

    /// @brief Function to return the string of the token
    /// @param code 
    /// @return 
    std::string returnTokenString(int code);

    /// @brief  Function to analyze the current word
    /// @param currWord 
    /// @param loc line of code 
    /// @return Token
    Token analyze(std::string currWord, int loc);

    /// @brief  Function to get the token list
    /// @return  list<Token>
    list<Token> getTokenList() { return this->tokenList; }

    bool isListedValue(int numToCheck);

};
