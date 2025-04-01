/*
Request         ::= Request-Line CRLF
                    *(Header-Field CRLF)
                    CRLF
                    [Message-body]

Request-Line    ::= Method SP Request-URI SP HTTP-Version CRLF
Method          ::= "OPTIONS" | "GET" | "HEAD" | "POST" | "PUT" | "DELETE" | "TRACE" | "CONNECT"
Request-URI     ::=  Absolute-URI | Asterisk-URI | Path
HTTP-Version    ::= "HTTP/" DIGIT "." DIGIT
Header-Field    ::= Field-Name ":" OWS Field-Value OWS
Field-Value     ::= *(HTAB / SP / VCHAR / obs-text)
Message-Body    ::= *OCTET

Absolute-URI    ::= scheme "://" host [":" port] [abs-path] ["?" query] ["#" fragment]
scheme          ::= "http" | "https" | "ftp" | other schemes...
host            ::= <hostname or IP address>
port            ::= 1*DIGIT
abs-path        ::= "/" 1*(char)
query           ::= *(char)
fragment        ::= *(char)

Asterisk-URI    ::= "*"

Path            ::= "/" 1*( pchar )
pchar           ::= unreserved | pct-encoded | sub-delims | ":" | "@"
unreserved      ::= ALPHA / DIGIT / "-" / "." / "_" / "~"
pct-encoded     ::= "%" HEXDIG HEXDIG
sub-delims      ::= "!" / "$" / "&" / "'" / "(" / ")" / "*" / "+" / "," / ";" / "="
ALPHA           ::= "a"..."z" / "A"..."Z"
DIGIT           ::= "0"..."9"
HEXDIG          ::= DIGIT / "a"..."f" / "A"..."F"

SP      ::= %x20 (a space character)
HTAB    ::= %x09 (a tab whitespace)
CRLF    ::= %x0D %x0A (Carriage return + line feed)
OWS     ::= *(SP / HTAB) (optional white space)
VCHAR   ::= %x21-7E (visible character)
OCTET   ::= %x00-ff (any 8-bit byte value)
*/

#include <string.h>

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "Parser.h"

Token* tokenizeRequest(char* request);
void getNextToken();
char* checkIfInList(char** methods, int numOfMethods, char* element);

void somethingWentWrong(char* location);

Token HTTPMethodParsing();
Token HTTPURIParsing();
Token HTTPVersionParsing();

Token* parseManyHeaders(Token* tokens);
Token parseBody (Token* tokens);

void printToken(Token token);

char* input;
Token currentToken;

char* listOfMethods[] = {"OPTIONS", "GET", "HEAD", "POST", "PUT", "DELETE", "TRACE", "CONNECT"};
char* schemes[] = {"HTTP", "HTTPS"};

Token* tokenizeRequest(char* request)
{
    printf("\nbegin tokenising request");
    input = request;

    Token* tokenizedRequest = malloc(100 * sizeof(Token));

    tokenizedRequest[0] = HTTPMethodParsing();
    getNextToken();
    tokenizedRequest[1] = HTTPURIParsing();    
    getNextToken();
    tokenizedRequest[2] = HTTPVersionParsing();
    getNextToken();
    parseManyHeaders(tokenizedRequest);
    getNextToken();
    parseBody(tokenizedRequest);

    printf("\nParsing done");

    return tokenizedRequest;
}

//To print the token if necessary
void printToken(Token token)
{
    switch (token.type)
    {
    case TOKEN_BEGIN:
        printf("Begin \n");
        break;
    case TOKEN_METHOD:
        printf("Method: %s \n", token.method);
        break;
    case TOKEN_URI:
        printf("Uri: %s \n", token.path);
        break;
    case TOKEN_VERSION:
        printf("Version: %f \n", token.integerValue);
        break;
    case TOKEN_HEADER_FIELD:
        printf("Header-fields %s : %s \n", token.fieldName, token.fieldValue);
        break;
    case TOKEN_BODY:
        printf("Body: %s \n", token.body);
        break;
    case TOKEN_END:
        printf("end \n");
        break;
        printf("No token");
    default:
        break;
    }
}

void getNextToken()
{
    //This is here to skip and spaces or CRLF's
    while(*input == ' ') input++;

    if(*input == '\r' && *(input + 1) == '\n')
    {
        input += 2;
    }
}

