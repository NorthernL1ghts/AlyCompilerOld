/*
 * Copyright (c) 2025 NorthernL1ghts
 */

/*
 * SHA HASH: 04fd962bad7d49dc3efb6afbe5246ce2a6c05715e008b95d08176c07accabe9e
 * Filename: src/codegen.h
 * Updated: 2025-05-01, 14:06:24
*/
#ifndef COMPILER_CODEGEN_H
#define COMPILER_CODEGEN_H

#include <error.h>
#include <parser.h>

typedef enum CodegenOutputFormat {
	OUTPUT_FMT_DEFAULT = 0,
	OUTPUT_FMT_x86_64_AT_T_ASM,
} CodegenOutputFormat;

Error codegen_program(CodegenOutputFormat, ParsingContext* context, Node* program);

#endif /* COMPILER_CODEGEN_H */