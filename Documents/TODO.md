# AlyCompiler TODO

## Top-Level Tasks

- [x] **Lexing**
  Convert source code into tokens.

- [x] **Parsing**
  Transform tokens into an AST (Abstract Syntax Tree) — a structure representing the program.

- [ ] **Compilation**
  Compile the AST into x86_64 assembly.
  Alternatively, consider trans-piling into a language like C and compiling from there.

## Urgent

- [ ] **Have Compiler able to collect TODO comments and things from source**
    - This would allow compiler-aided software design in more scenarios.

- [ ] **TODO: Add token member to AST node structure**
 - This would allow for much better error messages that point to the source code that the error originates from.

 - We should also think about line count or something like that; maybe node AST holds custom structure containing token span, as well as file path (if applicable), line number, column number, etc.

- [ ] **Bind all arguments to a function, not just the first one!**
  - `param_it` needs fixed in `parser.c` around line 570.

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

- [x] **Assertions Not Working**
  - The `NODE_TYPE_MAX` assertions may not be firing. Might need to cast enum values to integers.

## Parsing & Lexing Architecture

- [ ] **State Machine Parser**
  - As complexity grows, consider using a state machine to handle edge cases and special parsing logic.
  - For now, current approach may be simpler and faster to implement.

## Testing and Tooling

- [ ] **Write End-to-End Tests in Algol**
  - Step 1: Learn Algol.
  - Step 2: Write tests in it.

# Misc

- [ ] Implement `environment_free()`.

- [ ] **Make CodeGen structure with function pointers and such**
  - This would allow each implementation to actually be generated the same way, just have slightly different semantics.

  - It's kind of the same thing we're doing now, just moved into structure data instead of switch case in function.

- [ ] **Add an environment to the parsing context that contains operators to handle**


-  [ ] **Convert `Environment` type in an AST Node**
  - This mimics LISP much more closely, and also doesn't change anything from out current linked list implementation.

  - I think the idea is just to store a list of binding in a node, each of which contains a left side (key) and a right side (value). We can then just rewrite our environment code to operate on those structures, instead of on the custom Environment structure. I believe this will also allow for other things that I just can't think of right now. Oh yeah! The environment will then be able to be freed and/or garbage collected in the same way as the rest of the nide, which greatly simplifies the memory management :^)

- [x] **Compile with `--Wall` and `--Wextra`, fix warnings!**
  - I haven't done this and the codebase is somewhat sizeable; if hunting for something to do, compile with extra flags and fix warnings.

- [ ] **Have the ability to pass arguments in any order, if named**
 - The definition must contain parameter ID's in order for local scope binding to work.

 - Say `diff` is a function with two integer arguments, `X` and `Y`, with an integer return value that is the result of a subtract b.

- `diff(9, 6)` would return 3, as expected.
- `diff(y: 9, 6)` however, would return -3 as expected.