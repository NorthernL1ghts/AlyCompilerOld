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

#ifndef ALY_COMPILER_TYPE_CHECKER_H
#define ALY_COMPILER_TYPE_CHECKER_H

#include <error.h>
#include <parser.h>

Error expression_return_type(ParsingContext* context, Node* expression, int* type);

Error typecheck_expression(ParsingContext* context, Node* expression);

Error typecheck_program(ParsingContext* context, Node* program);

#endif /* ALY_COMPILER_TYPE_CHECKER_H */