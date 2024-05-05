#pragma once
#include "Parser.hpp"
#include "Symbol.hpp"
#include "RegisterEntry.hpp"
#include <iostream>

class CodeGen
{
private:
    int labelCount = 0;         // Counter for labels
    int whileLabels = 0;        // Counter for while labels
    int ifLabels = 0;           // Counter for if labels
    int forLabels = 0;          // Counter for for labels
    int dataCount = 0;          // Counter for data
    vector<string> code;    // Vector to store the generated code
    vector<string> data;    // Vector to store the data
    std::string fileName;   // Name of the file
    unordered_set<Symbol, Symbol::HashFunction> symbolTable;    // Symbol table  
    std::vector<RegisterEntry> registers = {    
    {"eax", true},{"ebx", true},{"ecx", true},
    {"edx", true},{"edi", true},{"esi", true},};    // Vector to store the registers
    
public:

    /// @brief Constructor
    /// @param tree 
    CodeGen(std::string fileName,  unordered_set<Symbol, Symbol::HashFunction> symbolTable)
    {this->fileName = fileName; this->symbolTable = symbolTable; labelCount = 0;};
    ~CodeGen();

    /// @brief Function to generate the code
    bool generate(ParseTree *tree);

    /// @brief Function to exacute the nasm code
    /// @param outputFileName
    void exacuteNasm(string outputFileName);

    /// @brief Function to create the base assembly code
    /// @param file
    void createBaseAsm(ofstream &file);

    /// @brief Function to generate the identifiers
    /// @param file
    void generateIdentifiers(ofstream &file);

    /// @brief Function to generate the code from the AST
    /// @param tree
    void generateCodeFromAST(ParseTree *tree);

    /// @brief Function to generate the code for the statements
    /// @param tree
    /// @return RegisterEntry
    RegisterEntry generateExpression(ParseTree *expression);

    /// @brief Function to generate the end of the code
    /// @param file
    void generateExit(ofstream &file);

    /// @brief Function to generate the code for the statements
    /// @param tree
    /// @return RegisterEntry
    string convert32to8(RegisterEntry reg, int byte);

    /// @brief  Function to generate the code for the statements
    /// @param regs 
    /// @param n 
    RegisterEntry getRegister();

    /// @brief Function to free the register
    /// @param reg
    void freeRegister(RegisterEntry reg);

    /// @brief Function to set the register state
    /// @param reg
    /// @param state
    void setRegState(RegisterEntry reg, bool state);

    /// @brief Function to push the registers
    /// @param regs
    /// @param n
    void pushRegs(string *regs, int n);

    /// @brief Function to pop the registers
    /// @param regs
    /// @param n
    void popRegs(string *regs, int n);

    /// @brief Function to add the code to the vector
    /// @param code
    void addCode(string code);

    /// @brief Function to add the data to the vector
    /// @param data
    void addData(string data);
};


