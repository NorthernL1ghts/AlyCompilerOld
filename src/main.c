#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

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
	} type;
	char* msg;
} Error;

Error ok = { ERROR_NONE, NULL };

void print_error(Error err) {
	if (err.type == ERROR_NONE) { return; }
	printf("ERROR: ");
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

Error lex(char* source, char** beg, char** end) {
	Error err = ok;
	if (!source) {
		ERROR_PREP(err, ERROR_ARGUMENTS, "Can not lex empty source.");
	};
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
		printf("Contents of %s:\n---\n\"%s\"\n---\n", path, contents);
		free(contents);
	}

	Error err = lex(NULL, NULL, NULL);
	print_error(err);

	return 0;
}