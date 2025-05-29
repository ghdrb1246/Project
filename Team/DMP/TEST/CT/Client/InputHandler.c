#include <stdio.h>
#include <string.h>
#include "InputHandler.h"
#include "MenuIO.h"

void inputLine(const char *prompt, char *buf, int size) {
    printf("%s", prompt);
    fgets(buf, size, stdin);
    buf[strcspn(buf, "\n")] = '\0';  // 개행 제거
}

void inputCredentials(char *id, char *pw) {
    inputLine("ID 입력: ", id, 50);
    inputLine("PW 입력: ", pw, 50);
}

int inputMenu() {
    int choice;

    printf("메뉴 선택: ");
    scanf("%d", &choice);
    getchar(); // 남은 개행 제거

    // printf("\n-----------------------------------------------------------\n");
    P_MENU_IN;
    
    // printf("===========================================================\n");
    P_MENU_END

    return choice;
}
