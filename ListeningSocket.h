
#define LISTENINGSOCKET_H

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

SOCKET initializeListeningSocket(struct sockaddr_in server);
void bindListeningSocket(struct sockaddr_in server, SOCKET s);
SOCKET initializeAcceptingSocket(struct sockaddr_in server, SOCKET listeningSocket);
void closeListeningSocket(SOCKET s);
void closeAcceptingSocket(SOCKET s);