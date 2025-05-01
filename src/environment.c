/*
 * Copyright (c) 2025 NorthernL1ghts
 */

/*
 * SHA HASH: 9c35ca3b4c23cac3944685e67f62f51748e4df0fd6865b8b89b561e9fa183168
 * Filename: src/environment.c
 * Updated: 2025-05-01, 13:13:40
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