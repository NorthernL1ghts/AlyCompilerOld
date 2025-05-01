/*
 * Copyright (c) 2025 NorthernL1ghts
 */

/*
 * SHA HASH: d5632c04070941b120071561ba9834b3852a53c3afd79113ccc9f2df15f84a4b
 * Filename: src/error.c
 * Updated: 2025-05-01, 17:36:02
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