#include <parser.h>
#include <environment.h>
#include <error.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ============================================================ BEG lexer
const char* comment_delimiters = ";#";
const char* whitespace = " \r\n";
const char* delimiters = " \r\n,():";

/// @return Boolean-like value: 1 for success, 0 for failure.
int comment_at_beginning(Token token) {
	const char* comment_it = comment_delimiters;
	while (*comment_it) {
		if (*(token.beginning) == *comment_it) {
			return 1;
		}
		comment_it++;
	}
	return 0;
}

/// Lex the next token from SOURCE, and point to it with BEG and END.
/// If BEG and END of token are equal, there is nothing more to lex.
Error lex(char* source, Token* token) {
	Error err = ok;
	if (!source || !token) {
		ERROR_PREP(err, ERROR_ARGUMENTS, "Can not lex empty source.");
		return err;
	}
	token->beginning = source;
	token->beginning += strspn(token->beginning, whitespace); // Skip the whitespace at the beginning.
	token->end = token->beginning;
	if (*(token->end) == '\0') { return err; }
	// Check if current line is a comment, and skip past it.
	while (comment_at_beginning(*token)) {
		// Skip to after next newline.
		token->beginning = strpbrk(token->beginning, "\n");
		if (!token->beginning) {
			// If last line of file is comment, we're done lexing.
			token->end = token->beginning;
			return err;
		}
		// Skip to beginning of next token after comment.
		token->beginning += strspn(token->beginning, whitespace);
		token->end = token->beginning;
	}
	if (*(token->end) == '\0') { return err; }
	token->end += strcspn(token->beginning, delimiters); // Skip everything not in delimiters.
	if (token->end == token->beginning) {
		token->end += 1;
	}
	return err;
}

int token_string_equalp(char* string, Token* token) {
	if (!string || !token) { return 0; }
	char* beg = token->beginning;
	while (*string && token->beginning < token->end) {
		if (*string != *beg) { return 0; }
		string++;
		beg++;
	}
	return 1;
}

void print_token(Token t) {
	if (t.end - t.beginning < 1) {
		printf("INVALID TOKEN POINTERS\n");
	} else {
		printf("%.*s", t.end - t.beginning, t.beginning);
	}
}
// ============================================================ END lexer

Node* node_allocate() {
	Node* node = calloc(1, sizeof(Node));
	assert(node && "Could not allocate memory for AST node");
	return node;
}

void node_add_child(Node* parent, Node* new_child) {
	if (!parent || !new_child) { return; }
	if (parent->children) {
		Node* child = parent->children;
		while (child->next_child) {
			child = child->next_child;
		}
		child->next_child = new_child;
	} else {
		parent->children = new_child;
	}
}

int node_compare(Node* a, Node* b) {
	if (!a || !b) {
		if (!a && !b) { return 1; }
		return 0;
	}
	assert(NODE_TYPE_MAX == 8 && "node_compare() must handle all node types");
	if (a->type != b->type) { return 0; }
	switch (a->type) {
	case NODE_TYPE_NONE:
		if (nonep(*b)) { return 1; }
		break;
	case NODE_TYPE_INTEGER:
		if (a->value.integer == b->value.integer) {
			return 1;
		}
		break;
	case NODE_TYPE_SYMBOL:
		if (a->value.symbol && b->value.symbol) {
			if (strcmp(a->value.symbol, b->value.symbol) == 0) {
				return 1;
			}
			break;
		} else if (!a->value.symbol && !b->value.symbol) {
			return 1;
		}
		break;
	case NODE_TYPE_BINARY_OPERATOR:
		printf("TODO: node_compare() BINARY OPERATOR\n");
		break;
	case NODE_TYPE_VARIABLE_REASSIGNMENT:
		printf("TODO: node_compare() VARIABLE REASSIGNMENT\n");
		break;
	case NODE_TYPE_VARIABLE_DECLARATION:
		printf("TODO: node_compare() VARIABLE DECLARATION\n");
		break;
	case NODE_TYPE_VARIABLE_DECLARATION_INITIALIZED:
		printf("TODO: node_compare() VARIABLE DECLARATION INITALIZED\n");
		break;
	case NODE_TYPE_PROGRAM:
		// TODO: Compare two programs.
		printf("TODO: Compare two programs.\n");
		break;
	}
	return 0;
}

