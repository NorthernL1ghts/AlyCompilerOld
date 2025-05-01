/*
 * Copyright (c) 2025 NorthernL1ghts
 */

/*
 * SHA HASH: 5370b2bc12ab38663e85fa89ff6831bcb344bbd6db90807c676ab6932ee651b6
 * Filename: src/error.c
 * Updated: 2025-05-01, 14:06:24
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