
#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include "Parser.h"

#include <stdio.h>

char* processRequest(Token* request);
Token* findToken(Token* tokens, myTokenType identifier);
Token* findHeaderToken(Token* tokens, char* identifier);

void OPTIONSRequest(char* response);
void GETRequest(Token* request, char* response);
void HEADRequest(Token* request, char* response);
void POSTRequest(Token* request, char* response);
void PUTRequest(Token* request, char* response);
void DELETERequest(Token* request, char* response);
void TRACERequest(Token* request, char* response);
void CONNECTRequest(Token* request, char* response);


#endif