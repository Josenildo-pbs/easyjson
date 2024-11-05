#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

void appendString(char **json_str, size_t *len, const char *str) {
    size_t str_len = strlen(str);
    *json_str = (char *)realloc(*json_str, *len + str_len + 1);
    strcpy(*json_str + *len, str);
    *len += str_len;
}

void space(char **json_str, size_t *len, uint16_t num){
    for(size_t i=0; i<num; i++){
        appendString(json_str, len, "   ");
    }
}

void stringifyContent(char **json_str, size_t *len, content_t *content, uint16_t formated) {
    switch (content->type){
        case STR:
            appendString(json_str, len, "\"");
            appendString(json_str, len, content->data.str);
            appendString(json_str, len, "\"");
            break;
        case INT:
            char num_buf[20];
            snprintf(num_buf, sizeof(num_buf), "%d", content->data.num);
            appendString(json_str, len, num_buf);
            break;
        case FLOAT:
            char float_buf[20];
            snprintf(float_buf, sizeof(float_buf), "%f", content->data.fnum);
            appendString(json_str, len, float_buf);
            break;
        case BOOL:
            appendString(json_str, len, (content->data.num)?"true":"false");
            break;
        case EMPTY:
            appendString(json_str, len,"null");
            break;
        case OBJ:
            appendString(json_str, len, "{");
            if(formated)appendString(json_str, len, "\n");
            for (uint16_t i = 0; i < content->data.arr->len; i++) {
                if(formated)space(json_str, len, formated);
                appendString(json_str, len, "\"");
                appendString(json_str, len, content->data.arr->content[i]->data.pair->name);
                appendString(json_str, len, "\":");
                stringifyContent(json_str, len, content->data.arr->content[i]->data.pair->content, formated?formated+1:formated);
                appendString(json_str, len, ",");
                if(formated)appendString(json_str, len, "\n");
            }
            if(formated)space(json_str, len, formated);
            appendString(json_str, len, "}");
            break;
        case ARR:
            appendString(json_str, len, "[");
            if(formated)appendString(json_str, len, "\n");
            if(formated)space(json_str, len, formated);
            for (uint16_t i = 0; i < content->data.arr->len; i++) {
                if (i > 0) {
                    appendString(json_str, len, ",");
                    if(formated)appendString(json_str, len, "\n");
                    if(formated)space(json_str, len, formated);
                }
                stringifyContent(json_str, len, content->data.arr->content[i], formated);
            }
            appendString(json_str, len, "]");
            break;
        default:
            appendString(json_str, len, "null");
            break;
    }
}

char *stringify(json_t *json, uint8_t formated) {
    if (!json || !json->root) {
        return NULL;
    }
    char *json_str = NULL;
    size_t len = 0;
    
    stringifyContent(&json_str, &len, json->root->content, formated);
    return json_str;
}
