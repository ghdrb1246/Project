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

void sendRequest(int sock, const char *message) {
    char recvBuf[1024];
    send(sock, message, strlen(message), 0);
    int len = recv(sock, recvBuf, 1023, 0);
    if (len > 0) {
        recvBuf[len] = '\0';
        printf("[응답] %s\n", recvBuf);
    }
}

int sendRequestWithResponse(int sock, const char *message, char *response) {
    char recvBuf[1024];
    send(sock, message, strlen(message), 0);
    int len = recv(sock, recvBuf, 1023, 0);
    if (len > 0) {
        recvBuf[len] = '\0';
        strcpy(response, recvBuf);
        return 1;
    }
    return 0;
}

MenuState handleMainMenu(int sock) {
    int meunNumber = mainMenu();
    char sendBuf[1024];
    char id[ID_SIZE], pw[PW_SIZE];

    UserSignupInfo *USI = USImalloc();
    
  /*   if (!USI) {
        printf("메모리 할당 실패!\n");
        return;
    }
     */
    switch (meunNumber) {
        case 1:
            signupMenu(USI);
            sprintf(
                sendBuf, 
                "SIGNUP/%s/%s/%s/%d/%f/%f/%f", 
                USI->id, USI->pw, USI->gender, USI->age,
                USI->height, USI->exerciseWeight, USI->goalWeight
            ); 

            sendRequest(sock, sendBuf);
        return STATE_MAIN_MENU;
        
        case 2:
            loginMenu(id, pw);
            sprintf(sendBuf, "LOGIN/%s/%s", id, pw);

            char response[1024];
            if (sendRequestWithResponse(sock, sendBuf, response)) {
                // 서버 응답에 \"성공\"이 포함되면
                if (strstr(response, "성공")) {
                    printf("[응답] %s\n", response);
                    // 로그인 성공 → 사용자 메뉴
                    strcpy(loggedInUserId, id);

                    return STATE_USER_MENU;
                }
                else {
                    printf("[응답] %s\n", response);
                    // 실패 → 다시 메인 메뉴
                    return STATE_MAIN_MENU;
                }
            }
            printf("로그인 중 오류 발생\n");
        return STATE_MAIN_MENU;

        case 3:
            printf("프로그램을 종료합니다.\n");
        return STATE_EXIT;
        
        default:
            sprintf(sendBuf, "MENU/%d", meunNumber);
            sendRequest(sock, sendBuf);
        return STATE_MAIN_MENU;
    }
    USIfree(USI);
}

MenuState handleUserMenu(int sock) {
    char id[50];
    strcpy(id, loggedInUserId);
    int choice = userMenu(id);
    char sendBuf[256];
    
    MealInputInfo *MII = MIImalloc();
    WorkOutInputInfo *WOII = WOIImalloc();
    WeightInputInfo *WII = WIImalloc();

    switch (choice) {
        case 1: 
            mealMenu(MII);
            sprintf(sendBuf, "INPUT_MEAL/%s/%s/%s/%f", id, MII->dateTime, MII->foodName, MII->gram);
        break;
        
        case 2: 
            workOutMenu(WOII);
            sprintf(sendBuf, "INPUT_WORKOUT/%s/%s/%s/%f", id, WOII->dateTime, WOII->exerciseName, WOII->minutes);
            
            char response[1024];
            if (strstr(response, "실패")) {
                // 검사 실패 → workOutMenu
                printf("%s 는/은 제공된 운동 파일에 입력 운동은 없습니다. 다시 입력 해주세요..\n", WOII->exerciseName);
                workOutMenu(WOII);
            }
        break;
        
        case 3: 
            weightMenu(WII);
            sprintf(sendBuf, "INPUT_WEIGHT/%s/%s/%f", id, WII->date, WII->weight);
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
            sprintf(sendBuf, "LOGOUT/%s", id); 
        return STATE_MAIN_MENU;

        case 8: 
            deleteIdMenu(id);
            sprintf(sendBuf, "DELETE_ID/%s", id); 
        break;
        // return STATE_MAIN_MENU;
        
        default:
            printf("잘못된 선택입니다.\n");
        break;
    }

    sendRequest(sock, sendBuf);

    MIIfree(MII);
    WOIIfree(WOII);
    WIIfree(WII);

    return STATE_MAIN_MENU;
}