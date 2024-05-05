#include "../headers/CodeGen.hpp"

bool CodeGen::generate(ParseTree *parseTree)
{
    std::string outputFileName = this->fileName.substr(0, fileName.size() - 4) + ".asm";
    std::ofstream outputFile(outputFileName);
    
    if (outputFile.is_open())
    {
        createBaseAsm(outputFile);
        generateCodeFromAST(parseTree);
        outputFile << "\n_start:\n";
        for (string line : code)
        {
            outputFile << line;
        }
        generateExit(outputFile);
        generateIdentifiers(outputFile);
    }
    else
    {
        std::cerr << "Error: Unable to open file" << std::endl;
        return false;
    }
    outputFile.close();
    exacuteNasm(outputFileName);

    return true;
}

void CodeGen::exacuteNasm(string outputFileName)
{
    std::cout << "\nRunning following commands to run the code: "<<std::endl;
    // Command to execute (replace with your actual command)
    std::string assmble = "nasm -f elf32 " + outputFileName + " -o " + this->fileName.substr(0, fileName.size() - 4) + ".o";
    std::string link = "ld -m elf_i386 -s -o " + this->fileName.substr(0, fileName.size() - 4) + " "+ this->fileName.substr(0, fileName.size() - 4) + ".o";
    std::string run = "./" + this->fileName.substr(0, fileName.size() - 4);

    std:cout << assmble << std::endl << link << std::endl << run << std::endl;

    // Execute the command using system
    bool returnCode = system(assmble.c_str());
    bool returnCode2 = system(link.c_str());
    bool returnCode3 = system(run.c_str());

    if (!(returnCode || returnCode2 || returnCode3)) {
        std::cout << "\nCommands executed successfully." << std::endl;
    } else {
        std::cerr << "\nError executing one or more commands " << std::endl;
    }
}

void CodeGen::createBaseAsm(ofstream &file)
{
    file << "BITS 32\nsection .text\nglobal _start\n" << std::endl;
} 

void CodeGen::generateIdentifiers(ofstream &file)
{
    file << "\nsection .data\n";
    file << "\tsavedMemoryForPrinting db 100 dup(0)\n"; // write the temporary variable
    file << "\ttenDividerForPrintingNumbers dw 10\n";        // write the divider
    
    addData("newLineSavedMemory db 10, 0");
    for(string str : data) //add the predefined data
    {
        file << "\t" << str << "\n";
    }
    for (Symbol s : symbolTable)     // iterate over the symbol table
    {
        file << "\t" << s.name << (s.type == INT ? " dd 0\n" : " db 0\n"); // write the identifier
    }
}

