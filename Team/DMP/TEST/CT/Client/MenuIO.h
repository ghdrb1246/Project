#ifndef MENU_IO_H
#define MENU_IO_H

#define P_MENU_TITLE(s)     printf("================= [ %s ] =================\n", s)
#define P_MENU_IN           printf("----------------------------------------------------------\n")
#define P_MENU_END          printf("==========================================================\n\n")
#define P_MENU_SB_S(s, l)   printf("[ %s %s------------------------------------------ ]\n", s, l)

void mainMenu();
void userMenu();

#endif