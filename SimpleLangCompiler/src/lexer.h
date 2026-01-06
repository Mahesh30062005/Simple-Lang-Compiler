#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>

// The vocabulary of SimpleLang
enum TokenType {
    TOK_INT,        // "int"
    TOK_ID,         // variable names like "a", "count"
    TOK_NUM,        // numbers like "10", "255"
    TOK_ASSIGN,     // "="
    TOK_PLUS,       // "+"
    TOK_MINUS,      // "-"
    TOK_IF,         // "if"
    TOK_LBRACE,     // "{"
    TOK_RBRACE,     // "}"
    TOK_LPAREN,     // "("
    TOK_RPAREN,     // ")"
    TOK_SEMICOLON,  // ";"
    TOK_EOF,        // End of File
    TOK_UNKNOWN,    // Errors
    TOK_EQ          // "=="

};

// A single word in the language
struct Token {
    TokenType type;
    std::string value; // Stores the actual text (e.g., "10", "a")
};

class Lexer {
private:
    std::string src;  // The source code
    int pos;          // Current position in the string

    char peek();      // Look at current char
    char advance();   // Eat current char and move to next

public:
    Lexer(std::string source);
    std::vector<Token> tokenize(); // The main engine
};

#endif