void CodeGen::generateCodeFromAST(ParseTree *tree)
{
    Token *token = &tree->token; // get the root of the AST
    if (tree->value == ASSIGNMENT)  // if the root is an assignment
    {
        Token *identifier = &tree->children.at(0).token;
        RegisterEntry reg = generateExpression(&tree->children.at(2)); // get a register
        addCode("\tmov [" + identifier->token + "], " + reg.name + "\n");
        freeRegister(reg); // free the register
        return;
    }
    if (tree->value == DECELERATION_STATEMENT) // if the root is a declaration
    {
        if (tree->children.size() == 4) {
            RegisterEntry reg = generateExpression(&tree->children.at(3));
            addCode("\tmov [" + tree->children.at(1).token.token + "], " + reg.name + "\n");
            freeRegister(reg);
        }
        else
        {
            addCode("\tmov byte [" + tree->children.at(1).token.token + "], 0\n");    
        }
        return;
    }
    if (tree->value == EXPRESSION) // if the root is an expression
    {
        return;
    }
    if (tree->value == PRINT_STATEMENT) // if the root is an out function
    {
        RegisterEntry reg = generateExpression(&tree->children.at(2)); // generate the expression
        if(tree->children.at(2).token.type == ID_CHAR )
        {
            addCode("\tmov [savedMemoryForPrinting], " + reg.name + "\n"); // move the result to the temporary variable
            addCode("\tpusha\n");
            addCode("\tmov eax, 4\n");
            addCode("\tmov ebx, 1\n");
            addCode("\tmov ecx, savedMemoryForPrinting\n");
            addCode("\tmov edx, 2\n");
            addCode("\tint 0x80\n");
            addCode("\tpopa\n");
        }
        else if (tree->children.at(2).token.type == ID_NUMBER)
        {
            //prepare the number for printing
            addCode("\n\tmov eax, " + reg.name + "\n");
            addCode("\tcall print_num\n");
        }

        else if(tree->children.at(2).token.type == ID_LITERAL)
        {
            //for printing string literals
            // calculate the length of string
            addCode("\tmov eax,messageToPrint" + to_string(dataCount) + "\n");
            addCode("\tmov ecx,-1\n");
            addCode("print_loop"+ to_string(labelCount) + ":\n");
            addCode("\tinc ecx\n");
            addCode("\tcmp byte [eax + ecx], 0\n");
            addCode("\tjne print_loop"+ to_string(labelCount++) + "\n");
            
            // print the string
            addCode("\tmov	edx,ecx\n");		// arg3, length of string to print
            addCode("\tmov	ecx,messageToPrint"+ to_string(dataCount++) + "\n");		// arg2, pointer to string
            addCode("\tmov	ebx,1\n");	// arg1, where to write, screen
            addCode("\tmov	eax,4\n");	// write sysout command to int 80 hex
            addCode("\tint	0x80\n");	// interrupt 80 hex, call kernel 
        }
        addCode("\tcall print_new_line_label\n");
        return;
    }
    if (tree->value == WHILE_STATEMENT) // if the root is a loop
    {
        addCode("while_loop_label" + to_string(whileLabels) + ":\n");
        RegisterEntry reg = generateExpression(&tree->children.at(2)); // get a register
        addCode("\tcmp " + reg.name + ", 0\n");
        addCode("\tjz while_statement_end" + to_string(whileLabels) + "\n");
        generateCodeFromAST(&tree->children.at(5)); // generate code for the body
        addCode("\tjmp while_loop_label" + to_string(whileLabels) + "\n");
        addCode("while_statement_end" + to_string(whileLabels++) + ":\n");
        freeRegister(reg); // free the register
        return;
    }
    if (tree->value == FOR_STATEMENT)
    {
        Token *count = &tree->children.at(2).root->token;
        int typeCode = count->type;                      
        RegisterEntry reg = getRegister();                        
        addCode("\tmov " + reg.name + ", " + count->token + "\n");
        addCode("for_loop_label" + to_string(forLabels) + ":\n");
        addCode("\tpush " + reg.name + "\n");
        generateCodeFromAST(&tree->children.at(5)); // generate code for the list of expressions
        addCode("\tpop " + reg.name + "\n");
        addCode("\tdec " + reg.name + "\n");
        addCode("\tjnz for_loop_label" + to_string(forLabels++) + "\n");
        return;
    }
    if (tree->value == IF_STATEMENT) // if the root is a conditional
    {
        if(tree->children.size() == 7) //if statement without else statement
        {
            RegisterEntry reg = generateExpression(&tree->children.at(2)); // get a register
            addCode("\tcmp " + reg.name + ", 0\n");
            addCode("\tjz if_statement_end" + to_string(ifLabels) + "\n");
            generateCodeFromAST(&tree->children.at(5)); // generate code for the body
            addCode("if_statement_end" + to_string(ifLabels++) + ":\n");
            freeRegister(reg); // free the register
        }
        else
        {      
            //if statement with else statement                                                           
            ParseTree *cond = &tree->children.at(0);                                  
            RegisterEntry reg = generateExpression(&cond->children.at(2).children.at(0)); 
            addCode("\tcmp " + reg.name + ", 0\n");
            addCode("\tjz else_" + to_string(ifLabels) + "\n");
            generateCodeFromAST(&cond->children.at(5)); // generate code for the if body
            addCode("\tjmp else_statemet_end" + to_string(ifLabels) + "\n");
            addCode("else_" + to_string(ifLabels) + ":\n");
            generateCodeFromAST(&tree->children.at(3)); // generate code for the else body
            addCode("else_statemet_end" + to_string(ifLabels++) + ":\n");
            freeRegister(reg); // free the register
        }
        return;
    }
    for (ParseTree child : tree->children) // generate code for each child
    {
        generateCodeFromAST(&child); // generate code from the child
    }
}