Node* node_integer(long long value) {
	Node* integer = node_allocate();
	integer->type = NODE_TYPE_INTEGER;
	integer->value.integer = value;
	return integer;
}

Node* node_symbol(char* symbol_string) {
	Node* symbol = node_allocate();
	symbol->type = NODE_TYPE_SYMBOL;
	symbol->value.symbol = strdup(symbol_string);
	return symbol;
}

Node* node_symbol_from_buffer(char* buffer, size_t length) {
	assert(buffer && "Could not create AST symbol node from NULL buffer");
	char* symbol_string = malloc(length + 1);
	assert(symbol_string && "Could not allocate memory for symbol string");
	memcpy(symbol_string, buffer, length);
	symbol_string[length] = '\0';
	Node* symbol = node_allocate();
	symbol->type = NODE_TYPE_SYMBOL;
	symbol->value.symbol = symbol_string;
	return symbol;
}

void print_node(Node* node, size_t indent_level) {
	if (!node) { return; }

	// Print indent.
	for (size_t i = 0; i < indent_level; ++i) {
		putchar(' ');
	}
	// Print type + value.
	assert(NODE_TYPE_MAX == 8 && "print_node() must handle all node types");
	switch (node->type) {
	default:
		printf("UNKNOWN");
		break;
	case NODE_TYPE_NONE:
		printf("NONE");
		break;
	case NODE_TYPE_INTEGER:
		printf("INT:%lld", node->value.integer);
		break;
	case NODE_TYPE_SYMBOL:
		printf("SYM:");
		if (node->value.symbol) {
			printf("%s", node->value.symbol);
		}
		break;
	case NODE_TYPE_BINARY_OPERATOR:
		printf("TODO: print_node() BINARY_OPERATOR");
		break;
	case NODE_TYPE_VARIABLE_REASSIGNMENT:
		printf("VARIABLE REASSIGNMENT");
		break;
	case NODE_TYPE_VARIABLE_DECLARATION:
		printf("VARIABLE DECLARATION");
		break;
	case NODE_TYPE_VARIABLE_DECLARATION_INITIALIZED:
		printf("TODO: print_node() VAR DECL INIT");
		break;
	case NODE_TYPE_PROGRAM:
		printf("PROGRAM");
		break;
	}
	putchar('\n');
	// Print children.
	Node* child = node->children;
	while (child) {
		print_node(child, indent_level + 4);
		child = child->next_child;
	}
}

void node_free(Node* root) {
	if (!root) { return; }

	Node* child = root->children;
	Node* next_child = NULL;
	while (child) {
		next_child = child->next_child;
		node_free(child);
		child = next_child;
	}
	if (symbolp(*root) && root->value.symbol) {
		free(root->value.symbol);
	}
	free(root);
}

void node_copy(Node* a, Node* b) {
	if (!a || !b) { return; }
	b->type = a->type;
	// Handle all allocated values here.
	switch (a->type) {
	default:
		b->value = a->value;
		break;
	case NODE_TYPE_SYMBOL:
		b->value.symbol = strdup(a->value.symbol);
		assert(b->value.symbol && "node_copy(): Could not allocate memory for new symbol");
		break;
	}
	Node* child = a->children;
	Node* child_it = NULL;
	while (child) {
		Node* new_child = node_allocate();

		if (child_it) {
			child_it->next_child = new_child;
			child_it = child_it->next_child;
		} else {
			b->children = new_child;
			child_it = new_child;
		}

		node_copy(child, child_it);

		child = child->next_child;
	}
}

ParsingContext* parse_context_create() {
	ParsingContext* ctx = calloc(1, sizeof(ParsingContext));
	assert(ctx && "Could not allocate memory for parsing context.");
	ctx->types = environment_create(NULL);
	if (environment_set(ctx->types, node_symbol("integer"), node_integer(0)) == 0) {
		printf("ERROR: Failed to set builtin type in types environment.\n");
	}
	ctx->variables = environment_create(NULL);
	return ctx;
}

/**   PROGRAM
*     [0 -> PROG_MAX]
*      VARIABLE DECLARATION
*      [0 -> 17]
*      `-- SYMBOL ("a") ->
*         [0 -> 1]
*/

