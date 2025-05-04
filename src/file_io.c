/*
 * Copyright (c) 2025 NorthernL1ghts. All rights reserved.
 *
 * This file is part of the AlyCompiler.
 * Unauthorized copying, modification, or distribution of this file,
 * via any medium, is strictly prohibited except as permitted under
 * the terms outlined in the LICENSE file.
 *
 * For licensing details, contributions, or inquiries, refer to LICENSE
 * or contact:
 * NorthernL1ghts Software Division
 */

#include <file_io.h>
#include <errno.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

size_t file_size(FILE* file) {
    if (!file) { return 0; }
    // Cast required for cross-compatibility.
    fpos_t original;
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
    size_t size = file_size(file);
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
