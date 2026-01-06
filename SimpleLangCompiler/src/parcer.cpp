#include "parser.h"
#include <iostream>

using namespace std;

Parser::Parser(vector<Token> tokens) {
    this->tokens = tokens;
    this->pos = 0;
}

Token Parser::peek() {
    if (pos >= tokens.size()) return tokens.back(); // Return EOF if OOB
    return tokens[pos];
}

Token Parser::advance() {
    if (pos < tokens.size()) pos++;
    return tokens[pos - 1];
}

bool Parser::match(TokenType type) {
    if (peek().type == type) {
        advance();
        return true;
    }
    return false;
}

Token Parser::consume(TokenType type, string errorMsg) {
    if (peek().type == type) return advance();
    cerr << "Parser Error: " << errorMsg << " at " << peek().value << endl;
    exit(1);
}

// ---------------------------------------------------------
// Top Level: Parse the whole file
// ---------------------------------------------------------
Program* Parser::parseProgram() {
    Program* program = new Program();
    while (peek().type != TOK_EOF) {
        program->statements.push_back(parseStatement());
    }
    return program;
}

// ---------------------------------------------------------
// Statements: Declarations, Assignments, Ifs
// ---------------------------------------------------------
Statement* Parser::parseStatement() {
    // 1. Variable Declaration: "int a;"
    if (match(TOK_INT)) {
        Token id = consume(TOK_ID, "Expected variable name after 'int'");
        consume(TOK_SEMICOLON, "Expected ';' after variable declaration");
        return new VarDecl(id.value);
    }

    // 2. If Statement: "if (expr) { ... }"
    if (match(TOK_IF)) {
        consume(TOK_LPAREN, "Expected '(' after 'if'");
        Expression* condition = parseExpression();
        consume(TOK_RPAREN, "Expected ')' after condition");
        
        consume(TOK_LBRACE, "Expected '{' to start if-block");
        vector<Statement*> body;
        while (peek().type != TOK_RBRACE && peek().type != TOK_EOF) {
            body.push_back(parseStatement());
        }
        consume(TOK_RBRACE, "Expected '}' to end if-block");
        
        return new IfStmt(condition, body);
    }

    // 3. Assignment: "a = 5;"
    // Check if it starts with an identifier (variable name)
    if (peek().type == TOK_ID) {
        Token id = advance(); // Eat the variable name
        consume(TOK_ASSIGN, "Expected '=' after variable name");
        Expression* value = parseExpression();
        consume(TOK_SEMICOLON, "Expected ';' after assignment");
        return new Assignment(id.value, value);
    }

    cerr << "Parser Error: Unexpected token " << peek().value << endl;
    exit(1);
}

// ---------------------------------------------------------
// Expressions: Math logic (Left + Right)
// ---------------------------------------------------------
Expression* Parser::parseExpression() {
    // Parse the left side first (e.g., "5")
    Expression* left = parseTerm();

    // While we see "+" or "-", keep building the tree
    while (peek().type == TOK_PLUS || peek().type == TOK_MINUS || peek().type == TOK_EQ) {
        string op = advance().value; // Eat the operator
        Expression* right = parseTerm(); // Parse the right side
        // Combine them: (OldLeft + Right)
        left = new BinaryOp(left, op, right);
    }

    return left;
}

// Term: A single unit (Number or Variable)
Expression* Parser::parseTerm() {
    if (peek().type == TOK_NUM) {
        return new Number(stoi(advance().value));
    }
    if (peek().type == TOK_ID) {
        return new Variable(advance().value);
    }
    
    cerr << "Parser Error: Expected Number or Variable, got " << peek().value << endl;
    exit(1);
}