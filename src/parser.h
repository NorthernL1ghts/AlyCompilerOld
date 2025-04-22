#ifndef COMPILER_PARSER_H
#define COMPILER_PARSER_H

#include <error.h>
#include <stddef.h>

typedef struct Environment Environment;

typedef struct Token {
	char* beginning;
	char* end;
} Token;

void print_token(Token t);

Error lex(char* source, Token* token);

typedef enum NodeType {
  // BEGIN LITERALS

  /// The definition of nothing; false, etc.
  NODE_TYPE_NONE,

  /// Just an integer.
  NODE_TYPE_INTEGER,

  /// When a literal is expected but no other literal is valid,
  /// it becomes a symbol.
  NODE_TYPE_SYMBOL,

  // END LITERALS

  /// Contains two children.
  /// 1. SYMBOL (VARIABLE IDENTIFIER)
  /// 2. INITIALIZE EXPRESSION, or None.
  NODE_TYPE_VARIABLE_DECLARATION,
  NODE_TYPE_VARIABLE_DECLARATION_INITIALIZED,

  /// Contains two children.
  /// 1. SYMBOL (VARIABLE IDENTIFIER)
  /// 2. VALUE EXPRESSION
  NODE_TYPE_VARIABLE_REASSIGNMENT,

  /// Contains two children that determine left and right acceptable
  /// types.
  NODE_TYPE_BINARY_OPERATOR,
  NODE_TYPE_PROGRAM,

  /// Contains a list of expressions to execute in sequence.
  NODE_TYPE_MAX,
} NodeType;

typedef struct Node {
  	// TODO: Think about how to document node types and how they fit in the AST.
	int type;
	union NodeValue {
		long long integer;
		char* symbol;
	} value;
	// Possible TODO: Parent?
	struct Node* children;
	struct Node* next_child;
} Node;

Node* node_allocate();

#define nonep(node) ((node).type == NODE_TYPE_NONE)
#define integerp(node) ((node).type == NODE_TYPE_INTEGER)
#define symbolp(node) ((node).type == NODE_TYPE_SYMBOL)

/// PARENT is modified, NEW_CHILD pointer is verbatim.
void node_add_child(Node* parent, Node* new_child);

/// @return Boolean-like value; 1 for success, 0 for failure.
int node_compare(Node* a, Node* b);

/// Create a new node within integer type and given value.
Node* node_integer(long long value);

// TODO: Think about caching used symbols and not creating duplicates!
/// Create a new node within symbol type and given value.
Node* node_symbol(char* symbol_string);

/// Create a new node within symbol type and value copied from given buffer.
Node* node_symbol_from_buffer(char* buffer, size_t length);

void print_node(Node* node, size_t indent_level);

void node_free(Node* root);

/// Copy A into B, assertign allocations.
void node_copy(Node* a, Node* b);

/// @return Boolean-like value; 1 for success, 0 for failure.
int token_string_equalp(char* string, Token* token);

/// @return Boolean-like value; 1 upon success, 0 for failure.
int parse_integer(Token* token, Node* node);

typedef struct ParsingContext {
  // FIXME: "struct ParsingContext* parent;" ???
  /// TYPE
  /// `-- SYMBOL (IDENTIFIER) -> TYPE (NODE_TYPE)
  /// 	                         `-- BYTE_SIZE (N)
	Environment* types;
	Environment* variables;
} ParsingContext;

ParsingContext* parse_context_create();

Error parse_expr(ParsingContext* context, char* source, char** end, Node* result);

#endif /* COMPILER_PARSER_H */