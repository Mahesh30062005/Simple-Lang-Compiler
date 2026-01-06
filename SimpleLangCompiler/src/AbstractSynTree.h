#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include <iostream>

// Base class for all nodes in the tree
struct ASTNode {
    virtual ~ASTNode() = default; // Virtual destructor is crucial for cleanup
};

// Base class for Expressions (things that have a value: 5, a, a+b)
struct Expression : ASTNode {};

// Represents a raw number like "10"
struct Number : Expression {
    int value;
    Number(int v) : value(v) {}
};

// Represents a variable usage like "a" in "b = a + 5"
struct Variable : Expression {
    std::string name;
    Variable(std::string n) : name(n) {}
};

// Represents math: "left + right"
struct BinaryOp : Expression {
    Expression* left;
    std::string op; // "+" or "-"
    Expression* right;
    
    BinaryOp(Expression* l, std::string o, Expression* r) 
        : left(l), op(o), right(r) {}
};

// Base class for Statements (things that DO something)
struct Statement : ASTNode {};

// "int a;"
struct VarDecl : Statement {
    std::string name;
    VarDecl(std::string n) : name(n) {}
};

// "a = 10;"
struct Assignment : Statement {
    std::string name;
    Expression* value;
    Assignment(std::string n, Expression* v) : name(n), value(v) {}
};

// "if (cond) { body }"
struct IfStmt : Statement {
    Expression* condition;
    std::vector<Statement*> body; // A list of statements inside { }
    
    IfStmt(Expression* c, std::vector<Statement*> b) 
        : condition(c), body(b) {}
};

// The Root Node: The entire program is a list of statements
struct Program {
    std::vector<Statement*> statements;
};

#endif