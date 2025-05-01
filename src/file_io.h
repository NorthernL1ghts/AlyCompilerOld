/*
 * Copyright (c) 2025 NorthernL1ghts
 */

/*
 * SHA HASH: 0663eff0b3c707d6ae08049060b194f947fe9ecf05892dc462368bd603e96fd3
 * Filename: src/file_io.h
 * Updated: 2025-05-01, 13:13:40
*/
#ifndef COMPILER_FILE_IO_H
#define COMPILER_FILE_IO_H

#include <stdio.h>

long file_size(FILE* file);
char* file_contents(char* path);

#endif /* COMPILER_FILE_IO_H */