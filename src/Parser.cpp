#include "../headers/Parser.hpp"

Parser::Parser()
{
    setRuleList();
    generateParseTables();
    makeMap();
}

void Parser::setRuleList()
{
    //definig program
    productions.push_back(Production(PROGRAM, {STATEMENT_LIST}));

    //defining statement list
    productions.push_back(Production(STATEMENT_LIST, {STATEMENT}));
    productions.push_back(Production(STATEMENT_LIST, {STATEMENT_LIST, STATEMENT}));

    //defining statements
    productions.push_back(Production(STATEMENT, {IF_STATEMENT}));
    productions.push_back(Production(STATEMENT, {WHILE_STATEMENT}));
    productions.push_back(Production(STATEMENT, {ASSIGNMENT}));
    productions.push_back(Production(STATEMENT, {DECELERATION_STATEMENT}));
    productions.push_back(Production(STATEMENT, {FOR_STATEMENT}));
    productions.push_back(Production(STATEMENT, {EXPRESSION}));
    productions.push_back(Production(STATEMENT, {RETURN_STATEMENT}));
    productions.push_back(Production(STATEMENT, {FUNCTION}));
    productions.push_back(Production(STATEMENT, {PRINT_STATEMENT}));

    //defining loops and conditions
    productions.push_back(Production(FOR_STATEMENT, {FOR_ACTION, LPARAN_ACTION, ASSIGNMENT, SEMI_COLON_ACTION, CONDITION, SEMI_COLON_ACTION, EXPRESSION, RPARAN_ACTION, LBRACE_ACTION, STATEMENT_LIST, RBRACE_ACTION}));
    productions.push_back(Production(IF_STATEMENT, {IF_ACTION, LPARAN_ACTION, CONDITION, RPARAN_ACTION, LBRACE_ACTION, STATEMENT_LIST, RBRACE_ACTION}));
    productions.push_back(Production(IF_STATEMENT, {IF_ACTION, LPARAN_ACTION, CONDITION, RPARAN_ACTION, LBRACE_ACTION,  STATEMENT_LIST, RBRACE_ACTION, ELSE_ACTION, LBRACE_ACTION, STATEMENT_LIST, RBRACE_ACTION}));
    productions.push_back(Production(WHILE_STATEMENT, {WHILE_ACTION, LPARAN_ACTION, CONDITION, RPARAN_ACTION, LBRACE_ACTION, STATEMENT_LIST, RBRACE_ACTION}));
    
    //defining declarations
    productions.push_back(Production(DECELERATION_STATEMENT, {TYPE, IDENTIFIER_ACTION, ASSIGNMENT_ACTION, EXPRESSION}));
    productions.push_back(Production(DECELERATION_STATEMENT, {TYPE, IDENTIFIER_ACTION}));

    //defining assignments
    productions.push_back(Production(ASSIGNMENT, {IDENTIFIER_ACTION, ASSIGNMENT_ACTION, EXPRESSION}));
    
    //defining types
    productions.push_back(Production(TYPE, {INT_ACTION}));
    productions.push_back(Production(TYPE, {CHAR_ACTION}));
    productions.push_back(Production(TYPE, {BOOL_ACTION}));
    productions.push_back(Production(VOID_TYPE, {VOID_ACTION}));

    //defining expressions
    productions.push_back(Production(EXPRESSION, {FACTOR}));
    productions.push_back(Production(EXPRESSION, {EXPRESSION, AROP, FACTOR}));
    productions.push_back(Production(EXPRESSION, {EXPRESSION, BITOP, FACTOR}));
    productions.push_back(Production(EXPRESSION, {IDENTIFIER_ACTION, UNARY_EXPRESSION, COMMA_TOKEN}));

    //defining factors
    productions.push_back(Production(FACTOR, {NUMBER_ACTION}));
    productions.push_back(Production(FACTOR, {IDENTIFIER_ACTION}));
    productions.push_back(Production(FACTOR, {LITERAL_ACTION}));
    productions.push_back(Production(FACTOR, {LPARAN_ACTION, EXPRESSION, RPARAN_ACTION}));
    productions.push_back(Production(FACTOR, {BOOL_TYPE}));

    //defining conditions
    productions.push_back(Production(CONDITION, {EXPRESSION, RELATIONAL_OPERATOR, EXPRESSION}));

    //defining relational operators
    productions.push_back(Production(RELATIONAL_OPERATOR, {EQUAL_ACTION}));
    productions.push_back(Production(RELATIONAL_OPERATOR, {NOTEQ_ACTION}));
    productions.push_back(Production(RELATIONAL_OPERATOR, {SMALLER_ACTION}));
    productions.push_back(Production(RELATIONAL_OPERATOR, {BIGGER_ACTION}));
    productions.push_back(Production(RELATIONAL_OPERATOR, {SMALLER_EQUAL_ACTION}));
    productions.push_back(Production(RELATIONAL_OPERATOR, {BIGGER_EQUAL_ACTION}));
   
    //defining arithmetic operators
    productions.push_back(Production(AROP, {MINUS_ACTION}));
    productions.push_back(Production(AROP, {PLUS_ACTION}));
    productions.push_back(Production(AROP, {MULT_ACTION}));
    productions.push_back(Production(AROP, {DIV_ACTION}));

    //defining unary operators
    productions.push_back(Production(UNARY_EXPRESSION, {PLUS_ACTION, PLUS_ACTION}));
    productions.push_back(Production(UNARY_EXPRESSION, {MINUS_ACTION, MINUS_ACTION}));

    //defining bitwise oprators
    productions.push_back(Production(BITOP, {OR_ACTION}));
    productions.push_back(Production(BITOP, {AND_ACTION}));
    productions.push_back(Production(BITOP, {SHR_ACTION}));
    productions.push_back(Production(BITOP, {SHL_ACTION}));

    //defining return statement
    productions.push_back(Production(RETURN_STATEMENT, {RETURN_ACTION, FACTOR}));

    //defining functions
    productions.push_back(Production(FUNCTION, {GIVE_ACTION, TYPE, IDENTIFIER_ACTION, LPARAN_ACTION, INPUT_VAR_LIST,
     RPARAN_ACTION, LBRACE_ACTION, STATEMENT_LIST, RETURN_STATEMENT ,RBRACE_ACTION}));
    productions.push_back(Production(FUNCTION, {GIVE_ACTION, VOID_TYPE, IDENTIFIER_ACTION, LPARAN_ACTION, RPARAN_ACTION,
     LBRACE_ACTION, STATEMENT_LIST, RBRACE_ACTION}));

    //defining input var list
    productions.push_back(Production(INPUT_VAR_LIST, {INPUT_VAR}));
    productions.push_back(Production(INPUT_VAR_LIST, {INPUT_VAR_LIST, COMMA_ACTION, INPUT_VAR}));
    productions.push_back(Production(INPUT_VAR_LIST, {}));

    //defining input var
    productions.push_back(Production(INPUT_VAR, {TYPE, IDENTIFIER_ACTION}));

    productions.push_back(Production(BOOL_TYPE, {TRUE_ACTION}));
    productions.push_back(Production(BOOL_TYPE, {FALSE_ACTION}));

    //define print statement
    productions.push_back(Production(PRINT_STATEMENT, {PRINT_ACTION, LPARAN_ACTION, EXPRESSION, RPARAN_ACTION}));
}


