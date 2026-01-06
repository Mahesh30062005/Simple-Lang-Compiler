#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "AbstractSynTree.h" // Make sure file name matches exactly
#include <vector>

// Parser class: Yeh tokens ko leke AST (Abstract Syntax Tree) banata hai
class Parser {
private:
    std::vector<Token> tokens; // Saare tokens yahan store honge
    int currentPos;            // Abhi hum kaunse token pe hain (Pointer)
    
    // --- Helper Functions (Internal Logic) ---

    // Current token ko dekho bina aage badhe (Just peeking)
    Token peek();

    // Current token ko consume karo aur pointer ko next pe move karo
    Token advance();

    // Check karo agar current token specific type ka hai kya. 
    // Agar haan, toh consume karlo aur true return karo.
    bool match(TokenType type);

    // Ye strict check hai. Agar token match nahi kiya toh seedha error throw karega (Crash).
    // Jaise agar semicolon missing hai toh ye pakdega.
    Token consume(TokenType type, std::string errorMsg);

public:
    // Constructor: Tokens load karne ke liye
    Parser(std::vector<Token> tokens);
    
    // Main Entry Point: Pura program yahan se parse hoga
    Program* parseProgram();

    // --- Recursive Descent Functions (Logic breakdown) ---
    
    // Single line statement parse karne ke liye (int a; ya a=10;)
    Statement* parseStatement();
    
    // Block parsing ke liye { ... } (abhi use nahi ho raha but structure ke liye important hai)
    Statement* parseBlock(); 

    // Math expressions handle karega (jaise a + b - c)
    Expression* parseExpression();
    
    // Basic units handle karega (Single number ya Variable name)
    Expression* parseTerm(); 
};

#endif