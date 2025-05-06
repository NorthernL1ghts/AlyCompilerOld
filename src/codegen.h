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

#ifndef COMPILER_CODEGEN_H
#define COMPILER_CODEGEN_H

#include <error.h>
#include <environment.h>
#include <parser.h>

typedef int RegisterDescriptor;

typedef struct Register {
	struct Register* next;
	/// What will be emitted when referencing this register, i.e "%rax"
	char* name;
	/// If non-zero, this register is in use.
	char in_use;
} Register;

/// NAME is now owned by register.
Register* register_create(char* name);

/// NAME is now owned by register.
void register_add(Register* base, char* name);

void register_free(Register* base);

RegisterDescriptor register_allocate(Register* base);
void register_deallocate(Register* base, RegisterDescriptor register_descriptor);

char* register_name(Register* base, RegisterDescriptor register_descriptor);

char* label_generate();

typedef struct CodegenContext {
	struct CodegenContext* parent;
	/// LOCALS
	/// `-- SYMBOL (NAME) -> INTEGER (STACK OFFSET)
	Environment* locals;
} CodegenContext;

enum CodegenOutputFormat {
	CG_FMT_DEFAULT = 0,
	CG_FMT_x86_64_MSWIN,
};

Error codegen_program(enum CodegenOutputFormat format, ParsingContext* context, Node* program);

#endif COMPILER_CODEGEN_H /* COMPILER_CODEGEN_H */