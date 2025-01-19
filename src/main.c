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
	// NOTE (NL) : We should probably handle this a better way when dealing 
	// with large files, but that doesn't really matter right now.
	size_t bytes_read = 0;
	while (bytes_read < size) {
		size_t bytes_read_this_iteration = fread(write_it, 1, size - bytes_read, file); // Read the rest of the file.
		if (ferror(file)) {
			printf("Error when reading: %i\n", errno);
			free(contents);
			return NULL;
		}
		bytes_read += bytes_read_this_iteration;
		write_it += bytes_read_this_iteration;
		if (feof(file)) { break; }
	}
	contents[bytes_read] = '\0'; // NULL terminator.
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
const char* delimiters = " \r\n,():"; // NOTE (NL) : Delimiters just end a token and begin a new one.

/// Lex the next token from SOURCE, and point to it with BEG and END.
Error lex(char* source, char** beg, char** end) {
	Error err = ok;
	if (!source || !beg || !end) {
		ERROR_PREP(err, ERROR_ARGUMENTS, "Can not lex empty source.");
		return err;
	};
	*beg = source;
	*beg += strspn(*beg, whitespace); // Skip the whitespace at the beginning.
	*end = *beg;
	if (**end == '\0') { return err; }
	*end += strcspn(*beg, delimiters); // Skip everything that is not in delimiters.
	if (*end == *beg) {
		*end += 1; // 1 byte.
	}
	return err;
}

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

	struct Node** children;

} Node;

// Predicates (booleans)
#define nonep(node) ((node).type == NODE_TYPE_NONE)
#define integerp(node) ((node).type == NODE_TYPE_INTEGER)

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

void environment_set() {

}

Error parse_expr(char* source, Node* result) {
	char* beg = source;
	char* end = source;
	Error err = ok;
	while ((err = lex(end, &beg, &end)).type == ERROR_NONE) {
		if (end - beg == 0) { break; }
		printf("lexed: %.*s\n", end - beg, beg);
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