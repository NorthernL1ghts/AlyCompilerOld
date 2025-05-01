/*
 * Copyright (c) 2025 NorthernL1ghts
 */

/*
 * SHA HASH: e7c44e77e4057579030b5cb459f4306804a2e487f550f09b59aece803ec970e7
 * Filename: src/codegen.h
 * Updated: 2025-05-01, 17:36:02
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