RegisterEntry CodeGen::generateExpression(ParseTree *expression)
{
   
    // ParseTree *root = expression;                   // get the root of the expression
    if (expression->value == IDENTIFIER_ACTION  || expression->value == NUMBER_ACTION) // if the expression is an identifier or literal
    {
        RegisterEntry reg = getRegister();
        addCode("\tmov " + reg.name + ", " + (expression->value == IDENTIFIER_ACTION ? "[" + expression->token.token + "]" : expression->token.token) + "\n");
        expression->setReg(reg);
    }
    else if(expression->value == LITERAL_ACTION)
    {
        RegisterEntry reg = getRegister();
        addData("messageToPrint" + to_string(dataCount) + " db " + expression->token.token + ", 0");
        expression->setReg(reg);
    }
    else if (expression->value == EXPRESSION || expression->value == CONDITION) // if the expression is an expression
    {
        for (ParseTree& child : expression->children) // generate code for each child
        {
            generateExpression(&child);
        }
        if (expression->children.size() == 3) // if the expression has 3 children
        {
            RegisterEntry lreg = expression->children.at(0).getReg();     // get the left register
            RegisterEntry rreg = expression->children.at(2).getReg();     // get the right register
            string op = expression->children.at(1).children.at(0).token.token; // get the operator
            if (op == "+")                                         // if the operator is addition
            {
                addCode("\tadd " + lreg.name + ", " + rreg.name + "\n");
                freeRegister(rreg);
                expression->setReg(lreg);
            }
            else if (op == "-") // if the operator is subtraction
            {
                addCode("\tsub " + lreg.name + ", " + rreg.name + "\n");
                freeRegister(rreg);
                expression->setReg(lreg);
            }
            else if (op == "*") // if the operator is multiplication
            {
                addCode("\timul " + lreg.name + ", " + rreg.name + "\n");
                freeRegister(rreg);
                expression->setReg(lreg);
            }
            else if (op == "/") // if the operator is division
            {
                addCode("\tpush edx\n");
                addCode("\tpush eax\n");
                addCode("\tmov eax, " + lreg.name + "\n");
                RegisterEntry reg = getRegister();
                addCode("\tmov " + reg.name + ", " + rreg.name + "\n");
                addCode("\txor edx, edx\n");
                addCode("\tidiv " + reg.name + "\n");
                addCode("\tmov " + reg.name + ", eax\n");
                addCode("\tpop eax\n");
                addCode("\tpop edx\n");
                addCode("\tmov " + lreg.name + ", " + reg.name + "\n");
                freeRegister(reg);
                freeRegister(rreg);
                expression->setReg(lreg);
            }
            else if (op == "%") // if the operator is modulo
            {
                addCode("\tpush edx\n");
                addCode("\tpush eax\n");
                addCode("\tmov eax, " + lreg.name + "\n");
                setRegState({"edx", true}, false);
                RegisterEntry reg = getRegister();
                addCode("\tmov " + reg.name + ", " + rreg.name + "\n");
                addCode("\txor edx, edx\n");
                setRegState({"edx", false}, true);
                addCode("\tidiv " + reg.name + "\n");
                addCode("\tmov " + reg.name + ", edx\n");
                addCode("\tpop eax\n");
                addCode("\tpop edx\n");
                addCode("\tmov " + lreg.name + ", " + reg.name + "\n");
                freeRegister(reg);
                freeRegister(rreg);
                expression->setReg(lreg);
            }
            else if (op == "<") // if the operator is less than
            {
                addCode("\tcmp " + lreg.name + ", " + rreg.name + "\n");
                string reg8 = convert32to8(lreg, 1);
                addCode("\tsetl " + reg8 + "\n");
                freeRegister(rreg);
                expression->setReg(lreg);
            }
            else if (op == ">") // if the operator is greater than
            {
                addCode("\tcmp " + lreg.name + ", " + rreg.name + "\n");
                string reg8 = convert32to8(lreg, 1);
                addCode("\tsetg " + reg8 + "\n");
                freeRegister(rreg);
                expression->setReg(lreg);
            }
            else if (op == "==") // if the operator is equal to
            {
                addCode("\tcmp " + lreg.name + ", " + rreg.name + "\n");
                string reg8 = convert32to8(lreg, 1);
                addCode("\tsetz " + reg8 + "\n");
                freeRegister(rreg);
                expression->setReg(lreg);
            }
            else if (op == "!=") // if the operator is not equal to
            {
                addCode("\tcmp " + lreg.name + ", " + rreg.name + "\n");
                string reg8 = convert32to8(lreg, 1);
                addCode("\tsetz " + reg8 + "\n");
                addCode("\tnot " + reg8 + "\n");
                addCode("\tand " + lreg.name + ", 1\n");
                freeRegister(rreg);
                expression->setReg(lreg);
            }
            else if (op == "<=") // if the operator is less than or equal to
            {
                addCode("\tcmp " + lreg.name + ", " + rreg.name + "\n");
                string reg8 = convert32to8(lreg, 1);
                addCode("\tsetle " + reg8 + "\n");
                freeRegister(rreg);
                expression->setReg(lreg);
            }
            else if (op == ">=") // if the operator is greater than or equal to
            {
                addCode("\tcmp " + lreg.name + ", " + rreg.name + "\n");
                string reg8 = convert32to8(lreg, 1);
                addCode("\tsetge " + reg8 + "\n");
                freeRegister(rreg);
                expression->setReg(lreg);
            }
            else if (op == "&") // if the operator is bitwise AND
            {
                addCode("\tand " + lreg.name + ", " + rreg.name + "\n");
                freeRegister(rreg);
                expression->setReg(lreg);
            }
            else if (op == "|") // if the operator is bitwise OR
            {
                addCode("\tor " + lreg.name + ", " + rreg.name + "\n");
                freeRegister(rreg);
                expression->setReg(lreg);
            }
            else if (op == "^") // if the operator is bitwise XOR
            {
                addCode("\txor " + lreg.name + ", " + rreg.name + "\n");
                freeRegister(rreg);
                expression->setReg(lreg);
            }
            else if (op == "!") // if the operator is logical NOT
            {
                addCode("\tnot " + lreg.name + "\n");
                expression->setReg(lreg);
            }
            else if (op == "&&") // if the operator is logical AND
            {
                int count = this->labelCount++;

                // Evaluate the first expression and jump to falseLabel if it's false
                addCode("\tcmp " + lreg.name + ", 0\n");
                addCode("\tje falseLabel_" + to_string(count) + "\n");

                // Evaluate the second expression
                addCode("\tcmp " + rreg.name + ", 0\n");
                addCode("\tje falseLabel_" + to_string(count) + "\n");

                // If we're here, the first expression was true. The result of the AND operation
                addCode("\tmov " + lreg.name + ", 1\n");
                addCode("\tjmp endLabel_" + to_string(count) + "\n");

                // Label for the case where the first expression is false
                addCode("falseLabel_" + to_string(count) + ":\n");
                addCode("\tmov " + lreg.name + ", 0\n"); // Set result to false

                // End label
                addCode("endLabel_" + to_string(count) + ":\n");

                freeRegister(rreg);
                expression->setReg(lreg);
            }
            else if (op == "||") // if the operator is logical OR
            {
                int count = this->labelCount++;

                // Evaluate the first expression and jump to trueLabel if it's true
                addCode("\tcmp " + lreg.name + ", 0\n");
                addCode("\tjne trueLabel_" + to_string(count) + "\n");

                // Evaluate the second expression
                addCode("\tcmp " + rreg.name + ", 0\n");
                addCode("\tjne trueLabel_" + to_string(count) + "\n");

                // If we're here, the first expression was false. The result of the OR operation
                addCode("\tmov " + lreg.name + ", 0\n");
                addCode("\tjmp endLabel_" + to_string(count) + "\n");

                // Label for the case where the first expression is true
                addCode("trueLabel_" + to_string(count) + ":\n");
                addCode("\tmov " + lreg.name + ", 1\n"); // Set result to true

                // End label
                addCode("endLabel_" + to_string(count) + ":\n");

                freeRegister(rreg);
                expression->setReg(lreg);
            }
        }
        else // if the expression has only one child
        {
            expression->setReg(expression->children.at(0).getReg());
        }
    }
    else if (expression->value == FACTOR) // if the expression is a factor
    {
        if (expression->children.size() == 1) // if the expression has only one child
        {
            RegisterEntry reg = generateExpression(&expression->children.at(0));
            expression->setReg(reg);
        }
        else
        {
            RegisterEntry reg = generateExpression(&expression->children.at(1));
            expression->setReg(reg);
        }
    }
    return expression->getReg();
}

