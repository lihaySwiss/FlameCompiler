#pragma once
#include <iostream>
#include <fstream>

#define CHARS_NUM 128
#define NUM_OF_STATES 128

class DFA
{
private:
    int mat[NUM_OF_STATES][CHARS_NUM];

public:
    // DFA handle
    DFA();

    /// @brief Function to make the DFA
    void makeDFA();

    /// @brief Function to add tokens to the DFA
    /// @param token 
    void addTokens(const std::string &token);

    // file handle

    /// @brief Function to write the DFA to a file
    /// @param path 
    /// @return bool value to indicate the success of the operation
    bool writeToFile(const std::string &path);
    
    /// @brief Function to read the DFA from a file
    /// @param path
    /// @return bool value to indicate the success of the operation
    bool readFromFile(const std::string &path);

    /// @brief Function to get the DFA matrix
    /// @return int(*)[CHARS_NUM] 
    int (*getMat())[CHARS_NUM] { return this->mat; }
};
