#include <stdio.h>
#include <string.h>
#include "MenuIO.h"
#include "ClientController.h"

#ifdef _WIN32
    #include <winsock2.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <sys/socket.h>
#endif

void sendRequest(int sock, const char *message) {
    char recvBuf[1024];
    send(sock, message, strlen(message), 0);
    int len = recv(sock, recvBuf, 1023, 0);
    if (len > 0) {
        recvBuf[len] = '\0';
        printf("[응답] %s\n", recvBuf);
    }
}

MenuState handleMainMenu(int sock) {
    int meunNumber = mainMenu();
    char sendBuf[1024];
    char id[50], pw[50];

    switch (meunNumber) {
        case 1:
            signupMenu(id, pw);
            sprintf(sendBuf, "SIGNUP %s %s", id, pw);
            sendRequest(sock, sendBuf);
        return STATE_MAIN_MENU;
        
        // 수정 필요 | 로그인 성공 : STATE_USER_MENU, 실패 : STATE_MAIN_MENU
        case 2:
            loginMenu(id, pw);
            sprintf(sendBuf, "LOGIN %s %s", id, pw);
            sendRequest(sock, sendBuf);
        return STATE_USER_MENU;
        
        case 3:
            printf("프로그램을 종료합니다.\n");
        return STATE_EXIT;
        
        default:
            sprintf(sendBuf, "MENU %d", meunNumber);
            sendRequest(sock, sendBuf);
        return STATE_MAIN_MENU;
    }
}
MenuState handleUserMenu(int sock) {
    char id[50] = "user1"; // 로그인된 사용자 ID (추후 연동)
    int choice = userMenu(id);
    char sendBuf[256];
    char input1[50], input2[50];
    float value;

    switch (choice) {
        case 1: 
            mealMenu(input1, input2, &value);
            sprintf(sendBuf, "INPUT_MEAL %s %s %.2f", input1, input2, value);
        break;
        
        case 2: 
            workOutMenu(input1, input2, &value);
            sprintf(sendBuf, "INPUT_WORKOUT %s %s %.2f", input1, input2, value);
        break;
        
        case 3: 
            weightMenu(input1, &value);
            sprintf(sendBuf, "INPUT_WEIGHT %s %.2f", input1, value);
        break;
        
        case 4: 
            viewRecordsByDateMenu(); 
            sprintf(sendBuf, "GET_RECORD"); 
        break;

        case 5: 
            checkWeightLossProgressMenu(60.2, 50, 60);
            sprintf(sendBuf, "CHECK_PROGRESS"); 
        break;

        case 6: 
            feedBackMenu(60, 1000);
            sprintf(sendBuf, "FEEDBACK"); 
        break;

        case 7: 
            logOutMenu(id);
            sprintf(sendBuf, "LOGOUT %s", id); 
        return STATE_MAIN_MENU;

        case 8: 
            deleteIdMenu(id);
            sprintf(sendBuf, "DELETE_ID %s", id); 
        return STATE_MAIN_MENU;
        
        default:
            printf("잘못된 선택입니다.\n");
        return STATE_USER_MENU;
    }

    sendRequest(sock, sendBuf);
    return STATE_USER_MENU;
}