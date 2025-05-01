/*
 * Copyright (c) 2025 NorthernL1ghts
 */

/*
 * SHA HASH: 62489826a30f3412b299bfe029709e2e04c224b0d2114f2f093fcd7eb525cade
 * Filename: src/file_io.h
 * Updated: 2025-05-01, 17:36:02
*/
/*
 * Copyright (c) 2025 NorthernL1ghts
 */

 /*
  * SHA HASH: db3db72a01de98d833c44b891587fa9e3bf131cb4a6a16a42b6fd9a5e1e09d93
  * Filename: src/file_io.h
  * Updated: 2025-05-01, 14:06:24
 */
#ifndef COMPILER_FILE_IO_H
#define COMPILER_FILE_IO_H

#include <stddef.h>
#include <stdio.h>

size_t file_size(FILE* file);
char* file_contents(char* path);

#endif /* COMPILER_FILE_IO_H */