string CodeGen::convert32to8(RegisterEntry reg, int byte)
{
    string reg8;
    if (byte == 1) // low byte
    {
        if (reg.name == "eax")
            reg8 = "al";
        else if (reg.name == "ebx")
            reg8 = "bl";
        else if (reg.name == "ecx")
            reg8 = "cl";
        else if (reg.name == "edx")
            reg8 = "dl";
    }
    else // high byte
    {
        if (reg.name == "eax")
            reg8 = "ah";
        else if (reg.name == "ebx")
            reg8 = "bh";
        else if (reg.name == "ecx")
            reg8 = "ch";
        else if (reg.name == "edx")
            reg8 = "dh";
    }
    return reg8;
}

RegisterEntry CodeGen::getRegister()
{
    for (int i = 0; i < 6; i++) // iterate over the registers
    {
        if (registers[i].isFree) // if the register is free
        {
            registers[i].isFree = false;
            return registers[i];
        }
    }
    return registers[0];
}

void CodeGen::freeRegister(RegisterEntry reg)
{
    for (int i = 0; i < 6; i++)
    {
        if (registers[i].name == reg.name)
        {
            registers[i].isFree = true;
            break;
        }
    }
}

void CodeGen::setRegState(RegisterEntry reg, bool state)
{
    for (int i = 0; i < 6; i++)
    {
        if (registers[i].name == reg.name)
        {
            registers[i].isFree = state;
            break;
        }
    }
}

