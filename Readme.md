# SimpleLang Compiler - 8-bit CPU Project

Welcome to the **SimpleLang Compiler**. Basically, yeh project ek custom compiler hai jo High-Level language (`.sl`) ko read karta hai aur usse 8-bit CPU ke liye Assembly code (`.asm`) mein convert karta hai.

This was built as part of the **Vicharak Internship Task**.

---

## 🧐 Why this Project? (Kyun banaya?)

Assignment simple tha: Ek language banao aur usse hardware pe run karo.
But the challenge was that the **8-bit CPU is extremely dumb**. It doesn't understand complex math or loops. It only understands simple instructions like `LDA` (Load), `ADD`, and `STA` (Store).

So, maine yeh compiler isliye banaya taaki hum human-readable code likh sakein (like `c = a + b`) aur machine ko manually hex codes feed na karne padein.

---

## 📂 Project Structure (Folder mein kya hai?)

- **`src/`**: Yahan saara source code hai.
  - `lexer.cpp` / `.h`: **The Eyes.** Yeh text file ko padhta hai aur tokens banata hai (Words pehchanta hai).
  - `parser.cpp` / `.h`: **The Brain.** Yeh check karta hai ki grammar sahi hai ya nahi (AST banata hai).
  - `codegen.cpp` / `.h`: **The Translator.** Yeh final Assembly code generate karta hai.
- **`tests/`**: Yahan humare sample programs hain (`test1.sl`).
- **`output.asm`**: Final assembly file jo generate hoti hai.

---

## ⚙️ How It Works (Kaam kaise karta hai?)

Compiler 3 steps mein kaam karta hai:

1.  **Lexing (Tokenizing):**
    Code ko todna.
    *Input:* `int a = 10;`
    *Output:* `[INT, ID(a), ASSIGN, NUM(10), SEMICOLON]`
    *Logic:* Maine isme comments (`//`) handle kiye hain taaki hum notes likh sakein.

2.  **Parsing (Understanding):**
    Ab compiler grammar check karta hai.
    *Logic:* Recursive Descent Parser use kiya hai. Basically, function ke andar function call hota hai. Agar `int` likha hai, toh compiler expect karega ki next word ek variable name hona chahiye. Agar nahi mila, toh error throw karega.

3.  **Code Generation (Translation):**
    Yeh sabse important part hai.
    Target CPU ke paas sirf ek register hai (**Accumulator**). Toh agar hume `a + b` karna hai, hum direct add nahi kar sakte.
    *My Logic:*
    - Pehle `b` ki value ko load karo.
    - Usko ek temporary address (**Address 255**) pe save kar do.
    - Phir `a` ko load karo.
    - Aur finally `ADD 255` instruction chala do.
    - **So, Address 255 humara "Scratchpad" hai.**

---

## 🚀 How to Run (Chalayen kaise?)

Sabse pehle code compile karo using `g++`:

```bash
g++ src/main.cpp src/lexer.cpp src/parser.cpp src/codegen.cpp -o simplec