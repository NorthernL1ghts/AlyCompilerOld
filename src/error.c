/*
 * Copyright (c) 2025 NorthernL1ghts
 */

/*
 * SHA HASH: eabe9ae1faf81e68c7dfa2cedc1f64a4741ebcb8e536c35499e51ad55e451290
 * Filename: src/error.c
 * Updated: 2025-05-01, 13:13:40
*/
#include <error.h>

#include <assert.h>
#include <stddef.h>
#include <stdio.h>

Error ok = { ERROR_NONE, NULL };

void print_error(Error err) {
	if (err.type == ERROR_NONE) { return; }
	printf("ERROR: ");
	assert(ERROR_MAX == 6);
	switch (err.type) {
	default:
		printf("Unknown error type...");
		break;
	case ERROR_TODO:
		printf("TODO (not implemented)");
		break;
	case ERROR_SYNTAX:
		printf("Invalid syntax");
		break;
	case ERROR_TYPE:
		printf("Mismatched types");
		break;
	case ERROR_ARGUMENTS:
		printf("Invalid arguments");
		break;
	case ERROR_GENERIC:
		break;
	case ERROR_NONE:
		break;
	}
	putchar('\n');
	if (err.msg) {
		printf("     : %s\n", err.msg);
	}
}