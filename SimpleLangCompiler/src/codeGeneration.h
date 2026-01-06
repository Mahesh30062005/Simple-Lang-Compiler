#ifndef CODEGEN_H
#define CODEGEN_H

#include "AbstractSynTree.h"
#include <map>
#include <string>
#include <vector>

class CodeGen {
private:
    std::map<std::string, int> symbolTable; // Maps "a" -> Address 0
    int nextAddress; // Tracks the next free memory slot (0, 1, 2...)
    std::vector<std::string> instructions; // Stores the assembly lines

    // Internal Helper: Get address of a variable or crash if not found
    int getAddress(std::string name);

public:
    CodeGen();
    
    // The Main Driver
    void generate(Program* p);
    
    // Visitors for AST Nodes
    void genStatement(Statement* stmt);
    void genExpression(Expression* expr); // Result always ends up in Register A

    // Write the assembly to a file
    void saveToFile(std::string filename);
};

#endif