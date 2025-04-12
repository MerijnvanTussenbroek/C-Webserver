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

#include "RequestHandler.h"

#include "Parser.h"
#include "FileManager.h"

#include <stdio.h>

char* processRequest(Token* request)
{
    printf("\nbegin processing request");

    char* method = request[1].method;
    char* response = malloc(4096);

    if(request[3].integerValue != 1.1)
    {
        strcpy(response,
        "HTTP/1.1 505 HTTP Version Not Supported\r\n" 
        "Content-Type: text/plain\r\n"
        "Content-Length: 42\r\n"
        "\r\n"
        "Error: HTTP version not supported by server."
        );

        return response;
    }
    
    if(strcmp(method, "OPTIONS") == 0)
    {
        OPTIONSRequest(request, response);
    }
    if(strcmp(method, "GET") == 0)
    {
        GETRequest(request, response);
    }
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

    free(request);

    return response;
}


void OPTIONSRequest(Token* request,char* response)
{
    (void)request;

    strcpy(
        response,
        "HTTP/1.1 204 No Content\r\n"
        "Allow: GET, POST, PUT, DELETE, HEAD, OPTIONS, TRACE\r\n"
        "Content-Length: 0\r\n"
    );
}

void GETRequest(Token* request, char* response)
{
    Token URI = request[2];

    char* fileType = getFileType(URI.path);
    char* connectedFileType = connectFileType(fileType);
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
        "Content-Type: %s\r\n"
        "Content-Length: %zu\r\n"
        "\r\n"
        "%s"
        ,connectedFileType, size, body
    );

    free(fileType);
    free(connectedFileType);
    free(body);

    return;
}

void HEADRequest(Token* request, char* response)
{
    Token URI = request[2];

    char* fileType = getFileType(URI.path);
    char* connectedFileType = connectFileType(fileType);    
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
        "Content-Type: %s\r\n"
        "Content-Length: %zu\r\n"
        "\r\n"
        ,connectedFileType, size
    );

    free(fileType);
    free(connectedFileType);
    free(body);

    return; 
}

void POSTRequest(Token* request, char* response)
{
    Token URI = request[2];

    Token* bodyPointer = findToken(request, TOKEN_BODY);
    Token bodyToken = *bodyPointer;

    int responseInfo = postFile(URI.path, bodyToken.body);

    if(responseInfo == 1)
    {
        strcpy(response,
            "HTTP/1.1 204 OK\r\n"
        );
    }
    else
    {
        strcpy(response,
            "HTTP/1.1 500 Internal Server Error\r\n"
            "Content-Type: text/plain\r\n"
            "\r\n"
            "Could not write file to disk.\r\n"
        );
    }
}

void PUTRequest(Token* request, char* response)
{
    Token URI = request[2];

    Token* bodyPointer = findToken(request, TOKEN_BODY);
    Token bodyToken = *bodyPointer;

    int responseInfo = putFile(URI.path, bodyToken.body);

    if(responseInfo == 1)
    {
        strcpy(response,
            "HTTP/1.1 204 OK\r\n"
        );
    }
    else
    {
        strcpy(response,
            "HTTP/1.1 500 Internal Server Error\r\n"
            "Content-Type: text/plain\r\n"
            "\r\n"
            "Could not write file to disk.\r\n"
        );
    }
}

void DELETERequest(Token* request, char* response)
{
    Token URI = request[2];

    int responseInfo = deleteFile(URI.path);

    if(responseInfo == 1)
    {
        strcpy(response,
            "HTTP/1.1 204 No Content\r\n"
        );
    }
    else
    {
        if(responseInfo == -1)
        {
            strcpy(response,
                "HTTP/1.1 404 Not Found\r\n"
                "Content-Type: text/plain\r\n"
                "Content-Length: 27\r\n"
                "\r\n"
                "Resource not found: /item/42\r\n"
            );
        }
        if(responseInfo == -2)
        {
            strcpy(response,
                "HTTP/1.1 409 Conflict\e\n"
                "Content-Type: text/plain\r\n"
                "Content-Length: 43\r\n"
                "\r\n"
                "Cannot delete item because it is in use.\r\n"
            );
        }
        if(responseInfo == -3)
        {
            strcpy(response,
                "HTTP/1.1 403 Method Not Allowed\r\n"
                "Content-Type: text/plain\r\n"
                "\r\n"
                "Cannot delete item because of lack of permission"
            );
        }
    }
}

void TRACERequest(Token* request, char* response)
{
    char* echo = malloc(10000 * sizeof(char));
    echo[0] = '\0';
    int length = 0;

    for(int i = 0; request[i].type != TOKEN_END; i++)
    {
        char* s = stringifyToken(request[i]);
        strcat(echo, s);
        length++;
        length += strlen(s);
        free(s);
    }

    echo[length] = '\0';

    echo = realloc(echo, strlen(echo) + 1);

    echo[length] = '\0';

    snprintf(response, 4096,
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: message/http\r\n"
        "Content-Length: %zu\r\n"
        "\r\n"
        "%s"
        ,strlen(echo), echo 
    );

    free(echo);

    return;
}

void CONNECTRequest(Token* request, char* response)
{
    (void)request;

    strcpy(
        response,
        "HTTP/1.1 405 Method Not Allowed\r\n"
        "Content-Type: text/plain\r\n"
        "Allow: GET, HEAD, POST, PUT, OPTIONS\r\n"
        "Content-Length: 23\r\n"
        "\r\n"
        "Method CONNECT not allowed"
    );
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