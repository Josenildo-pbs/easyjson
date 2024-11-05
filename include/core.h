#ifndef CORE_H
#define CORE_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

typedef enum {
    FLOAT,
    INT,
    STR,
    OBJ,
    BOOL,
    EMPTY,
    ARR,
    PAIR,
}dataType_t;

typedef struct array{
    uint16_t len;
    struct content** content;
}array_t;

typedef struct content{
    dataType_t type;
    union {
        char *str;
        uint32_t num;
        double fnum;
        struct entity* pair;
        struct array* arr; 
    } data;
}content_t;

typedef struct entity{
    char* name;
    content_t* content;
}entity_t;

typedef struct {
    entity_t* root;
}json_t;

void* allocate(size_t size);
void deallocate(content_t* content);
void cleanup();
entity_t* createEntity(char* name);
content_t* createData(dataType_t type, void* value);
entity_t* searchEntity(entity_t* root, const char* path);
extern uint16_t testptr;
#endif 
