# BUG FIXES

- [ ] Look into why assert for `NODE_TYPE_MAX` is failing and overall not asserting the issues.
      - In VS, it worked at runtime but resulted in an aborted message.
      - In VSC, it doesn't throw any issue.
      - Further code inspection or environment setup review is needed.
      - Likely cause: `NODE_TYPE_MAX` was initially the third item in the `enum` but was moved to the seventh position.
      - In C, `CASSERT` moves up rather than down, unlike in C++, which may be causing the issue.

## Memory Management

- [x] **`file_contents()` does not check for `NULL` return value from `malloc()`.**

## Token Handling

- [x] **Tokens do not need to be linked lists** or have `create/free` functions.
- [x] **Remove `print_tokens()` while we are at it.**

## Lexing

## Data Types

- [x] **Get rid of `integer_t`.** Just use `long long` and get on with it.

## Parsing

- [x] **Completely change `parse_expr()`:**
  - Accept `char* source`, `char** end`, and `Node* result`.
  - Get rid of `working_node` completely—use `result` instead.
  - Remove root node allocation; it will be handled at a higher level. Just use `result`.
  - **Parse in a `while` loop** instead of parsing a single expression.

---

# Other

## Parsing Context

- Think about how the parser could parse **within and into a given parsing context.**
- A **parsing context** could contain multiple environments for:
  - **Types & Defined Variables** → Ensures symbols are defined and have proper types.
  - **Operator Environment** → Maps operators to functions that apply them.

## Operator Handling

- Operators need classifications such as:
  - Prefix unary, postfix unary
  - Prefix binary, infix binary, postfix binary
  - And possibly more...
- How do we **programmatically define a new operator** in a way that doesn’t suck?
