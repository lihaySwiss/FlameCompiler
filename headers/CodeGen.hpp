#include "Parser.hpp"

struct RegisterEntry
{
    std::string name;
    bool isFree;
    RegisterEntry(std::string name, int isFree) : name(name), isFree(isFree) {}
};

class CodeGen
{
private:
std::vector<RegisterEntry*> registers;
    
public:
    ~CodeGen();
    void setUpRegisters();
};


