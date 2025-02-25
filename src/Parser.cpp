#include "../headers/Parser.hpp"

Parser::Parser()
{
    //definig program
    productions.push_back(Production(PROGRAM, {STATEMENT}));
    productions.push_back(Production(PROGRAM, {PROGRAM, STATEMENT}));

    //defining statements
    productions.push_back(Production(STATEMENT, {IF_STATEMENT}));
    productions.push_back(Production(STATEMENT, {WHILE_STATEMENT}));
    productions.push_back(Production(STATEMENT, {ASSIGNMENT}));
    productions.push_back(Production(STATEMENT, {DECELERATION_STATEMENT}));
    productions.push_back(Production(STATEMENT, {FOR_STATEMENT}));

    //defining loops and conditions
    productions.push_back(Production(FOR_STATEMENT, {FOR_ACTION, LPARAN_ACTION, ASSIGNMENT_ACTION, SEMI_COLON_ACTION, CONDITION, SEMI_COLON_ACTION, AROP, RPARAN_ACTION, LBRACE_ACTION, EXPRESSION, RBRACE_ACTION}));
    productions.push_back(Production(IF_STATEMENT, {IF_ACTION, LPARAN_ACTION, CONDITION, RPARAN_ACTION, LBRACE_ACTION, EXPRESSION, RBRACE_ACTION}));
    productions.push_back(Production(IF_STATEMENT, {IF_ACTION, LPARAN_ACTION, CONDITION, RPARAN_ACTION, LBRACE_ACTION, EXPRESSION, RBRACE_ACTION, ELSE_ACTION, LBRACE_ACTION, PROGRAM, RBRACE_ACTION}));
    productions.push_back(Production(WHILE_STATEMENT, {WHILE_ACTION, LPARAN_ACTION, CONDITION, RPARAN_ACTION, LBRACE_ACTION, EXPRESSION, RBRACE_ACTION}));
    
    //defining declarations
    productions.push_back(Production(DECELERATION_STATEMENT, {TYPE, IDENTIFIER_ACTION, ASSIGNMENT_ACTION, EXPRESSION}));
    productions.push_back(Production(DECELERATION_STATEMENT, {TYPE, IDENTIFIER_ACTION}));

    //defining assignments
    productions.push_back(Production(ASSIGNMENT, {IDENTIFIER_ACTION, ASSIGNMENT_ACTION, EXPRESSION}));
    
    //defining types
    productions.push_back(Production(TYPE, {INT_ACTION}));
    productions.push_back(Production(TYPE, {CHAR_ACTION}));
    productions.push_back(Production(TYPE, {BOOL_ACTION}));

    //defining expressions
    productions.push_back(Production(EXPRESSION, {FACTOR}));
    productions.push_back(Production(EXPRESSION, {FACTOR, AROP, EXPRESSION}));
    productions.push_back(Production(EXPRESSION, {FACTOR, BITOP, EXPRESSION}));
    productions.push_back(Production(EXPRESSION, {IDENTIFIER_ACTION, UNARY_EXPRESSION}));
    productions.push_back(Production(EXPRESSION, {STATEMENT}));

    //defining factors
    productions.push_back(Production(FACTOR, {NUMBER_ACTION}));
    productions.push_back(Production(FACTOR, {IDENTIFIER_ACTION}));

    //defining conditions
    productions.push_back(Production(CONDITION, {EXPRESSION, RELATIONAL_OPERATOR, EXPRESSION}));

    //defining relational operators
    productions.push_back(Production(RELATIONAL_OPERATOR, {EQUAL_ACTION}));
    productions.push_back(Production(RELATIONAL_OPERATOR, {NOTEQ_ACTION}));
    productions.push_back(Production(RELATIONAL_OPERATOR, {SMALLER_ACTION}));
    productions.push_back(Production(RELATIONAL_OPERATOR, {BIGGER_ACTION}));
    productions.push_back(Production(RELATIONAL_OPERATOR, {SMALLER_EQUAL_ACTION}));
    productions.push_back(Production(RELATIONAL_OPERATOR, {BIGGER_EQUAL_ACTION}));
   
    //defining binops
    productions.push_back(Production(AROP, {MINUS_ACTION}));
    productions.push_back(Production(AROP, {PLUS_ACTION}));
    productions.push_back(Production(AROP, {MULT_ACTION}));
    productions.push_back(Production(AROP, {DIV_ACTION}));

    //defining unary operators
    productions.push_back(Production(UNARY_EXPRESSION, {UNARY_MINUS_ACTION}));
    productions.push_back(Production(UNARY_EXPRESSION, {UNARY_PLUS_ACTION}));

    //defining bitwise oprators
    productions.push_back(Production(BITOP, {OR_ACTION}));
    productions.push_back(Production(BITOP, {AND_ACTION}));
    productions.push_back(Production(BITOP, {SHR_ACTION}));
    productions.push_back(Production(BITOP, {SHL_ACTION}));

    generateParseTables();
    makeMap();
}