// Function to map token types to action table keys
// input: none
// output: none
void Parser::makeMap()
{
    //mapping token types to action table keys
    //these lexer tokens are mapped to their respective action table keys

    //factors
    this->tokenActions[ID_IDENTIFIER] = actionTableKeys::IDENTIFIER_ACTION;
    this->tokenActions[ID_NUMBER] = actionTableKeys::NUMBER_ACTION;

    //special characters
    this->tokenActions[LPARAN] = actionTableKeys::LPARAN_ACTION;
    this->tokenActions[RPARAN] = actionTableKeys::RPARAN_ACTION;
    this->tokenActions[LBRACE] = actionTableKeys::LBRACE_ACTION;
    this->tokenActions[RBRACE] = actionTableKeys::RBRACE_ACTION;
    this->tokenActions[SEMI_COLON] = actionTableKeys::SEMI_COLON_ACTION;
    this->tokenActions[COMMA_TOKEN] = actionTableKeys::COMMA_ACTION;
    
    //keywords
    this->tokenActions[ID_IF_CONDITION] = actionTableKeys::IF_ACTION;
    this->tokenActions[ID_ELSE_CONDITION] = actionTableKeys::ELSE_ACTION;
    this->tokenActions[ID_WHILE_LOOP] = actionTableKeys::WHILE_ACTION;
    this->tokenActions[ID_FOR_LOOP] = actionTableKeys::FOR_ACTION;

    //types
    this->tokenActions[ID_CHAR] = actionTableKeys::CHAR_ACTION;
    this->tokenActions[ID_INT] = actionTableKeys::INT_ACTION;
    this->tokenActions[ID_BOOL] = actionTableKeys::BOOL_ACTION;
    this->tokenActions[ID_VOID] = actionTableKeys::VOID_ACTION;
    this->tokenActions[ID_TRUE] = actionTableKeys::TRUE_ACTION;
    this->tokenActions[ID_FALSE] = actionTableKeys::TRUE_ACTION;
    this->tokenActions[ID_LITERAL] = actionTableKeys::LITERAL_ACTION;

    //operators
    this->tokenActions[BINOP_DIV] = actionTableKeys::DIV_ACTION;
    this->tokenActions[BINOP_PLUS] = actionTableKeys::PLUS_ACTION;
    this->tokenActions[BINOP_MINUS] = actionTableKeys::MINUS_ACTION;
    this->tokenActions[BINOP_MULT] = actionTableKeys::MULT_ACTION;

    //logical operators
    this->tokenActions[AND_OP] = actionTableKeys::AND_ACTION;
    this->tokenActions[EQUAL] = actionTableKeys::EQUAL_ACTION;
    this->tokenActions[SMALLER] = actionTableKeys::SMALLER_ACTION;
    this->tokenActions[BIGEER] = actionTableKeys::BIGGER_ACTION;
    this->tokenActions[SMALLER_EQUAL] = actionTableKeys::SMALLER_EQUAL_ACTION;
    this->tokenActions[BIGEER_EQUAL] = actionTableKeys::BIGGER_EQUAL_ACTION;
    this->tokenActions[NOT_EQ] = actionTableKeys::NOTEQ_ACTION;

    
    this->tokenActions[DEFINE_VAR] = actionTableKeys::ASSIGNMENT_ACTION;
    this->tokenActions[ID_RETURN] = actionTableKeys::RETURN_ACTION;
    this->tokenActions[ID_GIVE] = actionTableKeys::GIVE_ACTION;
    this->tokenActions[ID_PRINT] = actionTableKeys::PRINT_ACTION;
    this->tokenActions[ID_EOF] = actionTableKeys::EOF_ACTION;
}

