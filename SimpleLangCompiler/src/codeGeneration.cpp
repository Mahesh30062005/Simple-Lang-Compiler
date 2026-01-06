#include "codeGeneration.h"
#include <fstream>
#include <iostream>

using namespace std;

// Constructor: Start allocating memory at Address 0
CodeGen::CodeGen() {
    nextAddress = 0;
}

// Helper: Find where a variable lives
int CodeGen::getAddress(string name) {
    if (symbolTable.find(name) == symbolTable.end()) {
        cerr << "CodeGen Error: Variable '" << name << "' used but not declared." << endl;
        exit(1);
    }
    return symbolTable[name];
}


// Main Entry Point


void CodeGen::generate(Program* p) {
    for (Statement* stmt : p->statements) {
        genStatement(stmt);
    }
    instructions.push_back("HLT"); // Stop the CPU at the end
}


// Statement Generator


void CodeGen::genStatement(Statement* stmt) {
    
    // 1. Variable Declaration: "int a;"
    if (VarDecl* v = dynamic_cast<VarDecl*>(stmt)) {
        if (symbolTable.find(v->name) != symbolTable.end()) {
            cerr << "Error: Variable '" << v->name << "' already declared." << endl;
            exit(1);
        }
        // Assign next available memory slot
        symbolTable[v->name] = nextAddress++;
        // No assembly needed here, just booking the space.
    }
    
    // 2. Assignment: "a = 5;"
    else if (Assignment* a = dynamic_cast<Assignment*>(stmt)) {
        // Calculate the value (Result will be in A)
        genExpression(a->value);
        
        // Store A into variable's address
        int addr = getAddress(a->name);
        instructions.push_back("STA " + to_string(addr));
    }
    
    // 3. If Statement: "if (cond) { ... }"
    else if (IfStmt* i = dynamic_cast<IfStmt*>(stmt)) {
        // Evaluate condition (Result in A)
        // Assume non-zero is TRUE, zero is FALSE.
        genExpression(i->condition);
        
        // Logic: "JZ" (Jump if Zero) means "Jump if False".
        // We need to jump OVER the body if the condition is false.
        
        // Placeholder for the jump instruction (we fix the label later)
        int jumpIndex = instructions.size(); 
        instructions.push_back("JZ  PENDING"); 
        
        // Generate the body
        for (Statement* s : i->body) {
            genStatement(s);
        }
        
        // Patch the Jump: Tell it where to land (current instruction count)
        // We assume instructions are 2 bytes each (Opcode + Operand), 
        // but for a simple assembler, line numbers often work or we use labels.
        // SimpleLang Assembler trick: Let's assume we can jump to a line number.
        // Actually, JZ usually takes an absolute address. 
        // For this simple task, let's output a Label instead.
        
        // BETTER APPROACH FOR V1:
        // Since we don't have a label system yet, let's just use a comment placeholder
        // and tell the user to manually calculate jumps, OR 
        // we can count bytes. (Standard 8-bit instructions = 2 bytes).
        
        // Let's use a "Label" approach for clarity, assuming the assembler supports it.
        // If not, we would need to calculate: (instructions.size() * 2).
        string label = "L_END_IF_" + to_string(jumpIndex);
        instructions[jumpIndex] = "JZ " + label;
        instructions.push_back(label + ":");
    }
}




// Expression Generator (The Math)



void CodeGen::genExpression(Expression* expr) {
    
    // Case 1: Number ("10")
    if (Number* n = dynamic_cast<Number*>(expr)) {
        instructions.push_back("LDI " + to_string(n->value));
    }
    
    // Case 2: Variable ("a")
    else if (Variable* v = dynamic_cast<Variable*>(expr)) {
        int addr = getAddress(v->name);
        instructions.push_back("LDA " + to_string(addr));
    }
    
    // Case 3: Binary Operation ("a + b")
    else if (BinaryOp* b = dynamic_cast<BinaryOp*>(expr)) {
        // We have strict left-to-right processing constraint.
        // A = Left op Right
        
        // Step A: Calculate Right side first
        genExpression(b->right);
        // Step B: Save it to Scratchpad (Addr 255)
        instructions.push_back("STA 255");
        
        // Step C: Calculate Left side
        genExpression(b->left);
        
        // Step D: Perform Operation with Scratchpad
        if (b->op == "+") {
            instructions.push_back("ADD 255");
        } else if (b->op == "-") {
            instructions.push_back("SUB 255");
        }
        else if (b->op == "==") {
            instructions.push_back("SUB 255"); // Compare by subtracting
            // Note: Result is 0 if equal.
        }
    }
}

void CodeGen::saveToFile(string filename) {
    ofstream file(filename);
    for (string line : instructions) {
        // If it's a label, print as is. If instruction, indent.
        if (line.back() == ':') file << line << endl;
        else file << "    " << line << endl;
    }
    file.close();
    cout << "Assembly saved to " << filename << endl;
}