#include "ClientController.h"

void sendRequest(int sock, const char *message) {
    char recvBuf[BUF_SIZE];
    send(sock, message, strlen(message), 0);
    int len = recv(sock, recvBuf, BUF_SIZE - 1, 0);
    if (len > 0) {
        recvBuf[len] = '\0';
        printf("\n\n[응답] %s\n\n", recvBuf);
    }
    memset(recvBuf, 0, sizeof(recvBuf));
}

int sendRequestWithResponse(int sock, const char *message, char *response) {
    char recvBuf[BUF_SIZE];
    
    send(sock, message, strlen(message), 0);
    
    int len = recv(sock, recvBuf, BUF_SIZE - 1, 0);
    if (len > 0) {
        recvBuf[len] = '\0';
        strcpy(response, recvBuf);
        return 1;
    }
    memset(recvBuf, 0, sizeof(recvBuf));

    return 0;
}

MenuState handleMainMenu(int sock) {
    int meunNumber = mainMenu();
    char sendBuf[BUF_SIZE];
    char id[ID_SIZE], pw[PW_SIZE];
    char response[BUF_SIZE];

    memset(id, 0, sizeof(id));
    memset(pw, 0, sizeof(pw));
    memset(sendBuf, 0, sizeof(sendBuf));
    memset(response, 0, sizeof(response));

    UserSignupInfo *USI = USImalloc();
    
    switch (meunNumber) {
        case 1:
            signupMenu(USI);
            memset(sendBuf, 0, sizeof(sendBuf));
            sprintf(
                sendBuf, 
                "SIGNUP/%s/%s/%s/%d/%f/%f/%f", 
                USI->id, USI->pw, USI->gender, USI->age,
                USI->height, USI->exerciseWeight, USI->goalWeight
            ); 

            sendRequest(sock, sendBuf);
            // memset(sendBuf, 0, sizeof(sendBuf));
        return STATE_MAIN_MENU;
        
        case 2:
            loginMenu(id, pw);
            memset(sendBuf, 0, sizeof(sendBuf));
            sprintf(sendBuf, "LOGIN/%s/%s", id, pw);

            if (sendRequestWithResponse(sock, sendBuf, response)) {
                // 서버 응답에 \"성공\"이 포함되면
                if (strstr(response, "성공")) {
                    printf("[응답] %s\n", response);
                    // 로그인 성공 → 사용자 메뉴
                    strcpy(loggedInUserId, id);

                    // memset(sendBuf, 0, sizeof(sendBuf));
                    memset(response, 0, sizeof(response));
                    return STATE_USER_MENU;
                }
                else {
                    printf("[응답] %s\n", response);
                    // 실패 → 다시 메인 메뉴
                    // memset(sendBuf, 0, sizeof(sendBuf));
                    memset(response, 0, sizeof(response));
                    return STATE_MAIN_MENU;
                }
            }
            printf("로그인 중 오류 발생\n");
        return STATE_MAIN_MENU;

        case 3:
            memset(sendBuf, 0, sizeof(sendBuf));
            printf("프로그램을 종료합니다.\n");
        break;
        
        default:
            memset(sendBuf, 0, sizeof(sendBuf));
            sprintf(sendBuf, "MENU/%d", meunNumber);
            sendRequest(sock, sendBuf);

            memset(sendBuf, 0, sizeof(sendBuf));
        return STATE_MAIN_MENU;
    }
    
    USIfree(USI);
    
    return STATE_EXIT;
}

