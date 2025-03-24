
#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include "Parser.h"

#include <stdio.h>

char* processRequest(Token* request);
Token* findToken(Token* tokens, myTokenType identifier);
Token* findHeaderToken(Token* tokens, char* identifier);

#endif