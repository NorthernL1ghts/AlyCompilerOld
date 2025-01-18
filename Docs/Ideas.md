# IDEAS

1. We need some list of identifiers that are known, ideally with type information as well as the literal value / memory address or something. These need to be scoped (i.e. nested) to allow for shadowing of global identifiers within functions for example. This may even allow use to implement `let` which would be cool.

# Misc
1. Statically typed
2. Compiled
3. Whitespace agnostic
4. Environments could be a Node itself, that would mean we could have recursive pairs like Lisp.
5. Simplicity over anything else.
6. Maybe we transpile to C or another language later?
7. Top level declarations like foo() like Python does? Not sure I like that!

I suppose this could be done as a very simple linked list

[  [  ID -> TYPE  ], [  "a" -> integer ]  ]

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
