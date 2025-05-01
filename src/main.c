/*
 * Copyright (c) 2025 NorthernL1ghts
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
        exit(0);
    }

    char* contents = file_contents(argv[1]);
    if (contents) {
        Error err = ok;
        ParsingContext* context = parse_context_default_create();
        Node* program = node_allocate();
        program->type = NODE_TYPE_PROGRAM;
        Node* expression = node_allocate();
        char* contents_it = contents;
        for (;;) {
            Node* expression = node_allocate();
            node_add_child(program, expression);
            err = parse_expr(context, contents_it, &contents_it, expression);
            if (err.type != ERROR_NONE) {
                print_error(err);
                break;
            }
            // Check for end-of-parsing case (source and end are the same).
            if (!(*contents_it)) { break; }

            //printf("Parsed expression:\n");
            //print_node(expression, 0);
            //putchar('\n');
        }

        print_node(program, 0);
        putchar('\n');

        if (err.type == ERROR_NONE) {
            printf("Generating code!\n");

            err = codegen_program(OUTPUT_FMT_DEFAULT, context, program);
            print_error(err);

            printf("Code generated.\n");
        }

        node_free(program);
        free(contents);
    }

    printf("COMPLETE\n");

    return 0;
}