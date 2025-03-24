
#include "Parser.h"

#include <stdio.h>

char* processRequest(Token* request)
{
    printf("\n begin processing request");

    char* method = request[0].method;
    char* response = malloc(4096);
    
    if(strcmp(method, "OPTIONS") == 0)
    {

    }
    if(strcmp(method, "GET") == 0)
    {

    }
    if(strcmp(method, "HEAD") == 0)
    {
        
    }
    if(strcmp(method, "POST") == 0)
    {
        
    }
    if(strcmp(method, "PUT") == 0)
    {
        
    }
    if(strcmp(method, "DELETE") == 0)
    {
        
    }
    if(strcmp(method, "TRACE") == 0)
    {
        
    }
    if(strcmp(method, "CONNECT") == 0)
    {
        
    }



    free(request);

    return response;
}

Token* findToken(Token* tokens, myTokenType identifier)
{
    Token* list = tokens;

    for(int i = 0; list[i].type != TOKEN_END; i++)
    {
        if(list[i].type == identifier)
        {
            return &(list[i]);
        }
    }

    return NULL;
}

Token* findHeaderToken(Token* tokens, char* identifier)
{
    Token* list = tokens;

    for(int i = 0; list[i].type != TOKEN_END; i++)
    {
        if(list[i].fieldName == identifier)
        {
            return &(list[i]);
        }
    }

    return NULL;
}