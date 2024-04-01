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
    void makeDFA();
    void addTokens(const std::string &token);

    // file handle
    bool writeToFile(const std::string &path);
    bool readFromFile(const std::string &path);

    int (*getMat())[CHARS_NUM] { return this->mat; }
};
