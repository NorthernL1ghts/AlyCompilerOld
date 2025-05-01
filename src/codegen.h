/*
 * Copyright (c) 2025 NorthernL1ghts
 */

/*
 * SHA HASH: a9adc6c8ff760d4831767a8fd86cd53a6c942140bef2006573a2779d544ea327
 * Filename: src/codegen.h
 * Updated: 2025-05-01, 13:13:40
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