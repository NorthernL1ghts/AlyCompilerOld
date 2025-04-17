# AlyCompiler TODO

## Top-Level Tasks

- [x] **Lexing**
  Convert source code into tokens.

- [ ] **Parsing**
  Transform tokens into an AST (Abstract Syntax Tree) — a structure representing the program.

- [ ] **Compilation**
  Compile the AST into x86_64 assembly.
  Alternatively, consider trans-piling into a language like C and compiling from there.

## Urgent

- [x] **Comment Handling in Lexer**
  Skip over comments during lexing.

- [ ] **LISP Runtime**
  Implement the LISP runtime in the compiled language.

## AST and Token Enhancements

- [ ] **Attach Token Metadata to AST Nodes**
  - Enables detailed error messages that point to specific source locations.
  - Consider a structure that includes token span, file path, line number, column number, etc.

## Alternative Compilation Targets

- [ ] **CHIP-8 ASM Backend**
  - Inspired by a CHIP-8 toolset in PHP.
  - Would be interesting to target CHIP-8 as an additional backend.

- [ ] **Brainfuck Compilation**
  - A random but intriguing idea. Not sure it’s even viable, but could be fun to explore.

## Language Features and Ideas

### Type System

- [ ] **Automatic Type Deduction**
  - If we can already infer expression return types, consider supporting type-less variable declarations.
    Example syntax: `a :| = 69`

- [ ] **Uninitialized Variables**
  - Possible syntax: `a : integer !`
  - Alternative: `a : integer = None` with parser support for recognizing this as "uninitialized".

- [ ] **Any Type**
  - A universal type that accepts any other type.
  - Would likely require a way to inspect types at runtime or during compilation.

- [ ] **Meta-Types**
  - A type that can represent other types — for meta-programming or reflection-style behavior.

### Syntax & Declarations

- [ ] **Unnamed Variable Declarations**
  - Support for assigning to or declaring a variable without naming it, affecting a single unnamed environment variable.

- [ ] **Assertions Not Working**
  - The `NODE_TYPE_MAX` assertions may not be firing. Might need to cast enum values to integers.

## Parsing & Lexing Architecture

- [ ] **State Machine Parser**
  - As complexity grows, consider using a state machine to handle edge cases and special parsing logic.
  - For now, current approach may be simpler and faster to implement.

## Testing and Tooling

- [ ] **Write End-to-End Tests in Algol**
  - Step 1: Learn Algol.
  - Step 2: Write tests in it.
