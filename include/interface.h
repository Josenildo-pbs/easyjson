#ifndef INTERFACE_H
#define INTERFACE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "core.h" 

void appendString(char **json_str, size_t *len, const char *str);
void stringifyContent(char **json_str, size_t *len, content_t *content, uint16_t formated);
char *stringify(json_t *json_root, uint8_t formated);
#endif