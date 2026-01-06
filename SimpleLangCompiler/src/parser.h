#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "AbstractSynTree.h"
#include <vector>

class Parser {
private:
    std::vector<Token> tokens;
    int pos;
    
    // Helper: Get current token
    Token peek();
    // Helper: Eat current token and move forward
    Token advance();
    // Helper: Check if current token matches expected type
    bool match(TokenType type);
    // Helper: Require a specific token or crash (e.g., missing semicolon)
    Token consume(TokenType type, std::string errorMsg);

public:
    Parser(std::vector<Token> tokens);
    
    // The Main entry point
    Program* parseProgram();

    // Recursive Descent Functions
    Statement* parseStatement();
    Statement* parseblock(); // Handles { ... }
    Expression* parseExpression();
    Expression* parseTerm(); // Handles single numbers or variables
};

#endif