/// Update token, token length, and end of current token pointer.
Error lex_advance(Token* token, size_t* token_length, char** end) {
	if (!token || !token_length || !token->end) {
		ERROR_CREATE(err, ERROR_ARGUMENTS, "lex_advance(): pointer arguments must not be NULL!");
		return err;
	}
	Error err = lex(token->end, token);
	*end = token->end;
	if (err.type != ERROR_NONE) { return err; }
	*token_length = token->end - token->beginning;
	return ok;
}

int parse_integer(Token* token, Node* node) {
	if (!token || !node) { return 0; }
	char* end = NULL;
	if (token->end - token->beginning == 1 && *(token->beginning) == '0') {
		node->type = NODE_TYPE_INTEGER;
		node->value.integer = 0;
	} else if ((node->value.integer = strtoll(token->beginning, &end, 10)) != 0) {
		if (end != token->end) { return 0; }
		node->type = NODE_TYPE_INTEGER;
	} else { return 0; }
	return 1;
}

Error parse_expr(ParsingContext* context, char* source, char** end, Node* result) {
	size_t token_count = 0;
	size_t token_length = 0;
	Token current_token;
	current_token.beginning = source;
	current_token.end = source;
	Error err = ok;

	while ((err = lex_advance(&current_token, &token_length, end)).type == ERROR_NONE) {
		//printf("lexed: "); print_token(current_token); putchar('\n');
		if (token_length == 0) { return ok; }

		if (parse_integer(&current_token, result)) {

			// TODO: Look ahead for binary operators that include integers.
			// It would be cool to use an operator environment to look up
			// operators instead of hard-coding them. This would eventually
			// allow for user-defined operators, or stuff like that!

			return ok;
		}

		// TODO: Parse strings and other literal types.

		// TODO: Check for unary prefix operators.

		// TODO: Check that it isn't a binary operator (we should encounter left
		// side first and peek forward, rather than encounter it at top level).

		Node* symbol = node_symbol_from_buffer(current_token.beginning, token_length);

		// TODO: Check if valid symbol for variable environment, 
		// then attempt to pattern match variable access, assignment, 
		// declaration, or declaration with initialization.

		// TODO: Compact the next four lines into `expect()` helper.
		err = lex_advance(&current_token, &token_length, end);
		if (err.type != ERROR_NONE) { return err; }
		if (token_length == 0) { return ok; }
		if (token_string_equalp(":", &current_token)) {

			err = lex_advance(&current_token, &token_length, end);
			if (err.type != ERROR_NONE) { return err; }
			if (token_length == 0) { break; }

			Node* variable_binding = node_allocate();
			if (environment_get(*context->variables, symbol, variable_binding)) {

				// Re-assignment of existing variable (look for =)
				if (token_string_equalp("=", &current_token)) {
					// TODO: Stack based continuation to parse assignment expression.

					// FIXME: This recursive call is kind of the worse :^)
					Node* reassign_expr = node_allocate();
					err = parse_expr(context, current_token.end, end, reassign_expr);
					if (err.type != ERROR_NONE) {
						free(variable_binding);
						return err;
					}

					// TODO: FIXME: Proper type-checking (this only accepts literals)
					// We will have to figure out the return value of the expression.
					if (reassign_expr->type != variable_binding->type) {
						free(variable_binding);
						ERROR_PREP(err, ERROR_TYPE, "Variable assignment expression has mismatched type.");
						return err;
					}

					Node* var_reassign = node_allocate();
					var_reassign->type = NODE_TYPE_VARIABLE_REASSIGNMENT;

					node_add_child(var_reassign, reassign_expr);
					node_add_child(var_reassign, symbol);

					*result = *var_reassign;
					free(var_reassign);

					return ok;

				}
				// TODO: Check that type is actually valid before redefinition error.
				// TODO: Create new error type.
				printf("ID of redefined variable: \"%s\"\n", symbol->value.symbol);
				ERROR_PREP(err, ERROR_GENERIC, "Redefinition of variable!");
				return err;
			}
			free(variable_binding);

			Node* expected_type_symbol = node_symbol_from_buffer(current_token.beginning, token_length);
			if (environment_get(*context->types, expected_type_symbol, result) == 0) {
				ERROR_PREP(err, ERROR_TYPE, "Invalid type within variable declaration");
				printf("\nINVALID TYPE: \"%s\"\n", expected_type_symbol->value.symbol);
				return err;
			}

			Node* var_decl = node_allocate();
			var_decl->type = NODE_TYPE_VARIABLE_DECLARATION;

			Node* type_node = node_allocate();
			type_node->type = result->type;

			// `symbol` is now owned by var_decl.
			node_add_child(var_decl, symbol);

			// FIXME: Use `expect()` helper once it exists.
			char* old_end = *end;
			err = lex_advance(&current_token, &token_length, end);
			if (err.type != ERROR_NONE) { return err; }
			if (token_length == 0) { break; }
			if (token_string_equalp("=", &current_token)) {
				// TODO: Stack based continuation to parse assignment expression.

				// FIXME: This recursive call is kind of the worse :^)
				Node* assigned_expr = node_allocate();
				err = parse_expr(context, current_token.end, end, assigned_expr);
				if (err.type != ERROR_NONE) { return err; }

				// TODO: FIXME: Proper type-checking (this only accepts literals)
				// We will have to figure out the return value of the expression.
				if (assigned_expr->type != type_node->type) {
					node_free(assigned_expr);
					ERROR_PREP(err, ERROR_TYPE, "Variable assignment expression has mismatched type.");
					return err;
				}

				type_node->value = assigned_expr->value;
				// Node contents transfer ownership, assigned_expr is now hollow shell.
				free(assigned_expr);
			} else {
				*end = old_end;
			}

			/* VARIABLE DECLARATION
			 * `-- SYMBOL (ID)
			 * Add to parsing context variable enviornment
			 *
			 * ENVIRONMENT
			 * `-- variables
			 *	   `-- binding
			 *		   `-- SYMBOL (ID) -> TYPE (VALUE)
			 *
			 * During codegen:
			 * |-- Find somewhere to stick the length of bytes of the size of TYPE.
			 * `-- Keep track of where we stick it :^)
			 *
			 * We never actually need the symbol in the AST, I don't think.
			 * We just need to keep track of it in parsing context so that
			 * future accesses and re-assignments can refer to the same one.
			 *
			 * VARIABLE RE-ASSIGNMENT
			 * `-- NEW VALUE EXPRESSION -> SYMBOL (ID)
			 *
			 * If we have a codegen context, then we can map symbols to
			 * wherever we decide to stick them. Then we can just do a
			 * environment lookup in the codegen context to update the
			 * proper value.
			 *
			 * A codegen context must contain, for example in x86_64 ASM,
			 * stack offsets of local variable declarations. Otherwise,
			 * how would we ever access them after creation, right?
			 *
			 * PROGRAM -> "a : integer  a := 420"
			 *	 VARIABLE DECLARATION
			 *	 `-- SYMBOL ("a")
			 *	 VARIABLE REASSIGNMENT
			 *	 `-- INTEGER (420) -> SYMBOL ("a")
			 *
			 * Codegen for each top-level VAR DECL (inherited from parsing context variables environment):
			 *   Look up symbol in variable environment <- should never fail
			 *	 Get size of type in bytes	               if parsing works.
			 *   Generate variable space in .space or something.
			 *   Re-define binding in globals environment to store new symbol.
			 *
			 * ASM:
			 * .data
			 *	 ga: .space <SIZEOF INTEGER>, 0
			 * .code
			 *   movq ga, %rax
			 *   movq %rbx, (%rax)
			*/

			// AST gains variable declaration node.
			*result = *var_decl;

			// Node contents transfer ownership, var_decl is now hollow shell.
			free(var_decl);

			// Context variables environment gains new binding.
			Node* symbol_for_env = node_allocate();
			node_copy(symbol, symbol_for_env);
			int status = environment_set(context->variables, symbol_for_env, type_node);
			if (status != 1) {
				ERROR_PREP(err, ERROR_GENERIC, "Failed to define variable!");
				return err;
			}

			return ok;
		}

		printf("Unrecognized token: ");
		print_token(current_token);
		putchar('\n');

		ERROR_PREP(err, ERROR_SYNTAX, "Unrecognized token reached during parsing");
		return err;
	}

	return err;
}