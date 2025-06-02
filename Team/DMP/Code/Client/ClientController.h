#ifndef CLIENT_CONTROLLER_H
#define CLIENT_CONTROLLER_H
#include "MenuState.h"

static char loggedInUserId[50];

void sendRequest(int sock, const char *message);
int sendRequestWithResponse(int sock, const char *message, char *response);

MenuState handleMainMenu(int sock);
MenuState handleUserMenu(int sock);

#endif