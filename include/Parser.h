#ifndef PARSER_H
#define PARSER_H

#include <string.h>

#include <stdlib.h>

typedef enum{
    TOKEN_END,
    TOKEN_BEGIN,
    TOKEN_METHOD,
    TOKEN_URI,
    TOKEN_VERSION,
    TOKEN_HEADER_FIELD,
    TOKEN_BODY
} myTokenType;

typedef struct {
    myTokenType type;

    union{
        double integerValue;
    char method[8];
    struct{
        char fieldName[64];
        char fieldValue[256];
    };
    char path[100];
    unsigned char *body;
    };
} Token;

Token* tokenizeRequest(char* request);

void printToken(Token token);

#endif