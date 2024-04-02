#include "../headers/Lexer.hpp"
#include "../headers/DFA.hpp"
#include "../headers/Parser.hpp"

int main(int argc, char const * argv[]) {
    
    if(argc < 2){
        std::cout << "Usage: ./ace <path_to_.ace_file>" << std::endl;
        return 1;
    }

    //std::string path = "../tests/testFail.ace";
    std::string path = argv[1];

    
    Lexer *lexer = new Lexer(path);
    lexer->readFromFile();

    Parser *parser = new Parser();
    ParseTree parseTree;

    parser->setInputList(lexer->getTokenList());
    parseTree = parser->parse();
    parser->printAST(parseTree, "", true);

    return 0;
}