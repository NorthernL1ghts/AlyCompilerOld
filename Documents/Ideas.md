# Ideas

1. **Identifier Management**
   - Implement a structured list of known identifiers. Ideally, include:
     - Type information
     - Literal value / memory address (or equivalent representation)
   - Scope-aware to support nested contexts (e.g., function shadowing of global identifiers).
   - This could facilitate implementing `let` bindings, which would be a nice addition.

2. **Runtime Evaluator vs. Compiler**
   - Consider building both a runtime evaluator and a compiler.
   - Feasibility assessment required to determine if we can support both implementations.

## Miscellaneous Features & Considerations

1. **Statically Typed**
2. **Compiled Language**
3. **Whitespace Agnostic**
4. **Environments as Nodes**
   - Environments could themselves be nodes, allowing for Lisp-like recursive structures.
5. **Prioritizing Simplicity**
   - Keep language design minimalistic and focused.
6. **Potential Transpilation**
   - Consider transpiling to C or another lower-level language in the future.
7. **Top-Level Declarations**
   - Evaluate whether to allow top-level function declarations like Python (e.g., `foo()`).
   - Not sure if this aligns with our design philosophy.

## Identifier Structure Proposal

A simple linked-list representation of scoped identifiers:

```plaintext
[  [  ID -> TYPE  ], [  "a" -> integer ]  ]
```

## Example Execution Tree Representation

```plaintext
================================
        ROOT
         │
         ├── /
         │    ├── integer
         │    │     └── 34
         │    │
         │    ├── +
         │    │
         │    └── integer
         │          └── 35

        ROOT-x
         │
         ├── /
         │    └── +
         │         ├── integer
         │         │     └── 34
         │         │
         │         └── integer
         │               └── 35

        ROOT-x
         │
         ├── ?
         │    ├── condition
         │    │
         │    ├── integer
         │    │     └── 69
         │    │
         │    └── integer
         │          └── 420
================================
```

This structure supports arithmetic operations and conditional expressions while maintaining clear identifier scoping.

CONTEXT
   TYPES
      TYPE_SYMBOL -> TYPE INFO OF SOME SORT
                     `-- SIZE IN BYTES
   VARIABLES
      VAR_SYMBOL -> TYPE SYMBOL

## AlyCompiler AT&T x86_64 GNU ASM

```
   | 1 | <- RBP (1)
   | 1 |
   | 1 | <- [RBP (1)]
   | 1 | <- RSP <- RBP
   | 1 |
   | 1 |
   | 1 |
```