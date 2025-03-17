#ifdef PARSER_H
#define PARSER_H

typedef enum{
    TOKEN_BEGIN,
    TOKEN_METHOD,
    TOKEN_URI,
    TOKEN_VERSION,
    TOKEN_HEADER_FIELD,


    TOKEN_END
} TokenType;

typedef struct {
    TokenType type;
    double integerValue;
    char method[8];
    char path[100];
} Token;

Token* tokenizeRequest(char*);

#endif