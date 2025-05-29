#ifndef CLIENT_CONTROLLER_H
#define CLIENT_CONTROLLER_H
#include "MenuState.h"

MenuState handleMainMenu(int sock);
MenuState handleUserMenu(int sock);
void sendRequest(int sock, const char *message);
void inputCredentials(char *id, char *pw);

#endif