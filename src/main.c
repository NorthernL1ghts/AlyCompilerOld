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

#include <assert.h>
#include <codegen.h>
#include <environment.h>
#include <file_io.h>
#include <error.h>
#include <parser.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_usage(char** argv) {
    printf("USAGE: %s <path_to_file_to_compile>\n", argv[0]);
}

int main(int argc, char** argv) {
    if (argc < 2) {
        print_usage(argv);
        return 0;
    }

    Node* program = node_allocate();
    ParsingContext* context = parse_context_default_create();
    Error err = parse_program(argv[1], context, program);

    print_node(program, 0);
    putchar('\n');

    if (err.type) {
        print_error(err);
        return 1; // Upon encountering Parsing errors
    }

    err = codegen_program(OUTPUT_FMT_DEFAULT, context, program);
    if (err.type) {
        print_error(err);
        return 2; // Upon encountering CodeGen errors
    }

    node_free(program);

    return 0;
}