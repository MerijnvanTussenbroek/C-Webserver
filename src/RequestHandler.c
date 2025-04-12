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

    //We retrieve the method from the list and allocate memory for a response
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
    
    //We check what method was used
    if(strcmp(method, "OPTIONS") == 0)
    {
        OPTIONSRequest(response);
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


void OPTIONSRequest(char* response)
{
    //We simply return back all the options
    strcpy(
        response,
        "HTTP/1.1 204 No Content\r\n"
        "Allow: GET, POST, PUT, DELETE, HEAD, OPTIONS, TRACE\r\n"
        "Content-Length: 0\r\n"
    );
}

void GETRequest(Token* request, char* response)
{
    //We retrieve the file path
    Token URI = request[2];

    //We get the filetype, connect it to what needs to get into the HTTP response, and retrieve the file's content
    char* fileType = getFileType(URI.path);
    char* connectedFileType = connectFileType(fileType);
    char* body = openFile(URI.path);

    if(body == NULL)
    {
        //The file doesn't exist, we return the famous 404 error
        strcpy(response,
        "HTTP/1.1 404 Not Found\r\n" 
        "Content-Type: text/plain\r\n"
        "Content-Length: 19\r\n"
        "\r\n"
        "Page not found."
        );

        free(fileType);
        free(connectedFileType);
    
        return;
    }

    size_t size = strlen(body);

    //We print the contents into the response
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
    //We retrieve the filepath
    Token URI = request[2];

    //We get the file type and its associated HTTP response, and retrieve the files content
    char* fileType = getFileType(URI.path);
    char* connectedFileType = connectFileType(fileType);    
    char* body = openFile(URI.path);

    if(body == NULL)
    {
        //If the file was not found, we return a 404 error
        strcpy(response,
        "HTTP/1.1 404 Not Found\r\n" 
        "Content-Type: text/plain\r\n"
        "Content-Length: 19\r\n"
        "\r\n"
        "Page not found."
        );

        free(fileType);
        free(connectedFileType);
    
        return;
    }

    size_t size = strlen(body);

    //We put the contents into the response
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
    //We retrieve the filepath
    Token URI = request[2];

    //We specifically retrieve the body, which is what will be put into the file
    Token* bodyPointer = findToken(request, TOKEN_BODY);
    Token bodyToken = *bodyPointer;

    //We let the filemanager handle the rest
    int responseInfo = postFile(URI.path, (char*)bodyToken.body);

    if(responseInfo == 1)
    {
        //The file has been put into the system
        strcpy(response,
            "HTTP/1.1 204 OK\r\n"
        );
    }
    else
    {
        //Something went wrong while putting the file in the system
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
    //We retrieve the filepath
    Token URI = request[2];

    //We specifcially retrieve the body token, as this will be what will be appended in the file
    Token* bodyPointer = findToken(request, TOKEN_BODY);
    Token bodyToken = *bodyPointer;

    //We let the file manager do the rest
    int responseInfo = putFile(URI.path, (char*)bodyToken.body);

    if(responseInfo == 1)
    {
        //Everything went well
        strcpy(response,
            "HTTP/1.1 204 OK\r\n"
        );
    }
    else
    {
        //An error has occured
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
    //We retrieve the filepath
    Token URI = request[2];

    //We let the fileManager delete the file
    int responseInfo = deleteFile(URI.path);

    if(responseInfo == 1)
    {
        //Everything went according to plan
        strcpy(response,
            "HTTP/1.1 204 No Content\r\n"
        );
    }
    else
    {
        if(responseInfo == -1)
        {
            //The file could not be found
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
            //The file is currently being used
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
            //The requester does not have the permission to delete the file
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
    //We allocate memory to reconstruct the original request
    char* echo = malloc(10000 * sizeof(char));
    echo[0] = '\0';
    int length = 0;

    for(int i = 0; request[i].type != TOKEN_END; i++)
    {
        //We iterate over the request to reconstruct it in the echo string
        char* s = stringifyToken(request[i]);
        strcat(echo, s);
        length++;
        length += strlen(s);
        free(s);
    }

    echo[length] = '\0';

    //We reallocate memory as required
    echo = realloc(echo, strlen(echo) + 1);

    echo[length] = '\0';

    //We put the cho in the response
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
    //This webserver will not support the CONNECT HTTP request
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

//a function to find a token based on its type
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

//a function to find a headertoken based on the headers name
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