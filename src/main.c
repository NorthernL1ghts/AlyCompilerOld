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
	fseek(file, 0, SEEK_SET);
	long out = ftell(file);
	if (fsetpos(file, &original) != 0) {
		printf("fsetpos() failed: %i\n", errno);
	}
	return out;
}

char* file_contents(char* path) {
	FILE* file = fopen(path, "r");
	if (!file) {
		printf("Could not open file at %s", path);
		return NULL;
	}
	// Otherwise, if you find file get the size.
	long size = file_size(file);
}

void print_usage(char** argv) {
	printf("USAGE: %s <path_to_file_to_compile>\n", argv[0]);
}

int main(int argc, char** argv) {
	if (argc < 2) {
		print_usage(argv);
		exit(0);
	}

	return 0;
}