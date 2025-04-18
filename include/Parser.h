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
void getNextToken();
char* checkIfInList(char** methods, int numOfMethods, char* element);

void somethingWentWrong(char* location);

Token HTTPMethodParsing();
Token HTTPURIParsing();
Token HTTPVersionParsing();

Token* parseManyHeaders(Token* tokens);
void parseBody (Token* tokens);

void printToken(Token token);
char* stringifyToken(Token token);

void deleteTokens(Token* tokens);
#endif