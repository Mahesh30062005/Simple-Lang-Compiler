#include "lexer.h"
#include <cctype>   // For isalpha, isdigit, isspace
#include <iostream> // For error logging

// Constructor
Lexer::Lexer(std::string source) {
    src = source;
    pos = 0;
}

// Look at the current character without consuming it
char Lexer::peek() {
    if (pos >= src.length()) return '\0';
    return src[pos];
}

// Consume the current character and move forward
char Lexer::advance() {
    if (pos >= src.length()) return '\0';
    return src[pos++];
}

// The Main Engine
std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;

    while (pos < src.length()) {
        char current = peek();

        // 1. Skip Whitespace (Spaces, tabs, newlines)
        if (isspace(current)) {
            advance();
            continue;
        }

        // 2. Handle Comments (// ...)
        // We peek ahead to see if it is a double slash
        if (current == '/' && pos + 1 < src.length() && src[pos + 1] == '/') {
            // Consume the two slashes
            advance(); 
            advance(); 
            // Skip everything until the end of the line
            while (peek() != '\n' && peek() != '\0') {
                advance();
            }
            continue; // Go back to start of loop
        }

        // 3. Handle Identifiers (Variables) and Keywords ("int", "if")
        if (isalpha(current)) {
            std::string word;
            // Read until it's no longer a letter or number (e.g., "count1")
            while (isalnum(peek())) { 
                word += advance();
            }

            // Keyword Check
            if (word == "int") {
                tokens.push_back({TOK_INT, word});
            } else if (word == "if") {
                tokens.push_back({TOK_IF, word});
            } else {
                tokens.push_back({TOK_ID, word}); // It's a variable name
            }
            continue;
        }

        // 4. Handle Numbers (Digits)
        if (isdigit(current)) {
            std::string numStr;
            while (isdigit(peek())) { 
                numStr += advance();
            }
            tokens.push_back({TOK_NUM, numStr});
            continue;
        }

        // 5. Handle Operators and Punctuation
        switch (current) {
            case '=': 
            // Check if the NEXT character is also '=' (creating "==")
            if (pos + 1 < src.length() && src[pos + 1] == '=') {
                advance(); // Eat the first '='
                advance(); // Eat the second '='
                tokens.push_back({TOK_EQ, "=="});
            } else {
                // It's just a single '=' assignment
                tokens.push_back({TOK_ASSIGN, "="});
                advance(); // Eat the single '='
            }
            break;

                


            
            case '+': tokens.push_back({TOK_PLUS, "+"}); advance(); break;
            case '-': tokens.push_back({TOK_MINUS, "-"}); advance(); break;
            case ';': tokens.push_back({TOK_SEMICOLON, ";"}); advance(); break;
            case '{': tokens.push_back({TOK_LBRACE, "{"}); advance(); break;
            case '}': tokens.push_back({TOK_RBRACE, "}"}); advance(); break;
            case '(': tokens.push_back({TOK_LPAREN, "("}); advance(); break;
            case ')': tokens.push_back({TOK_RPAREN, ")"}); advance(); break;
            
            default:
                std::cerr << "Lexer Error: Unknown character '" << current << "'" << std::endl;
                advance(); // Skip to avoid infinite loop
                break;
        }
    }

    tokens.push_back({TOK_EOF, ""});
    return tokens;
}