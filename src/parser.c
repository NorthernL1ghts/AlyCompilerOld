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
	printf("%.*s", t.end - t.beginning, t.beginning);
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
	}
	else {
		parent->children = new_child;
	}
}

int node_compare(Node* a, Node* b) {
	if (!a || !b) {
		if (!a && !b) {
			return 1;
		}
		return 0;
	}
	// TODO: This assert doesn't work, I don't know why :^(.
	assert(NODE_TYPE_MAX == 7 && "node_compare() must handle all node types");
	if (a->type != b->type) { return 0; }
	switch (a->type) {
	case NODE_TYPE_NONE:
		if (nonep(*b)) { return 1; }
		return 0;
		break;
	case NODE_TYPE_INTEGER:
		if (a->value.integer == b->value.integer) {
			return 1;
		}
		return 0;
		break;
	case NODE_TYPE_SYMBOL:
		if (a->value.symbol && b->value.symbol) {
			if (strcmp(a->value.symbol, b->value.symbol) == 0) {
				return 1;
			}
			return 0;
		}
		else if (!a->value.symbol && !b->value.symbol) {
			return 1;
		}
		return 0;
		break;
	case NODE_TYPE_BINARY_OPERATOR:
		printf("TODO: node_compare() BINARY OPERATOR\n");
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
	integer->children = NULL;
	integer->next_child = NULL;
	return integer;
}

Node* node_symbol(char* symbol_string) {
	// NOTE: 'strdup' is deprecated on Window's MSVC for safety, in Clang still
	// exists.
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
	assert(NODE_TYPE_MAX == 7 && "print_node() must handle all node types");
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
	case NODE_TYPE_VARIABLE_DECLARATION:
		printf("VARIABLE DECLARATION");
		// printf("VAR_DECL:");
		// TODO: Print first child (ID symbol), then type of second child.
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

int parse_integer(Token* token, Node* node) {
	if (!token || !node) { return 0; }

	char* end = NULL;
	if (token->end - token->beginning == 1 && *(token->beginning) == '0') {
		node->type = NODE_TYPE_INTEGER;
		node->value.integer = 0;
	}
	else if ((node->value.integer = strtoll(token->beginning, &end, 10)) != 0) {
		if (end != token->end) {
			return 0;
		}
		node->type = NODE_TYPE_INTEGER;
	}
	else { return 0; }
	return 1;
}

Error parse_expr(ParsingContext* context, char* source, char** end, Node* result) {
	size_t token_count = 0;
	Token current_token;
	current_token.beginning = source;
	current_token.end = source;
	Error err = ok;

	while ((err = lex(current_token.end, &current_token)).type == ERROR_NONE) {
		*end = current_token.end;
		size_t token_length = current_token.end - current_token.beginning;
		if (token_length == 0) { break; }
		if (parse_integer(&current_token, result)) {
			// Look ahead for binary operators that include integers.
			Node lhs_integer = *result;
			err = lex(current_token.end, &current_token);
			*end = current_token.end;
			if (err.type != ERROR_NONE) {
				return err;
			}
			*end = current_token.end;

			// TODO: Check for valid integer operator.
			// It would be cool to use an operator environment to look up
			// operators instead of hard-coding them. This would eventually
			// allow for user-defined operators, or stuff like that!
		}
		else {
			// TODO: Check for unary prefix operators.

			// TODO: Check that it isn't a binary operator (we should encounter left
			// side first and peek forward, rather than encounter it at top level).

			Node* symbol = node_symbol_from_buffer(current_token.beginning, token_length);

			// *result = *symbol;

			// TODO: Check if valid symbol for environment, then attempt to
			// pattern match variable access, assignment, declaration, or
			// declaration with initialization.

			err = lex(current_token.end, &current_token);
			*end = current_token.end;

			if (err.type != ERROR_NONE) {
				return err;
			}
			*end = current_token.end;
			size_t token_length = current_token.end - current_token.beginning;
			if (token_length == 0) { break; }

			if (token_string_equalp(":", &current_token)) {

				err = lex(current_token.end, &current_token);
				*end = current_token.end;
				if (err.type != ERROR_NONE) { return err; }
				size_t token_length = current_token.end - current_token.beginning;
				if (token_length == 0) { break; }

				// TODO: Check for variable re-assignment...

				// FIXME: Actually set variable declarations within environment so that
				// reassigments and redefinitions can be properly parsed and handled.
				Node* variable_binding = node_allocate();
				if (environment_get(*context->variables, symbol, variable_binding)) {
					// Either re-assignment of existing variable (look for =)

					if (token_string_equalp("=", &current_token)) {
						err = lex(current_token.end, &current_token);
						*end = current_token.end;
						if (err.type != ERROR_NONE) { return err; }
						size_t token_length = current_token.end - current_token.beginning;
						if (token_length == 0) { break; }

						// TODO: Stack based continuation to parse assignment expression.

						// FIXME: This recursive call is kind of the worse :^)
						Node* assigned_expr = node_allocate();
						err = parse_expr(context, current_token.end, &current_token.end, assigned_expr);
						if (err.type != ERROR_NONE) { return err; }

						// TODO: FIXME: Proper type-checking (this only accepts literals)
						// We will have to figure out the return value of the expression.
						if (assigned_expr->type != variable_binding->children->type) {
							ERROR_PREP(err, ERROR_TYPE, "Variable assignment expression has mismatched type.");
							return err;
						}

						variable_binding->children->value = assigned_expr->value;

						// Node contents transfer ownership, assigned_expr is now hollow shell.
						free(assigned_expr);
						return ok;
					}
					// TODO: Create new error type.
					printf("ID of redefined variable: \"%s\"\n", symbol->value.symbol);
					ERROR_PREP(err, ERROR_GENERIC, "Redefinition of variable!");
					return err;
				}

				Node* expected_type_symbol = node_symbol_from_buffer(current_token.beginning, token_length);
				if (environment_get(*context->types, expected_type_symbol, result) == 0) {
					ERROR_PREP(err, ERROR_TYPE, "Invalid type within variable declaration");
					printf("\nINVALID TYPE: \"%s\"\n", expected_type_symbol->value.symbol);
					return err;
				}
				else {
					// TODO: Check for variable re-definition...

					// printf("Found valid type: ");
					// print_node(expected_type_symbol, 0);
					// putchar('\n');

					Node* var_decl = node_allocate();
					var_decl->type = NODE_TYPE_VARIABLE_DECLARATION;

					Node* type_node = node_allocate();
					type_node->type = result->type;

					// NOTE: Because `var_decl` is used in verbatim that means 
					// that we can access the memory of the child. 
					node_add_child(var_decl, type_node);
					node_add_child(var_decl, symbol);

					// TODO: Check for "=" initialization operator after var decl.
					err = lex(current_token.end, &current_token);
					*end = current_token.end;
					if (err.type != ERROR_NONE) { return err; }
					size_t token_length = current_token.end - current_token.beginning;
					if (token_length == 0) { break; }

					if (token_string_equalp("=", &current_token)) {
						err = lex(current_token.end, &current_token);
						*end = current_token.end;
						if (err.type != ERROR_NONE) { return err; }
						size_t token_length = current_token.end - current_token.beginning;
						if (token_length == 0) { break; }

						// TODO: Stack based continuation to parse assignment expression.

						// FIXME: This recursive call is kind of the worse :^)
						Node* assigned_expr = node_allocate();
						err = parse_expr(context, current_token.end, &current_token.end, assigned_expr);
						if (err.type != ERROR_NONE) { return err; }

						// TODO: FIXME: Proper type-checking (this only accepts literals)
						// We will have to figure out the return value of the expression.
						if (assigned_expr->type != type_node->type) {
							ERROR_PREP(err, ERROR_TYPE, "Variable assignment expression has mismatched type.");
							return err;
						}

						type_node->value = assigned_expr->value;

						// Node contents transfer ownership, assigned_expr is now hollow shell.
						free(assigned_expr);
					}

					*result = *var_decl;
					// TODO: Write node_copy() and node_deep_copy(), then deep copy
					// var_decl, into environment. Possibly copy symnbol as well?
					// int status = environment_set(context->variables, symbol, result);

					// Node contents transfer ownership, var_decl is now hollow shell.
					free(var_decl);

					return ok;
				}
			}

			printf("Unrecognized token: ");
			print_token(current_token);
			putchar('\n');

			ERROR_PREP(err, ERROR_SYNTAX, "Unrecognized token reached during parsing");
			return err;

		}
		printf("Intermediate node: ");
		print_node(result, 0);
		putchar('\n');
	}

	return err;
}
