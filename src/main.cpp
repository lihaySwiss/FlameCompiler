#include "../headers/Lexer.hpp"
#include "../headers/DFA.hpp"
#include "../headers/Parser.hpp"
#include "../headers/SemanticAnalysis.hpp"
#include "../headers/CodeGen.hpp"

int main(int argc, char const * argv[]) {
    
    if(argc < 2){
        std::cout << "Usage: ./ace <path_to_.ace_file>" << std::endl;
        //return 1;
    }

    //std::string path = "../tests/errorTypesTests/test2.ace";
    std::string path = argv[1];

    Lexer *lexer = new Lexer(path);
    lexer->readFromFile();

    Parser *parser = new Parser();
    ParseTree parseTree;

    parser->setInputList(lexer->getTokenList());
    parseTree = parser->parse();
    //parser->printAST(parseTree, "", true);

    SemanticAnalysis *semanticAnalysis = new SemanticAnalysis(&parseTree);
    semanticAnalysis->semantic(&parseTree, 0);
    //semanticAnalysis->printSymbolTable();
    if(semanticAnalysis->getErrors()){
        std::cout << "Semantic Errors detected in the code, please fix." << std::endl;
        return 1;
    }

    CodeGen *codeGen = new CodeGen(path, semanticAnalysis->getSymbolTable());
    codeGen->generate(&parseTree);

    return 0;
}