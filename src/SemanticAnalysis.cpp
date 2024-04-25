#include "../headers/SemanticAnalysis.hpp"

SemanticAnalysis::SemanticAnalysis(ParseTree* root)
{
    this->parseTree = root;
    this->symbolTable = unordered_set<Symbol, Symbol::HashFunction>();
    this->createSymbolTable(parseTree, 0);
}


void SemanticAnalysis::createSymbolTable(ParseTree *tree, int scope)
{
if (tree == NULL) // Empty tree
{
    return;
}
// Iterate through the children of the tree
for (int i = 0; i < tree->children.size(); i++)
{
    if (tree->children.at(i).token.type == LBRACE) // Increment scope when entering a new block
        scope++;

    createSymbolTable(&tree->children.at(i), scope); // Recursively create symbol table for the children
}
Token* token = &tree->root->token; // Get the root token of the tree
    if ((*token).type == DECELERATION_STATEMENT)
    {
        string type = tree->children.at(0).root->token.token; // Get the type of the variable
        int typeCode = mapOfTypes[type];                                // Get the type code of the variable
        string identifier = tree->children.at(1).root->token.token;                          // Get the identifier of the variable
        if (symbolTable.find(Symbol(identifier, 0, 0, 0)) != symbolTable.end()) // Check if the variable is already declared
        {
            std::cerr << "Variable " + identifier + " already declared in line: " << tree->children.at(0).token.loc << std::endl;
        }
        symbolTable.insert(Symbol(identifier, typeCode, scope, tree->children.at(1).token.loc)); // Insert the variable into the symbol table
        printSymbolTable();
    }
}

void SemanticAnalysis::printSymbolTable()
{
    cout << "Symbol Table" << endl;
    cout << "Identifier\tType\tScope" << endl;
    for (auto symbol : this->symbolTable)
    {
        cout << symbol.name << "\t\t" << symbol.type << "\t" << symbol.scope << endl;
    }
}

unordered_set<Symbol, Symbol::HashFunction> SemanticAnalysis::getSymbolTable()
{
    return this->symbolTable;
}

ParseTree *SemanticAnalysis::semantic(ParseTree *tree, int scope)
{
if (tree->children.empty()) // Empty tree
    {
        return tree;
    }
    for (int i = 0; i < tree->children.size(); i++) // Iterate through the children of the tree
    {
        if (tree->children.at(i).token.type == LBRACE) // Increment scope when entering a new block
            scope++;
        semantic(&tree->children.at(i), scope); // Recursively perform symantic analysis on the children
    }
    Token token = tree->token;
    if (token.type == ID_IDENTIFIER)
    {
        auto symbol = symbolTable.find(Symbol(non_terminal_words[tree->value], 0, 0, 0));                          // Find the identifier in the symbol table
        if (symbol == symbolTable.end() || symbol->scope > scope || symbol->line > token.loc) // Check if the identifier is undeclared
        {
            std::cerr << "Undeclared variable " + token.token + " in line: " << token.loc << std::endl;
        }
        token.type = symbol->type; // Set the type code of the identifier
    }
    else if (token.type == LITERAL)
    {
        token.type = identifyType(&token); // Identify the type of the literal
    }
    else if (token.type == FACTOR || token.type == EXPRESSION)
    {
        if (tree->size == 1 || token.type == EXPRESSION)        // Set the type code of the term or expression
            token.type = tree->children.at(0).root->type;        // Set the type code of the term or expression
        else
            token.type = tree->children.at(1).root->type;       // Set the type code of the term or expression
    }
    else if (token.type == ASSIGNMENT)
    {
        if (tree->children.at(0).root != tree->children.at(2).root) // Check for type errors in the assignment
        {
            std::cerr << "Invalid Type Error: Cannot assign " + 
            getSymbolType(tree->children.at(2).root->type) + " to identifer " + tree->children.at(0).token.token + " (" +
            getSymbolType(tree->children.at(0).root->type) + ")" + " in line: " << tree->children.at(0).token.loc << std::endl;
        }
    }
    if (token.type == EXPRESSION && tree->children.at(0).root->value == EXPRESSION) // Check for type errors in the expression
    {
        if (tree->children.at(0).root != tree->children.at(2).root) // Check for type errors in the expression
        {
            std::cerr << "Invalid Type Error: Cannot perform operation on " + 
            getSymbolType(tree->children.at(0).root->type) + " and " + 
            getSymbolType(tree->children.at(2).root->type) + " in line: " << tree->children.at(1).token.loc << std::endl;
        }
        if (isCmpOp(tree->children.at(1).token.token)) // Check if the expression is a comparison operator
        {
            token.type = INT; // Set the type code of the expression
        }
        else
            token.type = tree->children.at(0).root->type;  // Set the type code of the expression
    }
    tree->token = token; // Set the root of the tree
    return tree;
}


string SemanticAnalysis::getSymbolType(int type)
{
    switch (type)
    {
    case INT:
        return "int";
    case BOOL:
        return "bool";
    case CHAR:
        return "char";
    default:
        return "unknown";
    }
}


bool SemanticAnalysis::isCmpOp(string token)
{
    return token == "==" || token == "!=" || token == "<" || token == ">" || token == "<=" || token == ">=" || token == "&&" || token == "||";
}

int SemanticAnalysis::identifyType(Token *token)
{
    string literal = token->token;
    if (literal[0] == '\'')
    {
        // Check if the char literal is valid
        if (literal.size() != 3)
        {
            std::cerr << "Invalid char literal " + literal + " in line: " <<  token->loc << std::endl;
        }
        return CHAR;
    }
    else if (literal == "true" || literal == "false")
    {
        return BOOL;
    }
    else
    {
        return INT;
    }
}

