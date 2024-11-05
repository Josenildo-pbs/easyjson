#ifndef PARSER_H
#define PARSER_H
#include "core.h"
#include "lexer.h"

typedef struct {
    lexer_t *lexer;
    json_t *json;
    token_t currentToken;
} parser_t;

void expect(parser_t* parser, tokenType type);
content_t* parseArray(parser_t* parser);
content_t* parseValue(parser_t* parser);
void parseString(parser_t* parser, content_t* content);
void parseNumber(parser_t* parser, content_t* content);
void parseTrue(parser_t* parser, content_t* content);
void parseFalse(parser_t* parser, content_t* content);
void parseNull(parser_t* parser, content_t* content);
content_t* parseObject(parser_t* parser);
content_t* parseKeyValuePair(parser_t* parser);
json_t* parser(char* str);
#endif