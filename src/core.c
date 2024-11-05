#include "core.h"

uint16_t testptr =0;
void* allocate(size_t size) {
    testptr=testptr+1;
    void* ptr = malloc(size);
    return ptr;
}


void printContent(content_t* content){
    switch (content->type)
    {
    case INT:
        printf("%d\n",content->data.num);
        break;
    case FLOAT:
        printf("%f\n",content->data.fnum);
        break;
    case EMPTY:
        printf("%d\n",content->data.num);
        break;
    case BOOL:
        printf("%d\n",content->data.num);
        break;
    default:
        break;
    }
}
void deallocate(content_t* content) {
    if (content == NULL) {
        return;
    }
    if(content->type==STR){
        // printf("%s\n",content->data.str);
        free(content->data.str);
        // free(content);
    }
    else if(content->type==PAIR){
      
        free(content->data.pair->name);
        deallocate(content->data.pair->content);
        free(content->data.pair);

    }
    else if(content->type==ARR || content->type== OBJ){
        printf("arr size= %d\n",content->data.arr->len);
        for (size_t i = 0; i < content->data.arr->len; i++)
        {
            deallocate(content->data.arr->content[i]);
        }
        free(content->data.arr->content);
        free(content->data.arr);
    }
        
    free(content);
    
}

void cleanup(json_t* json) {
    if (json != NULL) {
        deallocate(json->root->content);
        free(json->root->name);
        free(json);
    }
}

entity_t* createEntity(char* name) {
    
    entity_t* entity = (entity_t*)allocate(sizeof(entity_t));

    if (entity == NULL) {
        return NULL;
    }

    entity->name = strdup(name);  
    if (entity->name == NULL) {
        free(entity);
        return NULL;
    }
    entity->content = NULL;  
    return entity;
}

content_t* createData(dataType_t type, void* value) {
    content_t* content = (content_t*)allocate(sizeof(content_t));

    if (content == NULL) {
        return NULL;
    }

    switch (type) {
        case STR:
            content->data.str = strdup((char*)value);
            if (content->data.str == NULL) {
                deallocate(content);
                return NULL;
            }
            break;

        case INT:
            content->data.num = *(int*)value;
            break;

        case FLOAT:
            content->data.fnum = *(float*)value;
            break;

        case OBJ:
            content->data.pair = (entity_t*)value;
            break;

        case ARR:
            content->data.arr = (array_t*)value;
            break;
        default:
            fprintf(stderr, "Unsupported content type.\n");
            deallocate(content);
            return NULL;
    }

    return content;
}

entity_t* searchEntity(entity_t* root, const char* path) {
    if (root == NULL || path == NULL) {
        return NULL;
    }

    char* path_copy = strdup(path);
    if (path_copy == NULL) {
        return NULL;
    }

    char* token = strtok(path_copy, ".");
    entity_t* current_entity = root;

    while (token != NULL && current_entity != NULL) {
        int found = 0;

        if (current_entity->content == NULL) {
            free(path_copy);
            return NULL; // If there's no content, we can't continue
        }

        // Check if current entity's content is of type ARR
        if (current_entity->content->type == OBJ) {
            array_t* array = current_entity->content->data.arr;

            if (array == NULL) {
                free(path_copy);
                return NULL; // Invalid array pointer
            }

            for (uint16_t i = 0; i < array->len; i++) {
                if (array->content[i] != NULL &&
                    array->content[i]->data.pair != NULL &&
                    array->content[i]->type == PAIR &&
                    strcmp(array->content[i]->data.pair->name, token) == 0) {
                    current_entity = array->content[i]->data.pair;
                    found = 1;
                    break;
                }
            }
        } else {
            free(path_copy);
            return NULL; // Current entity's content is not an array, so cannot proceed further
        }

        if (!found) {
            free(path_copy);
            return NULL;  // Entity not found in the current level
        }

        token = strtok(NULL, ".");  // Move to the next token
    }

    free(path_copy);
    return current_entity;  // Return the final entity if found
}


