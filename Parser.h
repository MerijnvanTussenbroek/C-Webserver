#ifdef PARSER_H
#define PARSER_H

#include <string.h>

typedef enum{
    TOKEN_BEGIN,
    TOKEN_METHOD,
    TOKEN_URI,
    TOKEN_VERSION,
    TOKEN_REQUESTI_URI,
    TOKEN_HEADER_FIELD,
    TOKEN_BODY,

    TOKEN_END
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

Token* tokenizeRequest(char*);

#endif