# BUG FIXES

## Assertions & Debugging

- [x] Investigated and resolved assertion failure for `NODE_TYPE_MAX`.
  - Issue was due to `NODE_TYPE_MAX` being moved in the `enum`, causing unexpected behavior in `CASSERT`.
  - Differences between C and C++ assertion behavior were identified and accounted for.

## Memory Management

- [x] **Fixed `file_contents()` to check for `NULL` return value from `malloc()`.**

## Token Handling

- [x] **Refactored tokens to remove unnecessary linked lists and `create/free` functions.**
- [x] **Removed `print_tokens()` as it was no longer needed.**

## Lexing

_TODO: Add lexing-specific bug fixes here._

## Data Types

- [x] **Removed `integer_t` in favor of `long long`.**

## Parsing

- [x] **Refactored `parse_expr()` to:**
  - Accept `char* source`, `char** end`, and `Node* result`.
  - Eliminate `working_node` and use `result` directly.
  - Remove root node allocation—handled at a higher level.
  - **Implemented parsing using a `while` loop** instead of handling a single expression.

---

# Other Considerations

## Parsing Context

- Explored how the parser could parse **within and into a given parsing context.**
- Implemented a **parsing context** managing multiple environments:
  - **Type & Variable Definitions** → Ensures symbols are properly defined and typed.
  - **Operator Environment** → Maps operators to their respective functions.

## Operator Handling

- Operators were classified into categories such as:
  - Prefix unary, postfix unary
  - Prefix binary, infix binary, postfix binary
  - Additional classifications where necessary
- **Implemented a clean and maintainable way to define new operators programmatically.**
