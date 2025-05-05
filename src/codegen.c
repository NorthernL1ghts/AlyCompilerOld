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

#include <codegen.h>

#include <assert.h>
#include <environment.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <parser.h>

CodegenContext* codegen_context_create(CodegenContext* parent) {
    CodegenContext* cg_ctx = calloc(1, sizeof(CodegenContext));
    cg_ctx->parent = parent;
    cg_ctx->locals = environment_create(NULL);
    return cg_ctx;
}

//============================================================== BEG REGISTER STUFF

Register* register_create(char* name) {
    Register* r = calloc(1, sizeof(Register));
    assert(r && "Could not allocate memory for new register");
    r->name = name;
    return r;
}

void register_add(Register* base, char* name) {
    while (base->next) {
        base = base->next;
    }
    base->next = register_create(name);
}

void register_free(Register* base) {
    Register* to_free = NULL;
    while (base) {
        to_free = base;
        if (base->name) {
            free(base->name);
        }
        base = base->next;
        free(to_free);
    }
}

RegisterDescriptor register_allocate(Register* base) {
    RegisterDescriptor register_descriptor = 0;
    while (base) {
        if (base->in_use == 0) {
            base->in_use = 1;
            return register_descriptor;
        }
        base = base->next;
        register_descriptor++;
    }
    printf("ERROR::register_allocate(): Could nt allocate register!");
    exit(1);
    return -1;
}

void register_deallocate(Register* base, RegisterDescriptor register_descriptor) {
    while (base) {
        if (register_descriptor == 0) {
            base->in_use = 0;
            return;
        }
        base = base->next;
        register_descriptor--;
    }
    printf("ERROR::register_deallocate(): Could not deallocate register %d\n", register_descriptor);
    exit(1);
}

char* register_name(Register* base, RegisterDescriptor register_descriptor) {
    while (base) {
        register_descriptor--;
        if (register_descriptor <= 0) {
            return base->name;
        }
        base = base->next;
    }
    printf("ERROR::register_name(): Could not find register with descriptor of %d\n", register_descriptor);
    return NULL;
}

//============================================================== END REGISTER STUFF

#define label_buffer_size 1024
char label_buffer[label_buffer_size];
size_t label_index;
size_t label_count = 0;
char* label_generate() {
    char* label = label_buffer + label_index;
    label_index += snprintf(label, label_buffer_size - label_index, ".L%d:", label_count);
    label_index++;
    if (label_index >= label_buffer_size) {
        label_index = 0;
        return label_generate();
    }
    label_count++;
    return label;
}

//============================================================== BEG CG_FMT_x86_64_MSWIN

#define symbol_buffer_size 1024
char symbol_buffer[symbol_buffer_size];
size_t symbol_index;
size_t symbol_count = 0;
// NOTE: Aly currently struggles with variable declarations and reassignments.
// In assembly (ASM), we can't manage this the way we currently do.
// To resolve this, we'll need a system that translates symbols into memory addresses.
// However, this doesn't mean we need to change the fundamental design of the language.
char* symbol_to_address(Node* symbol) {
    char* symbol_string = symbol_buffer + symbol_index;
    // Global variable access.
    symbol_index += snprintf(symbol_string, symbol_buffer_size - symbol_index, "%s(%%rip)", symbol->value.symbol);

    // TODO: Local variable access.

    symbol_index++;
    if (symbol_index >= symbol_buffer_size) {
        symbol_index = 0;
        return label_generate();
    }
    return symbol_string;
}

Error codegen_expression_x86_64_mswin(FILE* code, CodegenContext* cg_context, ParsingContext* context, Node* expression) {
    Error err = ok;
    switch (expression->type) {
    default:
        break;
    case NODE_TYPE_VARIABLE_REASSIGNMENT:
        if (cg_context->parent) {
            //ERROR_PREP(err, ERROR_TODO, "codegen_expression_x86_64_mswin(): ""Can not do local variable codegen yet, sorry :p");
            //break;
        }
        else {
            fprintf(code, "%s\n", symbol_to_address(expression->children));
        }
        break;
    }
    return err;
}

Error codegen_function_x86_64_att_asm_mswin(CodegenContext* cg_context, ParsingContext* context, char* name, Node* function, FILE* code) {
    Error err = ok;

    cg_context = codegen_context_create(cg_context);

    // Store base pointer integer within local environment.
    // Start at one to make space for pushed RBP in function header.
    size_t param_count = 1;
    Node* parameter = function->children->children;
    while (parameter) {
        param_count++;
        // Bind parameter name to integer base pointer offset.
        // FIXME: Assume each argument is 8 bytes for now.
        // TODO: This currently doesn't allow for passing arguments in register, which is much faster.
        environment_set(cg_context->locals, parameter->children, node_integer(-param_count * 8));
        parameter = parameter->next_child;
    }

    // Nested function execution protection
    fprintf(code, "jmp after%s\n", name);
    // Function beginning label
    fprintf(code, "%s:\n", name);

    // Function header
    const char* function_header =
        "push %rbp\n"
        "mov %rsp, %rbp\n"
        "sub $32, %rsp\n";
    fprintf(code, "%s", function_header);

    // Function body
    Node* expression = function->children->next_child->next_child->children;
    while (expression) {
        err = codegen_expression_x86_64_mswin(code, cg_context, context, expression);
        if (err.type) {
            print_error(err);
            return err;
        }
        expression = expression->next_child;
    }

    printf("here\n");


    // Function footer
    const char* function_footer =
        "add $32, %rsp\n"
        "pop %rbp\n"
        "ret\n";
    fprintf(code, "%s", function_footer);

    // Nested function execution jump label
    fprintf(code, "after%s:\n", name);

    // TODO: Free context
    cg_context = cg_context->parent;

    return ok;
}

Error codegen_program_x86_64_mswin(FILE* code, CodegenContext* cg_context, ParsingContext* context, Node* program) {
    Error err = ok;

    // Generate global functions
    Binding* function_it = context->functions->bind;
    while (function_it) {
        Node* function_id = function_it->id;
        Node* function = function_it->value;
        function_it = function_it->next;
        err = codegen_function_x86_64_att_asm_mswin(cg_context, context, function_id->value.symbol, function, code);
    }

    Node* expression = program->children;
    while (expression) {
        codegen_expression_x86_64_mswin(code, cg_context, context, expression);
        expression = expression->next_child;
    }

    ERROR_PREP(err, ERROR_TODO, "codegen_program_x86_64_mswin()");
    return err;
}

//============================================================== END CG_FMT_x86_64_MSWIN

Error codegen_program(enum CodegenOutputFormat format, ParsingContext* context, Node* program) {
    Error err = ok;

    CodegenContext* cg_context = codegen_context_create(NULL);

    // Open file for writing.
    FILE* code = fopen("code.S", "w");

    if (format == CG_FMT_DEFAULT || format == CG_FMT_x86_64_MSWIN) {
        err = codegen_program_x86_64_mswin(code, cg_context, context, program);
    }
    fclose(code);
    if (err.type) { return err; }

    ERROR_PREP(err, ERROR_TODO, "codegen_program()");
    return err;
}
