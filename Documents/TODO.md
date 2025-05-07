# AlyCompiler TODO (Prioritized by Urgency)

## 🔥 Urgent 🔥

- [ ] **TODO: Enhance AST Node with Token Information for Error Reporting**
  - **Details:** Currently, error messages likely lack precise source location information. To improve debugging, each AST node should store a reference to the token(s) from which it was derived.
  - **Implementation Considerations:**
    - Add a `token` member (likely a pointer to the `Token` structure) to the `ASTNode` structure.
    - Consider a more comprehensive `SourceLocation` structure containing:
      - `filepath` (if applicable, for multi-file projects)
      - `line_number`
      - `column_number`
      - `token_span` (start and end character indices within the line)
    - Update the parser to populate this information when creating AST nodes.
  - **Urgency:** High. Significantly improves the user experience by providing actionable error messages.

- [ ] **LISP Runtime Implementation**
  - **Details:** To execute the compiled LISP-like language, a runtime environment is necessary. This includes fundamental data structures (lists, symbols, etc.) and functions for basic operations (evaluation, function calls, etc.).
  - **Implementation Considerations:**
    - Define the core data structures needed at runtime.
    - Implement essential functions that will be called by the compiled code.
    - Consider memory management within the runtime.
  - **Urgency:** High. Essential for making the compiled output executable.

## ⚙️ Top-Level Tasks ⚙️

- [x] **Lexing**
  - **Description:** The initial phase of the compiler that converts raw source code into a stream of tokens. Each token represents a basic building block of the language (keywords, identifiers, operators, literals).

- [x] **Parsing**
  - **Description:** The second phase that takes the token stream and constructs an Abstract Syntax Tree (AST). The AST is a hierarchical representation of the program's structure, making it easier for subsequent compilation stages to process.

- [ ] **Compilation**
  - **Description:** The core task of translating the AST into low-level machine code (x86_64 assembly in this case) or an intermediate representation.
  - **Alternative:** Transpiling to a more widely supported language like C could simplify the backend development by leveraging existing compilers.
  - **Considerations:**
    - Define the target instruction set or intermediate language.
    - Implement the logic to traverse the AST and generate corresponding code.
    - Handle register allocation, memory management, and other low-level details.

## ✨ AST and Token Enhancements ✨

- [ ] **Attach Comprehensive Metadata to AST Nodes**
  - **Details:** Expanding on the urgent TODO, this involves consistently attaching detailed source location information to all relevant AST nodes.
  - **Benefits:** Enables highly precise error reporting, supports debugging tools, and could be useful for static analysis.
  - **Data Structure:** As discussed above, a `SourceLocation` structure containing file path, line number, column number, and token span is recommended.

## 💡 Language Features and Ideas 💡

### Type System

- [ ] **Automatic Type Deduction**
  - **Details:** Allow the compiler to infer the type of a variable based on its initialization value, reducing the need for explicit type declarations in some cases.
  - **Example Syntax:** `a :| = 69` (the `:|` could signify type inference).
  - **Implementation:** Requires analyzing the right-hand side of assignments to determine the type.

- [ ] **Uninitialized Variables**
  - **Details:** Explore allowing variable declarations without an initial value. This introduces the concept of potentially undefined behavior if such variables are used before assignment.
  - **Possible Syntax:**
    - `a : integer !` (the `!` could indicate no initial value).
    - `a : integer = None` (requires parser to recognize `None` in this context).
  - **Considerations:** How will the compiler handle or warn about the use of uninitialized variables?

- [ ] **Any Type**
  - **Details:** Introduce a universal type that can hold values of any other type. This can be useful for creating generic data structures or interacting with external systems.
  - **Implementation Challenges:** Requires mechanisms for type checking or runtime type identification to ensure safe operations on `Any` type values.

- [ ] **Meta-Types**
  - **Details:** Explore the concept of types that can represent other types. This opens possibilities for metaprogramming, where code can manipulate types themselves.
  - **Use Cases:** Reflection, code generation, advanced type system features.
  - **Complexity:** This is a more advanced feature with significant implementation challenges.

