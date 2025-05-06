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
 * NorthernL1ghts
 */

#ifndef COMPILER_FILE_IO_H
#define COMPILER_FILE_IO_H

#include <stddef.h>
#include <stdio.h>

size_t file_size(FILE* file);
char* file_contents(char* path);

#endif /* COMPILER_FILE_IO_H */