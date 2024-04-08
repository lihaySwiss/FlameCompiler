#pragma once
#include <string>

enum tokId{
    UNDEFINED = -999,
    ID_LITERAL = 0,
    ID_IDENTIFIER = 1,
    ID_NUMBER = 2,
    ID_IF_CONDITION = 4,
    ID_ELSE_CONDITION = 9,
    ID_WHILE_LOOP = 15,
    ID_FOR_LOOP = 19,
    ID_CHAR = 24,
    ID_INT = 27,
    ID_BOOL = 32,
    ID_VOID = 37,
    BINOP_DIV = 39,
    BINOP_PLUS = 41,
    BINOP_MINUS = 43,
    BINOP_MULT = 45,
    AND_OP = 47,
    LOGIC_AND = 51,
    LOGIC_OR = 54,
    EQUAL = 59,
    SMALLER = 61,
    BIGEER = 63,
    SMALLER_EQUAL = 65,
    BIGEER_EQUAL = 67,
    NOT_EQ = 70,
    DEFINE_VAR = 73,
    SEMI_COLON = 75,
    APOSTROPHE = 77,
    QUOTES_TOKEN = 79,
    COMMA_TOKEN = 81,
    LPARAN = 83,
    RPARAN = 85,
    LBRACE = 87,
    RBRACE = 89,
    ID_RETURN = 96,
    ID_TRUE = 99,
    ID_FALSE = 106,
    ID_GIVE = 111,
    ID_EOF = 113
}typedef tokId;

struct Token
{
    std::string token;
    int type;
    int loc;
} typedef Token;
