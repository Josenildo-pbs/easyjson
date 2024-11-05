#include "core.h"
#include "lexer.h"
#include "parser.h"
#include "interface.h"
// char* input = "{\n"
// "  \"basic_string\": \"Hello, World!\",\n"
// "  \"basic_integer\": 12345,\n"
// "  \"basic_float\": 123.456,\n"
// "  \"basic_boolean\": true,\n"
// "  \"null_value\": null,\n"
// "  \"simple_arr\":[10.27,10.30,5.5],\n"
// "  \"obj\":{\n" 
// "            \"item\":15.13,\n"
// "            \"arr\":[1,2,3,4,5],\n"
// "            \"obj\":{\"item\":2001.50}\n"
// "            },\n"
// "  \"complex_arr\":[\"name\",172,5.75,{\"item\":77.55}],\n"
// "}";

char* input = "{\"basic_string\":\"Hello, World!\", \"basic_integer\": 12345}";


size_t pos = 0;

int main() { 
    // lexer_t lexer;
    // lexer.input=strdup(input);
    // lexer.pos= 0;
    // while (TOKEN_EOF!=nextToken(&lexer).type)
    // {
    //     nextToken(&lexer);
    // }
    
    json_t *json = parser(input);
    entity_t* member = searchEntity(json->root, "basic_integer");
    if(member != NULL){
        printf("%s\n", member->name);
        printf("%d\n", member->content->data.num);
    } 
    // deallocate(json->root->content);
    // free(json->root);
    // free(json);
    // char* str = stringify(json,0);
    // printf("%s\n",str);
    return 0;
}
