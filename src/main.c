#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long file_size(FILE* file) {
	if (!file) { return 0; }
	fpos_t original = 0;
	if (fgetpos(file, &original) != 0) {
		printf("fgetpos() failed: %i\n", errno);
		return 0;
	}
	fseek(file, 0, SEEK_END);
	long out = ftell(file);
	if (fsetpos(file, &original) != 0) {
		printf("fsetpos() failed: %i\n", errno);
	}
	return out;
}

char* file_contents(char* path) {
	FILE* file = fopen(path, "r");
	if (!file) {
		printf("Could not open file at %s\n", path);
		return NULL;
	}
	// Otherwise, if you find file get the size.
	fseek(file, 0, SEEK_SET);
	long size = file_size(file);
	char* contents = malloc(size + 1);
	char* write_it = contents;
	size_t bytes_read = 0;
	while (bytes_read < size) {
		size_t bytes_read_this_iteration = fread(write_it, 1, size - bytes_read, file);
		if (ferror(file)) {
			printf("Error when reading: %i\n", errno);
			free(contents);
			return NULL;
		}
		bytes_read += bytes_read_this_iteration;
		write_it += bytes_read_this_iteration;
		if (feof(file)) { break; }
	}
	contents[bytes_read] = '\0';
	return contents;
}

void print_usage(char** argv) {
	printf("USAGE: %s <path_to_file_to_compile>\n", argv[0]);
}

typedef struct Error {
	enum ErrorType {
		ERROR_NONE = 0,
		ERROR_ARGUMENTS,
		ERROR_TYPE,
		ERROR_GENERIC,
		ERROR_SYNTAX,
		ERROR_TODO,
		ERROR_MAX,
	} type;
	char* msg;
} Error;

Error ok = { ERROR_NONE, NULL };

void print_error(Error err) {
	if (err.type == ERROR_NONE) { return; }
	printf("ERROR: ");
	assert(ERROR_MAX == 6);
	switch (err.type) {
	default:
		printf("Unknown error type...");
		break;
	case ERROR_TODO:
		printf("TODO (not implemented)");
		break;
	case ERROR_SYNTAX:
		printf("Invalid Syntax");
		break;
	case ERROR_TYPE:
		printf("Mismatched types");
		break;
	case ERROR_ARGUMENTS:
		printf("Invalid arguments");
		break;
	case ERROR_NONE:
		break;
	case ERROR_GENERIC:
		break;
	}
	putchar('\n');
	if (err.msg) {
		printf("     : %s\n", err.msg);
	}
}
#define ERROR_CREATE(n, t, msg)   \
	Error (n) = { (t), (msg) } 

#define ERROR_PREP(n, t, message)   \
	(n).type = (t);                  \
	(n).msg = (message);

const char* whitespace = " \r\n";
const char* delimiters = " \r\n,():"; // Delimiters just end a token and begin a new one.

typedef struct Token {
	char* beginning;
	char* end;
	struct Token* next;
} Token;

Token* token_create() {
	Token* token = malloc(sizeof(Token));
	assert(token && "Could not allocate memory for token");
	memset(token, 0, sizeof(Token));
	return token;
}

void token_free(Token* root) {
	while (root) {
		Token* token_to_free = root;
		root = root->next;
		free(token_to_free);
	}
}

void print_tokens(Token* root) {
	// NOTE: Sequential to prevent stackoverflow issue
	size_t count = 1;
	while (root) {
		if (count > 10000) { break; } // FIXME: Remove this limit.
		printf("Token %zu: ", count);
		if (root->beginning && root->end) {
			printf("%.*s", root->end - root->beginning, root->beginning);
		}
		putchar('\n');
		root = root->next;
		count++;
	}
}

