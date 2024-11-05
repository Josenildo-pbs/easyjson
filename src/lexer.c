#include "lexer.h"

char currentChar(lexer_t *lexer) {
    return lexer->input[lexer->pos];
}

void advance(lexer_t *lexer) {
    lexer->pos++;
}

token_t nextToken(lexer_t *lexer) {
    while (isspace(currentChar(lexer))) {
        advance(lexer);
    }

    token_t token;
    token.value = NULL;

    switch (currentChar(lexer)) {
        case '{':
            token.type = TOKEN_OBJECT_START;
            advance(lexer);
            break;
        case '}':
            token.type = TOKEN_OBJECT_END;
            advance(lexer);
            break;
        case '[':
            token.type = TOKEN_ARRAY_START;
            advance(lexer);
            break;
        case ']':
            token.type = TOKEN_ARRAY_END;
            advance(lexer);
            break;
        case ',':
            token.type = TOKEN_COMMA;
            advance(lexer);
            break;
        case ':':
            token.type = TOKEN_COLON;
            advance(lexer);
            break;
        case '"': {
            size_t start = ++lexer->pos;
            while (currentChar(lexer) != '"') {
                if (currentChar(lexer) == '\0') {
                    token.type = TOKEN_INVALID;
                    return token;
                }
                advance(lexer);
            }
            size_t length = lexer->pos - start;
            token.value = (char *)malloc(length + 1);
            strncpy(token.value, lexer->input + start, length);
            token.value[length] = '\0';
            token.type = TOKEN_STRING;
            advance(lexer);
            break;
        }
        case 't': {
            if (strncmp(lexer->input + lexer->pos, "true", 4) == 0) {
                token.type = TOKEN_TRUE;
                token.value = strdup("true");
                lexer->pos += 4;
            } else {
                token.type = TOKEN_INVALID;
            }
            break;
        }
        case 'f': {
            if (strncmp(lexer->input + lexer->pos, "false", 5) == 0) {
                token.type = TOKEN_FALSE;
                token.value = strdup("false");
                lexer->pos += 5;
            } else {
                token.type = TOKEN_INVALID;
            }
            break;
        }
        case 'n': {
            if (strncmp(lexer->input + lexer->pos, "null", 4) == 0) {
                token.type = TOKEN_NULL;
                token.value = strdup("null");
                lexer->pos += 4;
            } else {
                token.type = TOKEN_INVALID;
            }
            break;
        }
        case '\0':
            token.type = TOKEN_EOF;
            break;
        default:
            if (isdigit(currentChar(lexer)) || currentChar(lexer) == '-') {
                size_t start = lexer->pos;
                while (isdigit(currentChar(lexer)) || currentChar(lexer) == '.' || currentChar(lexer) == '-' || currentChar(lexer) == '+') {
                    advance(lexer);
                }
                size_t length = lexer->pos - start;
                token.value = (char *)malloc(length + 1);
                strncpy(token.value, lexer->input + start, length);
                token.value[length] = '\0';
                token.type = TOKEN_NUMBER;
            } else {
                token.type = TOKEN_INVALID;
                advance(lexer);
            }
            break;
    }
    // char test[] = {'{','}','[',']','"','n',',',':','T','F','N','i','f'};
    // printf("current token: %c\n", test[token.type]);
    return token;
}
