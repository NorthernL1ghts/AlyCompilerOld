/*
 * Copyright (c) 2025 NorthernL1ghts. All rights reserved.
 *
 * This file is part of the Aly Language / AlyCompiler.
 * Unauthorized copying, modification, or distribution of this file,
 * via any medium, is strictly prohibited except as permitted under
 * the terms outlined in the LICENSE file.
 *
 * For licensing details, contributions, or inquiries, refer to LICENSE
 * or contact:
 * NorthernL1ghts Software Division
 */

#include <environment.h>
#include <parser.h>

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

Environment* environment_create(Environment* parent) {
	Environment* env = malloc(sizeof(Environment));
	assert(env && "Could not allocate memory to new environment");
	env->parent = parent;
	env->bind = NULL;
	return env;
}

int environment_set(Environment* env, Node* id, Node* value) {
	// Over-write existing value if ID is already bound in environment.
	if (!env || !id || !value) { return 0; }

	Binding* binding_it = env->bind;
	while (binding_it) {
		if (node_compare(binding_it->id, id)) {
			binding_it->value = value;
			return 2;
		}
		binding_it = binding_it->next;
	}
	// Create new binding.
	Binding* binding = malloc(sizeof(Binding));
	assert(binding && "Could not allocate new binding for environment");
	binding->id = id;
	binding->value = value;
	binding->next = env->bind;
	env->bind = binding;
	return 1;
}

int environment_get(Environment env, Node* id, Node* result) {
	Binding* binding_it = env.bind;
	while (binding_it) {
		if (node_compare(binding_it->id, id)) {
			*result = *binding_it->value;
			return 1;
		}
		binding_it = binding_it->next;
	}
	return 0;
}

int enviornment_get_by_symbol(Environment env, char* symbol, Node* result) {
	Node* symbol_node = node_symbol(symbol);
	int status = environment_get(env, symbol_node, result);
	free(symbol_node);
	return status;
}