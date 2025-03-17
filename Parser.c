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

Token* tokenizeRequest(char* request);
Token getNextToken(Token* tokenizeRequest);
char* checkIfInList(char** methods, int numOfMethods, char* element);

char* input;
Token currentToken;

char* methods[] = {"OPTIONS", "GET", "HEAD", "POST", "PUT", "DELETE", "TRACE", "CONNECT"};
char* schemes[] = {"HTTP", "HTTPS"};

Token* tokenizeRequest(char* request)
{
    input = malloc(4096);
    input = request;

    Token tokenizedRequest[100];

    for(int i = 0; i < 100; i++)
    {
        tokenizedRequest[i] = getNextToken(tokenizedRequest);
        if(tokenizedRequest[i].type == TOKEN_END)
        {
            break;
        }
    }

    free(input);
    return tokenizedRequest;
}

Token getNextToken(Token* tokenizeRequest)
{
    while(*input == ' ') input++;

    if(*input == 0x0D && *(input + 1) == 0x0A)
    {
        input += 2;
    }

    //HTTP method parsing
    if(!(strcmp(checkIfInList(methods, 8, input), "NEGATIVE")) == 0)
    {
        char* method = checkIfInList(methods, 8, input);
        input += strlen(method);
        Token t;
        t.type = TOKEN_METHOD;
        strncpy(t.method, method, strlen(method));
        return t;
    }

    //HTTP Request-URI parsing
    //Absolute URI
    if(!(strcmp(checkIfInList(schemes, 2, input), "NEGATIVE")) == 0)
    {

    }

    // Asterisk URI


    // Path
    

    //HTTP version parsing
    if(((isdigit(*input)) && (*(input + 1) == '.') && (isdigit(*input))))
    {
        input += 3;
        //return (Token){TOKEN_VERSION, ((*input - '0') + (((*(input + 1)) - '0')/10)), "", ""};
    }

    Token t;
    t.type = TOKEN_END;
    t.integerValue = 0;
    return t;
}

char* checkIfInList(char** methods, int numOfMethods, char* element)
{

    for(int i = 0; i < numOfMethods; i++){
        if(strcmp(methods[i], element) == 0){
            return methods[i];
        }
    }

    return "NEGATIVE";
}