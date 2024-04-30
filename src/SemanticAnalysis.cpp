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
Token token = tree->token; // Get the token of the tree
    if (token.type == DECELERATION_STATEMENT)
    {
        string type = tree->children.at(0).children.at(0).token.token; // Get the type of the variable
        int typeCode = mapOfTypes[type];                                // Get the type code of the variable
        string identifier = tree->children.at(1).token.token;                          // Get the identifier of the variable
        if (symbolTable.find(Symbol(identifier, 0, 0, 0)) != symbolTable.end()) // Check if the variable is already declared
        {
            std::cerr << "Variable " + identifier + " already declared, line: " << tree->children.at(0).token.loc << std::endl;
        }
        symbolTable.insert(Symbol(identifier, typeCode, scope, tree->children.at(1).token.loc)); // Insert the variable into the symbol table
    }
}

void SemanticAnalysis::printSymbolTable()
{
    cout << "Symbol Table" << endl;
    cout << "------------" << endl;
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

ParseTree *SemanticAnalysis::semantic()
{
    semanticHelper(parseTree, 0); // Call the symantic helper function to perform symantic analysis
    return parseTree;
}

ParseTree *SemanticAnalysis::semanticHelper(ParseTree *tree, int scope)
{
    if (tree == NULL) // Empty tree
    {
        return tree;
    }
    for (int i = 0; i < tree->children.size(); i++) // Iterate through the children of the tree
    {
        if (tree->children.at(i).token.type == LBRACE) // Increment scope when entering a new block
            scope++;
        semanticHelper(&tree->children.at(i), scope); // Recursively perform symantic analysis on the children
    }
    
    Token* token = &tree->token;
    if (token->type == ID_IDENTIFIER && tree->root->token.type != FUNCTION && tree->root->token.type)    
    {
        auto symbol = symbolTable.find(Symbol(token->token, 0, 0, 0));                          // Find the identifier in the symbol table
        if (symbol == symbolTable.end() || symbol->scope > scope || symbol->line > token->loc) // Check if the identifier is undeclared
        {
            std::cerr << "Undeclared variable " + token->token + " in line: " << token->loc << std::endl;
        }
        else
        {
            token->type = symbol->type; // Set the type code of the identifier
        }
    }
    else if (token->type == LITERAL_ACTION)
    {
        token->type = identifyType(token); // Identify the type of the literal
    }
    else if (token->type == FACTOR || token->type == EXPRESSION)
    {
        if (tree->children.size() == 1 || token->type == EXPRESSION)        // Set the type code of the term or expression
            token->type = tree->children.at(0).token.type;
        else
            token->type = tree->children.at(1).token.type;       // Set the type code of the term or expression
    }
    else if (token->type == ASSIGNMENT)
    {
        if (tree->children.at(0).token.type != tree->children.at(2).token.type) // Check for type errors in the assignment
        {
            std::cerr << "Invalid Type Error: Cannot assign " + 
            getSymbolType(tree->children.at(2).token.type) + " to identifer " + tree->children.at(0).token.token + " (" +
            getSymbolType(tree->children.at(0).token.type) + ")" + " in line: " << tree->children.at(0).token.loc << std::endl;
        }
    }

    else if (token->type == DECELERATION_STATEMENT)
    {
        if (tree->children.size() == 4 && mapOfTypes[tree->children.at(0).children.at(0).token.token] != tree->children.at(3).token.type) // Check for type errors in the assignment
        {
            std::cerr << "Invalid Type Error: Cannot assign " + 
            getSymbolType(tree->children.at(3).token.type) + " to identifer " + tree->children.at(1).token.token + " (" <<
            getSymbolType(mapOfTypes[tree->children.at(0).children.at(0).token.token]) << ") in line: " << tree->children.at(1).token.loc << std::endl;
        }
    }
    else if(token->type == FUNCTION)
    {
        if(tree->children.size() == 10) // Check if the function has a return statement
        {
            if(tree->children.at(8).children.size() < 2) // Check if the function has a return type
            {
                std::cerr << "Function " + tree->children.at(0).token.token + " has no return type in line: " << tree->children.at(0).token.loc << std::endl;
            }
            if (mapOfTypes[tree->children.at(1).children.at(0).token.token] != tree->children.at(8).token.type) // Check if the return type of the function is the same as the return statement
            {
            std::cerr << "Invalid Type Error: Cannot return " + 
            getSymbolType(tree->children.at(8).token.type) + " from function with return type " + 
            getSymbolType(mapOfTypes[tree->children.at(1).children.at(0).token.token]) + " in line: " << tree->children.at(8).token.loc << std::endl;
            }
        }
    }
   
    else if (token->type == CONDITION) // Check if the token is a comparison operator
    {
        if (tree->children.at(0).token.type != tree->children.at(2).token.type) // Check if the operands of the comparison operator are of the same type
        {
            std::cerr << "Invalid Type Error: Cannot compare " + 
            getSymbolType(tree->children.at(0).token.type) + " with " + getSymbolType(tree->children.at(2).token.type) + " in line: " << tree->children.at(2).token.loc << std::endl;
        }
        token->type = BOOL_ACTION; // Set the type code of the comparison
    }
    else if (token->type == RETURN_STATEMENT)
    {
        if(tree->children.size() < 2) // Check if the return statement is valid
        {
            std::cerr << "Invalid Return Statement in line: " << tree->token.loc << std::endl;
        }
        tree->token.type = tree->children.at(1).token.type; // Set the type code of the return statement
    }
    

    //tree->root->token = *token; // Set the root of the tree
    return tree;
}


string SemanticAnalysis::getSymbolType(int type)
{
    switch (type)
    {
    case CHAR:
        return "char";
    case INT:
        return "int";
    case BOOL:
        return "bool";
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
        if(literal[3] != '\'')
        {
            std::cerr << "No closing apostrophe for char: " + literal + " in line: " <<  token->loc << std::endl;
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