// Function to generate parse tables from csv file
// input: csv file with action and goto table
// output: 2d vector of strings
void Parser::generateParseTables()
{
    std::string line;                    /* string to hold each line */
    vector<vector<std::string>> array;     /* vector of vector<int> for 2d array */
    ifstream f("ParserGen/parser_generator.csv"); /* open file */

    while (getline (f, line)) {         /* read each line */
        std::string val;                     /* string to hold value */
        std::vector<string> row;                /* vector for row of values */
        std::stringstream s (line);          /* stringstream to parse csv */
        while (getline (s, val, ','))   /* for each value */
            row.push_back (val);  /* add to row */
        array.push_back (row);          /* add row to array */
    }
    this->actionGoTable = array;
    f.close();
}

// Function to parse the input list and generate the AST
// input: none
// output: ParseTree node that is the root of the AST
ParseTree Parser::parse()
{
    int state, tokenCode, nextState, productionNum, nonTerminal;
    std::string action, nextStateStr;
    Production production;
    Token token;

    //pushing initial state to stack
    this->tokenStack.push(0);
    inputList.push_back(Token{"$", ID_EOF});

    while (!inputList.empty())
    {
        token = inputList.front();
        state = this->tokenStack.peek();
        tokenCode = this->tokenActions[token.type];
        action = this->actionGoTable[state][tokenCode];
        
        //print state, token, action for debugging
        cout << "state: " << state << " token: " << tokenCode << " action: " << action << endl;

        if(action == "acc")
        {
            cout << "accepted" << endl;
            ParseTree prog;
            prog.value = PROGRAM;
            prog.token = Token{"PROGRAM", PROGRAM};
            prog.children.push_back(parStack.pop());
            return prog;
        }
        else if(action[0] == 's')
        {
            nextState = stoi(action.substr(1));
            this->tokenStack.push(nextState);
            inputList.pop_front();

            ParseTree node;
            node.value = this->tokenActions[token.type];
            node.token = token;
            parStack.push(node);
        }
        else if(action[0] == 'r')
        {
            productionNum = stoi(action.substr(1));
            auto it = std::next(productions.begin(), productionNum);
            production = *it;

            Token *token = new Token();
            token->token = non_terminal_words[production.left - NT_OFFSET];
            token->type = production.left;

            ParseTree *ast = new ParseTree(token);
            ast->token = *token;
            ast->value = production.left;
        
            //poping from stack and adding to children
            for(int i = 0; i < production.right.size(); i++)
            {
                this->tokenStack.pop();

                ParseTree child = parStack.pop();
                child.root = ast;
                ast->token.loc = child.token.loc;
                ast->children.insert(ast->children.begin(), child);    
            }
            //pushing the node to node stack
            parStack.push(*ast);
            
            //getting next state from goto table and pushing it to stack
            state = this->tokenStack.peek();
            nonTerminal = production.left;
            nextStateStr = this->actionGoTable[state][nonTerminal];
            this->tokenStack.push(stoi(nextStateStr));
        }
        else
        {
            cout << "Parsing error in line " << token.loc << " - token:\"" << token.token << "\" is not valid in this position" << std::endl;
            return ParseTree();
        }
    }
    return ParseTree();
}

void ParseTree::setReg(RegisterEntry reg)
{
    this->reg = reg;
}

RegisterEntry ParseTree::getReg()
{
    return this->reg;
}


// Function to print the tree in level order
// input: ParseTree node, string prefix, bool isLast
// output: none
void Parser::printAST(ParseTree& root, string prefix, bool isLast) {
    cout << prefix << (isLast ? "└── " : "├── ") << root.token.token << " " << root.value << endl;
    for (int i = 0; i < root.children.size(); i++) {
        printAST(root.children[i], prefix + (isLast ? "    " : "│   "), i == root.children.size() - 1);
    }
}

