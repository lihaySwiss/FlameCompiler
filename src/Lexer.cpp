#include "../headers/Lexer.hpp"

int LOC = 1;

Lexer::Lexer(std::string path)
{
    this->path = path;
    this->dfa = new DFA();
}

void Lexer::readFromFile()
{
    int i = 0;
    Token tok;
    std::string skip;
    std::string data, line;
    ifstream file;

    if (!file.is_open())
        file.open(this->path);

    if (!file.is_open())
    {
        std::cout << "error opening code file" << std::endl;
        exit(1);
    }

    while (std::getline(file, line)) {
        data += line + "\n"; // Append each line to the string
    }
    
    // Analyizing each word recived from the file
    // word is a sequence of charecters whice ends with a space
    for (i = 0; i < data.size();)
    {
        if(data[i] == '\n')
        {
            LOC++;
            data[i] = '\0';
        }
        tok = analyze(data.substr(i), LOC);
        i += tok.token.size();
    }

    //Printing results for debugging
    // for (Token token : this->tokenList)
    // {
    //     std::cout << token.token << " ";
    //     std::cout << returnTokenString(token.type) << " ";
    //     std::cout << to_string(token.type) << std::endl;
    // }
}

Token Lexer::analyze(std::string data, int loc)
{
    int i = 0, tempNext = 0;
    std::string result = "";
    Token *t = new Token();

    int(*mat)[CHARS_NUM] = this->dfa->getMat();

    int state = mat[0][data[0]];

    // Getting the token by stat from the matrix
    // if no matching state and finel state is posative identify as identifier
    // else identify as UNDEFINED
    while(state != -1)
    {   
        // the type will be the most recent valid state
        t->type = state;
        result += data[i]; 
        
        i++;
        state = mat[state][data[i]];
        
    }

    t->token = result;

    if(t->token.size() > 0)
    {
        t->loc = loc;
        tokenList.push_back(*t);
    }
    else{
        t->token += ' ';
    }

    return (*t);
}

// Debugging that returns words for each code recived
std::string Lexer::returnTokenString(int code)
{
    switch (code)
    {
        case ID_INT:
            return "INT";


        case ID_NUMBER:
            return "NUMBER";

        case ID_CHAR:
            return "CHAR";

        case ID_BOOL:
            return "BOOL";

        case ID_VOID:
            return "VOID";

        case ID_IF_CONDITION:
            return "IF_CONDITION";


        case ID_ELSE_CONDITION:
            return "ELSE_CONDITION";


        case ID_WHILE_LOOP:
            return "WHILE_LOOP";

        case ID_FOR_LOOP:
            return "FOR_LOOP";

        case EQUAL:
            return "EQUAL";


        case BIGEER_EQUAL:
            return "BIGEER_EQUAL";


        case SMALLER_EQUAL:
            return "SMALLER_EQUAL";


        case BINOP_PLUS:
            return "BINOP_PLUS";


        case BINOP_DIV:
            return "BINOP_DIV";


        case BINOP_MINUS:
            return "BINOP_MINUS";


        case BINOP_MULT:
            return "BINOP_MULT";


        case BINOP_AND:
            return "BINOP_AND";


        case LOGIC_AND:
            return "LOGIC_AND";


        case LOGIC_OR:
            return "LOGIC_OR";


        case DEFINE_VAR:
            return "DEFINE_VAR";


        case LPARAN:
            return "LEFT_PARAN";


        case RPARAN:
            return "RIGHT_PARAN";

        case RBRACE:
            return "RIGHT_BRACE";

        case LBRACE:
            return "LEFT_BRACE";

        case ID_RETURN:
            return "ID_RETURN";


        case -1:
            return "NO SUCH TOKEN";


        default:
            return "IDENTIFIER";

    }
}