/// Lex the next token from SOURCE, and point to it with BEG and END.
Error lex(char* source, Token* token) {
	Error err = ok;
	if (!source || !token) {
		ERROR_PREP(err, ERROR_ARGUMENTS, "Can not lex empty source.");
		return err;
	};
	token->beginning = source;
	token->beginning += strspn(token->beginning, whitespace); // Skip the whitespace at the beginning.
	token->end = token->beginning;
	if (*(token->end) == '\0') { return err; }
	token->end += strcspn(token->beginning, delimiters); // Skip everything that is not in delimiters.
	if (token->end == token->beginning) {
		token->end += 1; // Just 1 byte.
	}
	return err;
}

//			 Node-
//			/  |  \
//		    0  1  2
//		   / \
//        3   4
// 
// Node
// |-- 0  ->  1  ->  2
//	   `-- 3  -> 4

// TODO:
// |-- API to create new node.
// `-- API to add node as child.
typedef long long integer_t;
typedef struct Node {
	enum NodeType {
		NODE_TYPE_NONE,
		NODE_TYPE_MAX,
		NODE_TYPE_PROGRAM,
		NODE_TYPE_INTEGER,
	} type;
	union NodeValue {
		integer_t integer;
	} value;
	// Possible TODO: Parent?
	struct Node* children;
	struct Node* next_child;
} Node;

// Predicates
#define nonep(node) ((node).type == NODE_TYPE_NONE)
#define integerp(node) ((node).type == NODE_TYPE_INTEGER)

// TODO: Make more efficient! -- Maybe keep track of allocated ptr's
// then free them all in one go?
void node_free(Node* root) {
	if (!root) { return; }
	Node* child = root->children;
	Node* next_child = NULL;
	while (child) {
		next_child = child->next_child;
		node_free(child);
		child = next_child;
	}
	free(root);
}

// TODO:
// |-- API to create new Binding.
// `-- API to add Binding to environment.
typedef struct Binding {
	char* id;
	Node* value;
	struct Binding* next;
} Binding;

// TODO: API to create new Environment.
typedef struct Environment {
	struct Environment* parent;
	Binding* bind;
} Environment;

void environment_set() {}

// @return Boolean-like value; 1 for success, 0 for failure.
int token_string_equalp(char* string, Token* token) {
	if (!string || !token) { return 0; }
	char* beg = token->beginning;
	while (*string && token->beginning < token->end) {
		if (*string != *beg) {
			return 0;
		}
		string++;
		beg++;
	}
	return 1;
}

Error parse_expr(char* source, Node* result) {
	size_t token_count = 0;
	Token current_token;
	current_token.next = NULL;
	current_token.beginning = source;
	current_token.end = source;
	Error err = ok;

	Node* root = calloc(1, sizeof(Node));
	assert(root && "Could notg allocate memory for AST Root.");
	root->type = NODE_TYPE_PROGRAM;

	Node working_node;
	working_node.children = NULL;
	working_node.next_child = NULL;
	working_node.type = NODE_TYPE_NONE;
	working_node.value.integer = 0;
	while ((err = lex(current_token.end, &current_token)).type == ERROR_NONE) {
		size_t token_length = current_token.end - current_token.beginning;
		if (token_length == 0) { break; }
		if (token_length == 1 && *(current_token.beginning) == '0') {
			working_node.type = NODE_TYPE_INTEGER;
			working_node.value.integer = 0;
			printf("Zero integer!\n");
		}
		if ((working_node.value.integer = strtoll(current_token.beginning, NULL, 10)) != 0) {
			working_node.type = NODE_TYPE_INTEGER;
			printf("Found integer %lld!\n", working_node.value.integer);
		}
		if (token_string_equalp(":", &current_token)) {
			// Have an assignment, so move forward.
			Token equals;
		}
	}
	return err;
}

int main(int argc, char** argv) {
	if (argc < 2) {
		print_usage(argv);
		exit(0);
	}

	char* path = argv[1];
	char* contents = file_contents(path);
	if (contents) {
		// printf("Contents of %s:\n---\n\"%s\"\n---\n", path, contents);

		Node expression;
		Error err = parse_expr(contents, &expression);
		print_error(err);

		free(contents);
	}

	return 0;
}