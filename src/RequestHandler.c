/*

Response        ::= Status-Line CRLF
                    *(Header-Field CRLF)
                    CRLF
                    [Message-Body]

Status-Line     ::= HTTP-Version SP Status-Code SP Reason-Phrase CRLF
Status-Code     ::= 3Digit
Reason-Phrase   ::= *(HTAB / SP / VCHAR / obs-text)

SP      ::= %x20 (a space character)
HTAB    ::= %x09 (a tab whitespace)
CRLF    ::= %x0D %x0A (Carriage return + line feed)
OWS     ::= *(SP / HTAB) (optional white space)
VCHAR   ::= %x21-7E (visible character)
OCTET   ::= %x00-ff (any 8-bit byte value)

*/


#include "Parser.h"
#include "FileManager.h"

#include <stdio.h>

void GETRequest(Token* request, char* response);

char* processRequest(Token* request)
{
    printf("\nbegin processing request");

    char* method = request[0].method;
    char* response = malloc(4096);
    /*
    if(strcmp(method, "OPTIONS") == 0)
    {
        OPTIONSRequest(request, response);
    }
    */
    if(strcmp(method, "GET") == 0)
    {
        GETRequest(request, response);
    }
    /*
    if(strcmp(method, "HEAD") == 0)
    {
        HEADRequest(request, response);
    }
    if(strcmp(method, "POST") == 0)
    {
        POSTRequest(request, response);
    }
    if(strcmp(method, "PUT") == 0)
    {
        PUTRequest(request, response);
    }
    if(strcmp(method, "DELETE") == 0)
    {
        DELETERequest(request, response);
    }
    if(strcmp(method, "TRACE") == 0)
    {
        TRACERequest(request, response);
    }
    if(strcmp(method, "CONNECT") == 0)
    {
        CONNECTRequest(request, response);
    }
        */

    free(request);

    return response;
}

/*
void OPTIONSRequest(char* request, char* response)
{

}
*/
void GETRequest(Token* request, char* response)
{
    Token URI = request[1];
    Token version = request[2];

    if(version.integerValue != 1.1)
    {
        strcpy(response,
        "HTTP/1.1 505 HTTP Version Not Supported\r\n" 
        "Content-Type: text/plain\r\n"
        "Content-Length: 42\r\n"
        "\r\n"
        "Error: HTTP version not supported by server."
        );

        return;
    }
    
    char* body = openFile(URI.path);

    if(body == NULL)
    {
        strcpy(response,
        "HTTP/1.1 404 Not Found\r\n" 
        "Content-Type: text/plain\r\n"
        "Content-Length: 19\r\n"
        "\r\n"
        "Page not found."
        );
    
        return;
    }

    size_t size = strlen(body);

    snprintf(response, 4096,
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: %zu\r\n"
        "\r\n"
        "%s"
        , size, body
    );

    return;
}
/*
void HEADRequest(char* request, char* response)
{

}

void POSTRequest(char* request, char* response)
{

}

void PUTRequest(char* request, char* response)
{

}

void DELETERequest(char* request, char* response)
{

}

void TRACERequest(char* request, char* response)
{

}

void CONNECTRequest(char* request, char* response)
{

}
*/

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