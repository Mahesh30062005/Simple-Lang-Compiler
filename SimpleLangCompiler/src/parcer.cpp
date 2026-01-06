#include "parser.h"
#include <iostream>

using namespace std;

// Constructor: Tokens receive karke position reset karte hain
Parser::Parser(vector<Token> t) {
    tokens = t;
    currentPos = 0;
}

// Helper: Current token return karta hai.
// Safety check: Agar end of file (EOF) cross ho gaya toh last token hi return karo.
Token Parser::peek() {
    if (currentPos >= tokens.size()) return tokens.back(); 
    return tokens[currentPos];
}

// Helper: Aage badhne ke liye. 
// Pehle increment karte hain, fir purana wala return karte hain (standard logic).
Token Parser::advance() {
    if (currentPos < tokens.size()) currentPos++;
    return tokens[currentPos - 1];
}

// Helper: Match and advance logic
bool Parser::match(TokenType type) {
    // Agar current token expected type ka hai, toh usse kha jao (advance)
    if (peek().type == type) {
        advance();
        return true;
    }
    return false;
}

// Helper: Strict Validation
// Agar expected token nahi mila toh program yahin phat jayega (Error print karke exit).
Token Parser::consume(TokenType type, string errorMsg) {
    if (peek().type == type) return advance();
    
    // Error logging nicely
    cerr << "[Parser Error] " << errorMsg << " | Found: " << peek().value << endl;
    exit(1);
}


// Main Function: Pura Program Parse Karo

Program* Parser::parseProgram() {
    Program* prog = new Program();
    
    // Jab tak file khatam nahi hoti (EOF), tab tak statements parse karte raho
    while (peek().type != TOK_EOF) {
        prog->statements.push_back(parseStatement());
    }
    return prog;
}


// Statement Logic: Decide karo kya karna hai (Decl, If, Assign)

Statement* Parser::parseStatement() {
    
    // 1. Variable Declaration Logic (e.g., "int a;")
    // Agar 'int' keyword dikha, matlab naya variable ban raha hai.
    if (match(TOK_INT)) {
        Token varName = consume(TOK_ID, "Variable ka naam missing hai 'int' ke baad.");
        consume(TOK_SEMICOLON, "Line end karne ke liye ';' lagana bhul gaye.");
        return new VarDecl(varName.value);
    }

    // 2. If Condition Logic (e.g., "if (a == b) { ... }")
    if (match(TOK_IF)) {
        consume(TOK_LPAREN, "'if' ke baad '(' missing hai.");
        Expression* condition = parseExpression(); // Condition check karo
        consume(TOK_RPAREN, "Condition ke baad ')' lagana padega.");
        
        consume(TOK_LBRACE, "Block start karne ke liye '{' chahiye.");
        
        vector<Statement*> bodyStatements;
        // Jab tak closing brace '}' ya EOF nahi milta, body parse karte raho
        while (peek().type != TOK_RBRACE && peek().type != TOK_EOF) {
            bodyStatements.push_back(parseStatement());
        }
        
        consume(TOK_RBRACE, "Block close karne ke liye '}' missing hai.");
        
        return new IfStmt(condition, bodyStatements);
    }

    // 3. Assignment Logic (e.g., "a = 10;")
    // Agar koi variable name (ID) dikha start mein, toh probably assignment hai.
    if (peek().type == TOK_ID) {
        Token varName = advance(); // Variable name save karlo
        
        // Ab expect kar rahe hain ki '=' aayega
        consume(TOK_ASSIGN, "Variable ke baad '=' expect kiya tha.");
        
        Expression* value = parseExpression(); // Right side solve karo
        
        consume(TOK_SEMICOLON, "Assignment ke baad ';' missing hai.");
        return new Assignment(varName.value, value);
    }

    // Agar kuch bhi samajh nahi aaya toh error throw karo
    cerr << "[Parser Error] Unexpected token: " << peek().value << endl;
    exit(1);
}


// Expression Parsing: Maths handle karta hai (Recursive)

Expression* Parser::parseExpression() {
    // Left side parse karo (Term)
    Expression* leftExpr = parseTerm();

    // Ab check karo ki aage koi operator (+, -, ==) hai kya.
    // Jab tak operators mil rahe hain, tree build karte jao.
    while (peek().type == TOK_PLUS || peek().type == TOK_MINUS || peek().type == TOK_EQ) {
        string op = advance().value; // Operator utha lo
        Expression* rightExpr = parseTerm(); // Right side parse karo
        
        // Purane left aur naye right ko milake naya BinaryOp node banao
        leftExpr = new BinaryOp(leftExpr, op, rightExpr);
    }

    return leftExpr;
}

// Term Parsing: Basic Numbers ya Variables

Expression* Parser::parseTerm() {
    // Agar number hai (e.g., 50)
    if (peek().type == TOK_NUM) {
        return new Number(stoi(advance().value));
    }
    
    // Agar variable hai (e.g., 'a')
    if (peek().type == TOK_ID) {
        return new Variable(advance().value);
    }
    
    // Agar yahan pahuche matlab syntax galat hai
    cerr << "[Parser Error] Number ya Variable expect kiya tha, but mila: " << peek().value << endl;
    exit(1);
}