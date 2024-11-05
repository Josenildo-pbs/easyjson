#include "parser.h"

void expect(parser_t* parser, tokenType type) {
    if (parser->currentToken.type != type) {
        
        fprintf(stderr, "Unexpected token: %d\n", parser->currentToken.type);
        exit(EXIT_FAILURE);
    }
    parser->currentToken = nextToken(parser->lexer);
}

content_t* parseArray(parser_t* parser) {
    content_t* array = (content_t*)allocate(sizeof(content_t));
    array->type = ARR;
    array->data.arr = (array_t*)allocate(sizeof(array_t));
    array->data.arr->len = 0;
    array->data.arr->content = NULL;

    expect(parser, TOKEN_ARRAY_START);
    // parser->currentToken = nextToken(parser->lexer);

    while (parser->currentToken.type != TOKEN_ARRAY_END && parser->currentToken.type != TOKEN_EOF) {
        content_t* value = parseValue(parser);
        array->data.arr->len++; 
        array->data.arr->content = (content_t**)realloc(array->data.arr->content, array->data.arr->len * sizeof(content_t*));
        array->data.arr->content[array->data.arr->len-1] = value;
        if (parser->currentToken.type == TOKEN_COMMA) {
            parser->currentToken = nextToken(parser->lexer);
        }
    }

    expect(parser, TOKEN_ARRAY_END);
    return array;
}

content_t* parseValue(parser_t* parser) {
    content_t* content = NULL;
    switch (parser->currentToken.type) {
        case TOKEN_STRING:
            content = (content_t*)allocate(sizeof(content));
            parseString(parser, content);
            break;
        case TOKEN_NUMBER:
            content = (content_t*)allocate(sizeof(content));
            parseNumber(parser, content);
            break;
        case TOKEN_TRUE:
            content = (content_t*)allocate(sizeof(content));
            parseTrue(parser, content);
            break;
        case TOKEN_FALSE:
            content = (content_t*)allocate(sizeof(content));
            parseFalse(parser, content);
            break;
        case TOKEN_NULL:
         content = (content_t*)allocate(sizeof(content));
            parseNull(parser, content);
            break;
        case TOKEN_OBJECT_START:
            content = parseObject(parser);
            break;
        case TOKEN_ARRAY_START:
            content = parseArray(parser);
            break;
        default:
            fprintf(stderr, "Unexpected token in value parsing: %d\n", parser->currentToken.type);
            exit(EXIT_FAILURE);
    }
    return content;
}

void parseString(parser_t* parser, content_t* content) {
    content->type = STR;
    content->data.str= strdup(parser->currentToken.value);
    free(parser->currentToken.value);
    expect(parser, TOKEN_STRING);
}

void parseNumber(parser_t* parser, content_t* content) {
    char *endptr;
    uint8_t isFloat = 0;
    size_t len = strlen(parser->currentToken.value);
    for(size_t i=0; i<len; i++){
        if(parser->currentToken.value[i]=='.'){
            isFloat=1;
            break;
        }
    }
    if (isFloat) {
        content->type = FLOAT;  
        content->data.fnum = strtod(parser->currentToken.value, &endptr);
    } else {
        content->type = INT;
        content->data.num = strtol(parser->currentToken.value, &endptr, 10);
    }
    if(*endptr != '\0'){
        fprintf(stderr, "Number parser error\n");
        exit(EXIT_FAILURE);
    }
    free(parser->currentToken.value);
    expect(parser, TOKEN_NUMBER);
}

void parseTrue(parser_t* parser, content_t* content) {
    content->type = BOOL;
    content->data.num = 1;
    free(parser->currentToken.value);
    expect(parser, TOKEN_TRUE);
}

void parseFalse(parser_t* parser, content_t* content) {
    content->type = BOOL;
    content->data.num = 0;
    free(parser->currentToken.value);
    expect(parser, TOKEN_FALSE);
}

void parseNull(parser_t* parser, content_t* content) {
    content->type = EMPTY;
    content->data.num = 0;
    free(parser->currentToken.value);
    expect(parser, TOKEN_NULL);
}

content_t* parseObject(parser_t* parser) {
    content_t* object = (content_t*)allocate(sizeof(content_t));
    object->type = OBJ;
    object->data.arr = (array_t*)allocate(sizeof(array_t*));
    object->data.arr->len = 0;
    object->data.arr->content = NULL;

    expect(parser, TOKEN_OBJECT_START);

    while (parser->currentToken.type != TOKEN_OBJECT_END && parser->currentToken.type != TOKEN_EOF) {
        content_t* content = parseKeyValuePair(parser);
        object->data.arr->len++; 
        object->data.arr->content= (content_t**)realloc(object->data.arr->content, object->data.arr->len * sizeof(content_t*));
        object->data.arr->content[object->data.arr->len-1]=content;
        if (parser->currentToken.type == TOKEN_COMMA) {
            parser->currentToken = nextToken(parser->lexer);
        }
    }

    expect(parser, TOKEN_OBJECT_END);
    return object;
}

content_t* parseKeyValuePair(parser_t* parser) {
    if (parser->currentToken.type != TOKEN_STRING) {
        fprintf(stderr, "Expected string token for key\n");
        exit(EXIT_FAILURE);
    }
    entity_t* entity = (entity_t*)allocate(sizeof(entity_t));
    content_t* content = (content_t*)allocate(sizeof(content_t));

    content->type = PAIR;
    content->data.pair = entity;

    entity->name = strdup(parser->currentToken.value);
    free(parser->currentToken.value);
    expect(parser, TOKEN_STRING);
    expect(parser, TOKEN_COLON);
 
    entity->content = parseValue(parser);
    return content;
}

json_t* parser(char* str){
    lexer_t lexer;
    lexer.input=strdup(str);
    lexer.pos= 0;

    json_t* json= (json_t*)allocate(sizeof(json_t));
    json->root=NULL;

    entity_t *entity = (entity_t*)allocate(sizeof(entity_t));
    entity->name=strdup("root"); 
    entity->content=NULL;
    json->root=entity;
    
    parser_t parser;
    parser.lexer=&lexer;
    parser.json=json;
    parser.currentToken=nextToken(parser.lexer); 
    entity->content=parseObject(&parser);

    free((void*)lexer.input);
    if(entity->content==NULL){
        free(json);
    }
    return json;
}