### Syntax & Declarations

- [ ] **Unnamed Variable Declarations**
  - **Details:** Investigate syntax to declare or assign to a temporary, unnamed variable, primarily affecting an implicit "environment" or context.
  - **Potential Use Cases:** Scenarios where a value is needed temporarily without needing a persistent name.

- [ ] **Have the ability to pass arguments in any order, if named**
  - **Details:** Allow function calls where arguments are provided with their corresponding parameter names, enabling out-of-order specification.
  - **Requirement:** Function definitions must include parameter identifiers to facilitate this mapping.
  - **Example:** For a function `diff(x: integer, y: integer)`, calls like `diff(y: 9, x: 6)` should be valid.

## 🎯 Alternative Compilation Targets 🎯

- [ ] **CHIP-8 ASM Backend**
  - **Inspiration:** Inspired by a CHIP-8 toolset written in PHP.
  - **Goal:** To explore the possibility of compiling the Aly language to the assembly language of the CHIP-8 virtual machine, a retro gaming platform.
  - **Challenges:** Significant differences in architecture and instruction set would require a specialized code generation phase.

- [ ] **Brainfuck Compilation**
  - **Idea:** A purely experimental and potentially impractical target. Brainfuck is an esoteric programming language with a very limited instruction set.
  - **Motivation:** A fun challenge to see if a higher-level language like Aly could be translated to such a minimal language.

## 🏗️ Parsing & Lexing Architecture 🏗️

- [ ] **Have Compiler able to collect TODO comments and things from source**
  - **Details:** Enhance the compiler to parse and report on special comments (like `TODO`, `FIXME`, etc.) within the source code.
  - **Potential Benefits:** Could be integrated into development workflows for tracking tasks and issues directly within the code.

- [ ] **State Machine Parser**
  - **Consideration:** As the language syntax becomes more complex, the current parsing approach might become harder to maintain and extend.
  - **Alternative:** A state machine-based parser could provide a more structured and robust way to handle various grammar rules and edge cases.
  - **Trade-off:** Might be more complex to initially implement compared to the current approach.

## 🧪 Testing and Tooling 🧪

- [ ] **Write End-to-End Tests in Algol**
  - **Prerequisites:**
    - Step 1: Learn the Algol programming language.
    - Step 2: Develop a suite of tests written in Algol that exercise various features of the Aly compiler and the compiled code.
  - **Goal:** Ensure the compiler produces correct output for a range of valid Aly programs.

## 🛠️ Misc 🛠️

- [ ] Implement `environment_free()`
  - **Details:** Ensure proper memory management by implementing a function to deallocate the resources associated with the `Environment` data structure. This is crucial to prevent memory leaks.

- [ ] **Refactor Code Generation with Function Pointers**
  - **Goal:** Improve the modularity and extensibility of the code generation phase.
  - **Approach:** Create a structure that holds function pointers for different code generation tasks (e.g., generating code for different AST node types). This would allow for easier addition of new target architectures or code generation strategies.

- [ ] **Integrate Operator Handling into Parsing Environment**
  - **Details:** Instead of hardcoding operator precedence and associativity within the parser, consider managing this information within the parsing environment. This could make the parser more flexible and easier to extend with new operators.

- [ ] **Convert `Environment` Type to an AST Node Structure**
  - **Rationale:** This design choice aligns more closely with LISP's approach where environments are first-class entities. Representing environments as AST nodes could simplify memory management (allowing them to be garbage collected alongside other nodes) and potentially enable more advanced language features related to scope and context.
  - **Implementation:** Each binding in the environment (key-value pair) would be represented as a child node of the environment node.

- [x] **Compile with `--Wall` and `--Wextra`, fix warnings!**
  - **Action:** Enable comprehensive compiler warnings during the build process and address all reported issues. This is essential for code quality and can help identify potential bugs or areas for improvement.