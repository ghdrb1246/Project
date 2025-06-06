#ifndef CLIENT_CONTROLLER_H
#define CLIENT_CONTROLLER_H

#include <stdio.h>
#include <string.h>
#include "MenuIO.h"
#include "ClientController.h"
#include "InputInfo.h"

#ifdef _WIN32
    #include <winsock2.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <sys/socket.h>
#endif

static char loggedInUserId[50];

void sendRequest(int sock, const char *message);
int sendRequestWithResponse(int sock, const char *message, char *response);

MenuState handleMainMenu(int sock);
MenuState handleUserMenu(int sock);

#endif