// Function to map token types to action table keys
void Parser::makeMap()
{
    this->tokenActions[ID_IDENTIFIER] = actionTableKeys::IDENTIFIER_ACTION;
    this->tokenActions[ID_NUMBER] = actionTableKeys::NUMBER_ACTION;
    this->tokenActions[LPARAN] = actionTableKeys::LPARAN_ACTION;
    this->tokenActions[RPARAN] = actionTableKeys::RPARAN_ACTION;
    this->tokenActions[LBRACE] = actionTableKeys::LBRACE_ACTION;
    this->tokenActions[RBRACE] = actionTableKeys::RBRACE_ACTION;
    this->tokenActions[SEMI_COLON] = actionTableKeys::SEMI_COLON_ACTION;
    this->tokenActions[ID_IF_CONDITION] = actionTableKeys::IF_ACTION;
    this->tokenActions[ID_ELSE_CONDITION] = actionTableKeys::ELSE_ACTION;
    this->tokenActions[ID_WHILE_LOOP] = actionTableKeys::WHILE_ACTION;
    this->tokenActions[ID_FOR_LOOP] = actionTableKeys::FOR_ACTION;
    this->tokenActions[ID_CHAR] = actionTableKeys::CHAR_ACTION;
    this->tokenActions[ID_INT] = actionTableKeys::INT_ACTION;
    this->tokenActions[ID_BOOL] = actionTableKeys::BOOL_ACTION;
    this->tokenActions[BINOP_DIV] = actionTableKeys::DIV_ACTION;
    this->tokenActions[BINOP_PLUS] = actionTableKeys::PLUS_ACTION;
    this->tokenActions[BINOP_MINUS] = actionTableKeys::MINUS_ACTION;
    this->tokenActions[BINOP_MULT] = actionTableKeys::MULT_ACTION;
    this->tokenActions[BINOP_AND] = actionTableKeys::AND_ACTION;
    this->tokenActions[LOGIC_AND] = actionTableKeys::AND_ACTION;
    this->tokenActions[LOGIC_OR] = actionTableKeys::OR_ACTION;
    this->tokenActions[EQUAL] = actionTableKeys::EQUAL_ACTION;
    this->tokenActions[BIGEER_EQUAL] = actionTableKeys::BIGGER_EQUAL_ACTION;
    this->tokenActions[SMALLER_EQUAL] = actionTableKeys::SMALLER_EQUAL_ACTION;
    this->tokenActions[NOT_EQ] = actionTableKeys::NOTEQ_ACTION;
    this->tokenActions[DEFINE_VAR] = actionTableKeys::ASSIGNMENT_ACTION;
    this->tokenActions[SEMI_COLON] = actionTableKeys::SEMI_COLON_ACTION;
    this->tokenActions[ID_EOF] = actionTableKeys::EOF_ACTION;
}

// Function to generate parse tables from csv file
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
ParseTree Parser::parse()
{
    int state, tokenCode, nextState, productionNum, nonTerminal;
    std::string action, nextStateStr;
    Production production;
    ParseTree root;
    Token token;

    root.value = PROGRAM;
    inputList.push_back(Token{"$", ID_EOF});
    this->tokenStack.push(0);

    while (!inputList.empty())
    {
        token = inputList.front();
        state = this->tokenStack.peek();
        tokenCode = this->tokenActions[token.type];
        action = this->actionGoTable[state][tokenCode];
        
        //print state, token, action for debugging
        //cout << "state: " << state << " token: " << tokenCode << " action: " << action << endl;

        if(action == "acc")
        {
            cout << "accepted" << endl;
            root.children.push_back(ASTack.pop());
            return root;
        }
        else if(action[0] == 's')
        {
            nextState = stoi(action.substr(1));
            this->tokenStack.push(nextState);
            inputList.pop_front();

            ParseTree node;
            node.value = this->tokenActions[token.type];
            ASTack.push(node);
        }
        else if(action[0] == 'r')
        {
            productionNum = stoi(action.substr(1));
            auto it = std::next(productions.begin(), productionNum);
            production = *it;

            ParseTree node;
            node.value = production.left;
            
            for(int i = 0; i < production.right.size(); i++)
            {
                this->tokenStack.pop();
                node.children.insert(node.children.begin(), ASTack.pop());                
            }
            ASTack.push(node);

            state = this->tokenStack.peek();
            nonTerminal = production.left;
            nextStateStr = this->actionGoTable[state][nonTerminal];
            this->tokenStack.push(stoi(nextStateStr));

        }
        else
        {
            cout << "parsing error - token: \"" << token.token << "\" is not valid in this position" << std::endl;
            return ParseTree();
        }
    }
    return ParseTree();
}

// Function to print the tree in level order
void Parser::printAST(ParseTree& root, string prefix, bool isLast) {
  cout << prefix << (isLast ? "└── " : "├── ") << root.value << endl;

  if (!root.children.empty()) {
    for (size_t i = 0; i < root.children.size(); ++i) {
      // Pass the child node directly, not its reference
      printAST(root.children[i], prefix + (isLast ? "   " : "│  "), i == root.children.size() - 1);
    }
  }
}