Token HTTPMethodParsing()
{
    //HTTP method parsing
    Token t;
    //We first check if the method provided is in the list, otherwise we cannot parse it.
    if(!(strcmp(checkIfInList(listOfMethods, 8, input), "NEGATIVE")) == 0)
    {
        //If it is, we parse.
        char* method = checkIfInList(listOfMethods, 8, input);
        input += strlen(method);
        t.type = TOKEN_METHOD;
        strncpy(t.method, method, sizeof(t.method) - 1);
        t.method[sizeof(t.method) - 1] = '\0';
    }else
    {
        somethingWentWrong("\nMethod parsing");
        t.type = TOKEN_END;
    }

    return t;
}

Token HTTPURIParsing()
{
    //HTTP Request-URI parsing
    Token t;
    t.type = TOKEN_URI;

    char* beginning = input;
    while(*input != ' ' && *input != '\r' && *input != '\n')
    {
        input++;
    }

    size_t length = input - beginning;

    if(length >= sizeof(t.path)) length = sizeof(t.path) - 1;

    strncpy(t.path, beginning, length);

    t.path[length] = '\0';

    return t;
}

Token HTTPVersionParsing()
{
    //HTTP version parsing
    Token t;
    //We first see if the input even has a version to parse.
    if(strncmp(input, "HTTP/", 5) == 0 && isdigit(*(input + 5)) && *(input + 6) == '.' && isdigit(*(input + 7))) 
    {
        //if so, we parse it and increase the input pointer.
        t.type = TOKEN_VERSION;
        t.integerValue = ((*(input + 5) - '0') + ((*(input + 7) - '0') / 10.0));
        input += 8;
    }
    else{
        somethingWentWrong("\nVersion parsing");
        t.type = TOKEN_END;
    }

    return t;
}

Token* parseManyHeaders(Token* tokens){
    int i = 3;
    //We keep trying to parse headers until we come to a CRLF CRLF, which means that there are no more headers to parse.
    while(!(*input == '\r' && *(input + 1) == '\n' && *(input + 2) == '\r' && *(input + 3) == '\n'))
    {
        getNextToken();
        //we first parse the name of the header.
        Token t;
        t.type = TOKEN_HEADER_FIELD;

        char* beginning = input;

        while(*input != ':')
        {
            input++;
        }

        size_t length = input - beginning;
        if(length >= sizeof(t.fieldName)) length = sizeof(t.fieldName) - 1;
        strncpy(t.fieldName, beginning, length);
        t.fieldName[length] = '\0';

        input++;

        //then we parse the body
        beginning = input;
        while(!(*input == '\r' && *(input + 1) == '\n'))
        {
            input++;
        }

        //afterwards, we copy it all, put the token into the list and increase i
        length = input - beginning;
        if(length >= sizeof(t.fieldValue)) length = sizeof(t.fieldValue) - 1;
        strncpy(t.fieldValue, beginning, length);
        t.fieldValue[length] = '\0';
        tokens[i] = t;
        i++;
    }

    return tokens;
}

Token parseBody(Token* tokens){
    //parses the body token if it's there.
    Token t;
    Token length;

    char* unparsedLength;

    long parsedLength;

    length.type = TOKEN_END;
    //We first search for the token containing the content-length. If it's not there, that means there's no body to parse.
    for(int i = 0; i < 100; i++){
        if(tokens[i].type == TOKEN_HEADER_FIELD)
        {
            if(strcmp(tokens[i].fieldName, "Content-Length")){
                length = tokens[i];
                break;
            }
        }
    }

    if(length.type == TOKEN_END)
    {
        t.type = TOKEN_END;
    }
    else
    {
        //parse the body into the token
        unparsedLength = length.fieldValue;
        parsedLength = strtol(unparsedLength, NULL, 10);
        t.type = TOKEN_BODY;
        t.body = malloc(parsedLength + 1);
        memcpy(t.body, input, parsedLength);
        t.body[parsedLength] = '\0';
        input += parsedLength;
    }

    return t;
}

char* checkIfInList(char** methods, int numOfMethods, char* element)
{

    for(int i = 0; i < numOfMethods; i++){
        int length = strlen(methods[i]);
        if(strncmp(methods[i], element, length) == 0){
            return methods[i];
        }
    }

    return "NEGATIVE";
}

void somethingWentWrong(char* location)
{
    printf("\nSomething went wrong");
    printf("%s", location);
    getchar();
}