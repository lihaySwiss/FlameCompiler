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
        for(int i = 0; i < line.size(); i++)
        {
            if(line[i] == '~')
            {
                line = line.substr(0, i);
                break;
            }
        }
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
        if(data[i] == '\'' || data[i] == '\"')
        {
            result += data[i];
            i++;

            while(data[i] != '\'' && data[i] != '\"' && data[i] != '\0')
            {
                result += data[i];
                i++;
            }

            if(data[i] != '\'' && data[i] != '\"')
            {
                std::cout << "Lexing error on line: " << loc << " missing closing quote" << std::endl;
                t->type = UNDEFINED;
                t->token = ' ';
                t->loc = loc;
                tokenList.push_back(*t);
                return (*t);
            }
            result += data[i];
            i++;

            // the type will be literal
            t->type = ID_LITERAL;
            t->token = result;
            t->loc = loc;
            tokenList.push_back(*t);
            return (*t);
        }

        //the type will be the most recent valid state
        t->type = state;
        result += data[i]; 
        
        i++;
        state = mat[state][data[i]];
        
    }

    if(!isListedValue(t->type) && t->type != -1)
    {
        t->type = 1;
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
        case ID_LITERAL:
            return "LITERAL";

        case ID_INT:
            return "INT";

        case ID_IDENTIFIER:
            return "IDENTIFIER";

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


        case AND_OP:
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
bool Lexer::isListedValue(int numToCheck) {
  return numToCheck == UNDEFINED ||
         numToCheck == ID_LITERAL ||
         numToCheck == ID_IDENTIFIER ||
         numToCheck == ID_NUMBER ||
         numToCheck == ID_IF_CONDITION ||
         numToCheck == ID_ELSE_CONDITION ||
         numToCheck == ID_WHILE_LOOP ||
         numToCheck == ID_FOR_LOOP ||
         numToCheck == ID_CHAR ||
         numToCheck == ID_INT ||
         numToCheck == ID_BOOL ||
         numToCheck == ID_VOID ||
         numToCheck == BINOP_DIV ||
         numToCheck == BINOP_PLUS ||
         numToCheck == BINOP_MINUS ||
         numToCheck == BINOP_MULT ||
         numToCheck == AND_OP ||
         numToCheck == LOGIC_AND ||
         numToCheck == LOGIC_OR ||
         numToCheck == EQUAL ||
         numToCheck == SMALLER ||
         numToCheck == BIGEER ||
         numToCheck == SMALLER_EQUAL ||
         numToCheck == BIGEER_EQUAL ||
         numToCheck == NOT_EQ ||
         numToCheck == DEFINE_VAR ||
         numToCheck == SEMI_COLON ||
         numToCheck == APOSTROPHE ||
         numToCheck == QUOTES_TOKEN ||
         numToCheck == COMMA_TOKEN ||
         numToCheck == LPARAN ||
         numToCheck == RPARAN ||
         numToCheck == LBRACE ||
         numToCheck == RBRACE ||
         numToCheck == ID_RETURN ||
         numToCheck == ID_TRUE ||
         numToCheck == ID_FALSE ||
         numToCheck == ID_GIVE ||
         numToCheck == ID_PRINT ||
         numToCheck == ID_EOF;
}