/*
 * Copyright (c) 2025 NorthernL1ghts
 */

/*
 * SHA HASH: 2b50add1fc5d21c18265fbbc4ba7ed4b819d50a1d59af4aad4962da65ae1090d
 * Filename: src/error.h
 * Updated: 2025-05-01, 17:36:02
*/
#ifndef COMPILER_ERROR_H
#define COMPILER_ERROR_H

// TODO: Add file path, byte offset, etc.
typedef struct Error {
  enum ErrorType {
    ERROR_NONE = 0,
    ERROR_ARGUMENTS,
    ERROR_TYPE,
    ERROR_GENERIC,
    ERROR_SYNTAX,
    ERROR_TODO,
    ERROR_MAX,
  } type;
  char *msg;
} Error;

void print_error(Error err);

extern Error ok;

#define ERROR_CREATE(n, t, msg) Error(n) = {(t), (msg)}

#define ERROR_PREP(n, t, message) \
  (n).type = (t);                  \
  (n).msg = (message);

#endif /* COMPILER_ERROR_H */