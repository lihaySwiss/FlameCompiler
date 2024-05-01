#pragma once
#include <iostream>

/// @brief Class to represent a register entry
struct RegisterEntry
{
    std::string name;
    bool isFree;
    RegisterEntry(std::string name, int isFree) : name(name), isFree(isFree) {}
    RegisterEntry() : name(""), isFree(true) {}
};