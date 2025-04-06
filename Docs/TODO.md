# AlyCompiler's TODO

## Top Level

- [x] Lex source into tokens or something

- [ ] Parse tokens or something into AST — Just a tree containing data about the program

- [ ] Compile AST into assembly, let's start with x86_64? We could also transpile into another language that then gets compiled, like C or something.

## Kind of Urgent

- [ ] Lex past comments or something.

- [ ] Write LISP runtime in the compiled language.

## Dumb ideas that we could implement

- **Compile to brain-fuck XD?** — This is just a perfect idea. I am not sure or even know if it's possible. It randomly came to me as I was doing research on how Compiler's work on deeper levels.

## Miscellaneous

### Decide what the syntax is for uninitialized variables

- Maybe we do something like: ` a : integer ! `

- We could also have a specific value the parser recognizes as uninitialized, like ` a : integer = None ` or something.

### = Any = type that can accept any type

- This would be cool to be able to accept any type as a parameter. We may need a way to query what type something is, byt that is also totally doable. This may be way too complicated, but it would be cool.

### Assertions not working???

- It seems like the `NODE_TYPE_MAX` assertions are not working. Maybe I have to case the enum value to an integer or something. I don't know.

### Unnamed variable declarations

- It would be cool if an assignment and/or variable declaration could have no identifier, and work on a singular unnamed variable in the environment.

### State Machine Parser

- Just a thought, but when we lex right now, we need to go deeper. As the compiler and parser get more advanced, maybe we could use a state machine or something to handle the special cases rather than how we are currently doing it.
- Right now, I think we'll handle all the special cases in the same way we are because it's going to become so much more complicated, and that will waste time.

### Have a type that represents all the other types

- This would allow variables to be defined with a type of another type.

### Write end-to-end tests in Algol

- First, I guess I have to learn Algol XD.