void CodeGen::pushRegs(string *regs, int n)
{
    for (int i = 0; i < n; i++)
    {
        addCode("\tpush " + regs[i] + "\n");
    }
}


void CodeGen::popRegs(string *regs, int n)
{
    for (int i = n - 1; i >= 0; i--)
    {
        addCode("\tpop " + regs[i] + "\n");
    }
}

void CodeGen::addCode(string code)
{
    this->code.push_back(code); // add the code to the code vector
}

void CodeGen::addData(string data)
{
    this->data.push_back(data); // add the data to the data vector
}

void CodeGen::generateExit(ofstream &file)
{
    file << "\tmov eax, 1\n";
    file << "\txor ebx, ebx\n";
    file << "\tint 0x80\n";


    //function to print numbers
    file << "\nprint_num:\n";
    file <<"\txor ecx, ecx\n";
    file <<"counter_label:\n";
    file <<"\tinc ecx\n";
    file <<"\txor edx, edx\n";
    
    //dividing the number by 10
    file << "\tdiv word [tenDividerForPrintingNumbers]\n";
    file << "\tpush edx\n";
    file << "\tcmp eax, 0\n";
    file << "\tjg counter_label\n";
    file << "\nputChar:\n";
    file << "\tpop edx\n";
    file << "\tadd edx, '0'\n";
    file << "\tmov [savedMemoryForPrinting], edx\n";
    file << "\tpusha\n";
    
    //printing the number
    file << "\tmov eax, 4\n";
    file << "\tmov ebx, 1\n";
    file << "\tmov ecx, savedMemoryForPrinting\n";
    file << "\tmov edx, 2\n";
    file << "\tint 0x80\n";
    
    //pop the registers
    file << "\tpopa\n";
    file << "\tloop putChar\n";
    file << "\tret\n";


    //after each print command we will print a new line
    file <<"print_new_line_label:\n";
    file <<"\tmov	edx,1\n";
    file <<"\tmov	ecx, newLineSavedMemory\n";	// to print newline
    file <<"\tmov	ebx,1\n";	// arg1, where to write, screen
    file <<"\tmov	eax,4\n";	// write sysout command to int 80 hex
    file <<"\tint	0x80\n"; 
    file <<"\tret\n";  
}