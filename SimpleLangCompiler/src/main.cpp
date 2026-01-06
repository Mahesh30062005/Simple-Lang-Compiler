#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "lexer.h"
#include "parser.h"
#include "codegen.h"  // Ensure this matches your actual filename!

using namespace std;

int main(int argc, char* argv[]) {
    // 1. Argument Check
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <source_file.sl>" << endl;
        return 1;
    }

    string filename = argv[1];
    
    // 2. Read File
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "[Error] File open nahi ho payi: '" << filename << "'" << endl;
        return 1;
    }

    // Read entire file into string
    stringstream buffer;
    buffer << file.rdbuf();
    string sourceCode = buffer.str();
    file.close();

    cout << "Compiling " << filename << "..." << endl;

    // 3. The Pipeline
    try {
        // Phase 1: Lexing (Text -> Tokens)
        Lexer lexer(sourceCode);
        vector<Token> tokens = lexer.tokenize();

        // Phase 2: Parsing (Tokens -> AST)
        Parser parser(tokens);
        Program* program = parser.parseProgram();

        // Phase 3: Code Generation (AST -> Assembly)
        CodeGen codegen;
        codegen.generate(program);
        
        // Output to 'output.asm'
        codegen.saveToFile("output.asm");
        
        cout << "Build Successful! Assembly saved to 'output.asm'" << endl;
        
        // Cleanup
        delete program; 

    } catch (const exception& e) {
        cerr << "[Compiler Crash] " << e.what() << endl;
        return 1;
    }

    return 0;
}