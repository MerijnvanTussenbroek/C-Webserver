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
void getNextToken();
char* checkIfInList(char** methods, int numOfMethods, char* element);

Token HTTPMethodParsing();
Token HTTPURIParsing();
Token HTTPVersionParsing();

Token* parseManyHeaders(Token* tokens);
Token parseBody (Token* tokens);

char* input;
Token currentToken;

char* methods[] = {"OPTIONS", "GET", "HEAD", "POST", "PUT", "DELETE", "TRACE", "CONNECT"};
char* schemes[] = {"HTTP", "HTTPS"};

Token* tokenizeRequest(char* request)
{
    input = malloc(4096);
    input = request;

    Token tokenizedRequest[100];

    tokenizedRequest[0] = HTTPMethodParsing();
    getNextToken();
    tokenizedRequest[1] = HTTPURIParsing();    
    getNextToken();
    tokenizedRequest[2] = HTTPVersionParsing();
    getNextToken();
    parseManyHeaders(tokenizedRequest);
    getNextToken();
    parseBody(tokenizedRequest);

    free(input);
    return tokenizedRequest;
}

void getNextToken()
{
    while(*input == ' ') input++;

    if(*input == 0x0D && *(input + 1) == 0x0A)
    {
        input += 2;
    }
}

Token HTTPMethodParsing()
{
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
}

Token HTTPURIParsing()
{
    //HTTP Request-URI parsing
    //Absolute URI
    if(!(strcmp(checkIfInList(schemes, 2, input), "NEGATIVE")) == 0)
    {
        char* beginning = input;
        Token t;
        t.type = TOKEN_URI;
        strcpy(t.fieldName, "Request-URI");
        while(*input != ' '){
            input++;
        }
        input--;
        strncpy(t.fieldValue, beginning, input - beginning);

        return t;
    }

    // Asterisk URI
    if(*input == '*'){
        Token t;
        t.type = TOKEN_URI;
        strcpy(t.fieldName, "Request-URI");
        strcpy(t.fieldValue, "*");

        return t;
    }

    // Path
    if(*input == '/'){
        char* beginning = input;
        Token t;
        t.type= TOKEN_URI;
        strcpy(t.fieldName, "Request-URI");
        while(*input != ' '){
            input++;
        }
        input--;
        strncpy(t.fieldValue, beginning, input - beginning);

        return t;
    }
}

Token HTTPVersionParsing()
{
    //HTTP version parsing
    if(((isdigit(*input)) && (*(input + 1) == '.') && (isdigit(*(input + 2)))))
    {
        Token t;
        t.type = TOKEN_VERSION;
        t.integerValue = ((*(input) - '0') + (((*(input + 2)) - '0')/10));
        input += 3;
        return t;
    }
}

Token* parseManyHeaders(Token* tokens){
    int i = 3;

    while(!(*input == 0x0D && *(input + 1) == 0x0A && *(input + 2) == 0x0D && *(input + 3) == 0x0A))
    {
        Token t;
        t.type = TOKEN_HEADER_FIELD;
        char* beginning = input;
        while(*input != ':')
        {
            input++;
        }
        strncpy(t.fieldName, beginning, input - 1 - beginning);
        beginning = input;
        while(!(*input == 0x0D && *(input + 1) == 0x0A))
        {
            input++;
        }
        strncpy(t.fieldValue, beginning, input - 1 - beginning);
        input += 1;
        tokens[i] = t;
        i++;
    }
}

Token parseBody(Token* tokens){
    Token t;
    Token length;
    length.type = TOKEN_END;
    for(int i = 0; i < 100; i++){
        if(tokens[i].type == TOKEN_HEADER_FIELD)
        {
            if(strcmp(tokens[i].fieldName, "Content-Length")){
                Token length = tokens[i];
                break;
            }
        }
    }

    if(length.type == TOKEN_END)
    {
        t.type = TOKEN_END;
        return t;
    }
    else
    {
        char* unparsedLength = length.fieldValue;
    }
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