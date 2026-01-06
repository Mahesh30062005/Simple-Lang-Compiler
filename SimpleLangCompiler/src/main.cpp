//   1) verify lexer.h , lexer.cpp, works perfectly

// #include <iostream>
// #include <vector>
// #include "lexer.h"

// using namespace std;

// int main() {
//     // Test Case: Variable declaration and assignment
//     string sourceCode = "int a = 100; // This is a comment";
    
//     Lexer lexer(sourceCode);
//     vector<Token> tokens = lexer.tokenize();

//     cout << "--- Lexer Output ---" << endl;
//     for (const Token& t : tokens) {
//         cout << "Type: " << t.type << " | Value: " << t.value << endl;
//     }

//     return 0;
// }


//   2) verify parser.h , parser.cpp, works perfectly

// #include <iostream>
// #include <vector>
// #include "lexer.h"
// #include "parser.h"
// #include "AbstractSynTree.h"

// using namespace std;

// int main() {
//     // Test Input
//     string sourceCode = "int a; a = 10; int b; b = a + 5;";
    
//     cout << "Compiling: " << sourceCode << endl;

//     // 1. Lexer
//     Lexer lexer(sourceCode);
//     vector<Token> tokens = lexer.tokenize();

//     // 2. Parser
//     Parser parser(tokens);
//     Program* program = parser.parseProgram();

//     cout << "Parsing Completed Successfully!" << endl;
//     cout << "Found " << program->statements.size() << " statements." << endl;

//     return 0;
// }





//  3) verify codegen.h , codegen.cpp, works perfectly


// #include <iostream>
// #include <vector>
// #include "lexer.h"
// #include "parser.h"
// #include "codeGeneration.h"

// using namespace std;

// int main(int argc, char* argv[]) {
//     // 1. Get Input
//     string sourceCode;
//     if (argc > 1) {
//         // If file provided (future step), read it. 
//         // For now, let's hardcode the test case from the prompt.
//         sourceCode = "int a; int b; int c; a = 10; b = 20; c = a + b;";
//     } else {
//         sourceCode = "int a; int b; int c; a = 10; b = 20; c = a + b;";
//     }

//     cout << "--- Compiling SimpleLang ---" << endl;
//     cout << "Source: " << sourceCode << endl;

//     // 2. Lexing
//     Lexer lexer(sourceCode);
//     vector<Token> tokens = lexer.tokenize();

//     // 3. Parsing
//     Parser parser(tokens);
//     Program* program = parser.parseProgram();
    
//     // 4. Code Generation
//     CodeGen codegen;
//     codegen.generate(program);
    
//     // 5. Output
//     codegen.saveToFile("output.asm");
    
//     cout << "--- Success! ---" << endl;
//     cout << "Check output.asm for the assembly code." << endl;

//     return 0;
// }


//    4) integrate all together and test


#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "lexer.h"
#include "parser.h"
#include "codeGeneration.h"

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
        cerr << "Error: Could not open file '" << filename << "'" << endl;
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
        // Phase 1: Lexing
        Lexer lexer(sourceCode);
        vector<Token> tokens = lexer.tokenize();

        // Phase 2: Parsing
        Parser parser(tokens);
        Program* program = parser.parseProgram();

        // Phase 3: Code Generation
        CodeGen codegen;
        codegen.generate(program);
        
        // Output to 'output.asm' (Or name it based on input)
        codegen.saveToFile("output.asm");
        
        cout << "Build Successful! Output saved to 'output.asm'" << endl;
        
        // Clean up memory (Optional but good practice)
        delete program; 

    } catch (const exception& e) {
        cerr << "Compiler Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}