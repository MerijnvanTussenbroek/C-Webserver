
#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include "Parser.h"

#include <stdio.h>

char* processRequest(Token* request);
Token* findToken(Token* tokens, myTokenType identifier);
Token* findHeaderToken(Token* tokens, char* identifier);

void OPTIONSRequest(char* request, char* response);
void GETRequest(char* request, char* response);
void HEADRequest(char* request, char* response);
void POSTRequest(char* request, char* response);
void PUTRequest(char* request, char* response);
void DELETERequest(char* request, char* response);
void TRACERequest(char* request, char* response);
void CONNECTRequest(char* request, char* response);

#endif