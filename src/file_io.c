/*
 * Copyright (c) 2025 NorthernL1ghts
 */

/*
 * SHA HASH: 74be94ac584e04951570f6730145288a6598ddb834bd6ebbb904ecaf4a691f76
 * Filename: src/file_io.c
 * Updated: 2025-05-01, 13:13:40
*/
#include <file_io.h>
#include <errno.h>

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
	fseek(file, 0, SEEK_SET);
	long size = file_size(file);
	char* contents = malloc(size + 1);
	assert(contents && "Could not allocate buffer for file contents");
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