MenuState handleUserMenu(int sock) {
    char id[ID_SIZE];
    char sendBuf[BUF_SIZE], recvBuf[BUF_SIZE], response[BUF_SIZE];
    char date[11] = "";
    
    memset(id, 0, sizeof(id));
    memset(sendBuf, 0, sizeof(sendBuf));
    memset(recvBuf, 0, sizeof(recvBuf));
    memset(response, 0, sizeof(response));

    strcpy(id, loggedInUserId);

    int choice = userMenu(id);
    int len;

    MealInputInfo *MII = MIImalloc();
    WorkOutInputInfo *WOII = WOIImalloc();
    WeightInputInfo *WII = WIImalloc();

    switch (choice) {
        case 1: 
            while(1) {
                mealMenu(MII);
                memset(sendBuf, 0, sizeof(sendBuf));
                sprintf(sendBuf, "INPUT_MEAL/%s/%s/%s/%f", id, MII->dateTime, MII->foodName, MII->gram);
                
                if (sendRequestWithResponse(sock, sendBuf, response)) {
                    // 서버 응답에 \"성공\"이 포함되면
                    if (strstr(response, "성공")) {
                        printf("[응답] %s\n", response);
                        break;
                    }
                    else {
                        printf("[응답] %s\n", response);
                        printf("%s 는/은 제공된 음식 파일에 입력 운동은 없습니다. 다시 입력 해주세요..\n", MII->foodName);
                    }
                }
            }
            // memset(sendBuf, 0, sizeof(sendBuf));
        break;
        
        case 2: 
            while(1) {
                workOutMenu(WOII);
                memset(sendBuf, 0, sizeof(sendBuf));
                sprintf(sendBuf, "INPUT_WORKOUT/%s/%s/%s/%f", id, WOII->dateTime, WOII->exerciseName, WOII->minutes);
                
                if (sendRequestWithResponse(sock, sendBuf, response)) {
                    // 서버 응답에 \"성공\"이 포함되면
                    if (strstr(response, "성공")) {
                        printf("[응답] %s\n", response);
                        break;
                    }
                    else {
                        printf("[응답] %s\n", response);
                        printf("%s 는/은 제공된 운동 파일에 입력 운동은 없습니다. 다시 입력 해주세요..\n", WOII->exerciseName);
                    }
                }
            }

            // memset(sendBuf, 0, sizeof(sendBuf));
        break;
        
        case 3: 
            weightMenu(WII);
            memset(sendBuf, 0, sizeof(sendBuf));
            sprintf(sendBuf, "INPUT_WEIGHT/%s/%s/%f", id, WII->date, WII->weight);

            if (sendRequestWithResponse(sock, sendBuf, response)) {
                // 서버 응답에 \"성공\"이 포함되면
                if (strstr(response, "성공")) {
                    printf("[응답] %s\n", response);
                }
                else {
                    printf("[응답] %s\n", response);
                    printf("%s 체중이 아닙니다.\n", WOII->exerciseName);
                }
            }
        break;
        
        case 4:
            viewRecordsByDate_IN_Menu(date); 
            memset(sendBuf, 0, sizeof(sendBuf));
            snprintf(sendBuf, sizeof(sendBuf), "GET_RECORD/%s/%s", id, date);
            send(sock, sendBuf, strlen(sendBuf), 0);
            // sendRequest(sock, sendBuf);

            len = recv(sock, recvBuf, sizeof(recvBuf) - 1, 0);
            recvBuf[len] = '\0';

            // printf("2CC : %s\n", recvBuf);
            viewRecordsByDate_OUT_Menu(recvBuf, date);
            
            memset(recvBuf, 0, sizeof(recvBuf));
            memset(sendBuf, 0, sizeof(sendBuf));
        return STATE_USER_MENU;

        case 5: 
            memset(sendBuf, 0, sizeof(sendBuf));
            sprintf(sendBuf, "CHECK_PROGRESS/%s", id); 
            send(sock, sendBuf, strlen(sendBuf), 0);

            len = recv(sock, recvBuf, sizeof(recvBuf) - 1, 0);
            recvBuf[len] = '\0';

            checkWeightLossProgressMenu(recvBuf, id);
            
            memset(recvBuf, 0, sizeof(recvBuf));
            memset(sendBuf, 0, sizeof(sendBuf));
        return STATE_USER_MENU;

        case 6: 
            memset(sendBuf, 0, sizeof(sendBuf));
            sprintf(sendBuf, "FEEDBACK/%s", id); 
            send(sock, sendBuf, strlen(sendBuf), 0);
            
            len = recv(sock, recvBuf, sizeof(recvBuf) - 1, 0);
            recvBuf[len] = '\0';

            feedBackMenu(60, 1000);
            
            memset(recvBuf, 0, sizeof(recvBuf));
            memset(sendBuf, 0, sizeof(sendBuf));
        return STATE_USER_MENU;

        case 7: 
            logOutMenu(id);
            memset(sendBuf, 0, sizeof(sendBuf));
            sprintf(sendBuf, "LOGOUT/%s", id); 
            if (sendRequestWithResponse(sock, sendBuf, response)) {
                // 서버 응답에 \"성공\"이 포함되면
                if (strstr(response, "성공")) {
                    printf("[응답] %s\n", response);
                    strcpy(loggedInUserId, "");
                }
                else {
                    printf("[응답] %s\n", response);
                }
            }
        // break;
        return STATE_MAIN_MENU;

        case 8: 
            deleteIdMenu(id);
            memset(sendBuf, 0, sizeof(sendBuf));
            sprintf(sendBuf, "DELETE_ID/%s", id); 
            if (sendRequestWithResponse(sock, sendBuf, response)) {
                // 서버 응답에 \"성공\"이 포함되면
                if (strstr(response, "성공")) {
                    printf("[응답] %s\n", response);
                }
                else {
                    printf("[응답] %s\n", response);
                }
            }

        // break;
        return STATE_MAIN_MENU;
        
        default:
            memset(sendBuf, 0, sizeof(sendBuf));
            printf("잘못된 선택입니다.\n");
        break;
    }

    sendRequest(sock, sendBuf);
    memset(sendBuf, 0, sizeof(sendBuf));

    MIIfree(MII);
    WOIIfree(WOII);
    WIIfree(WII);
    
    return STATE_USER_MENU;
}