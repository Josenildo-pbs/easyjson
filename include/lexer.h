#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum {
    TOKEN_OBJECT_START,
    TOKEN_OBJECT_END,
    TOKEN_ARRAY_START,
    TOKEN_ARRAY_END,
    TOKEN_STRING,
    TOKEN_NUMBER,
    TOKEN_COMMA,
    TOKEN_COLON,
    TOKEN_TRUE,
    TOKEN_FALSE,
    TOKEN_NULL,
    TOKEN_INVALID,
    TOKEN_EOF
} tokenType;

typedef struct {
    const char *input;
    size_t pos;
} lexer_t;

typedef struct {
    tokenType type;
    char *value;
} token_t;

token_t nextToken(lexer_t *lexer);
char currentChar(lexer_t *lexer); 
void advance(lexer_t *lexer);
#endif