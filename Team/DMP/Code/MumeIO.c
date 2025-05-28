#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "MumeIO.h"
#include "DBM.h"
#include "MenuClient.h"

/* ================== 메뉴 입출력 모듈 ================== */

/* ----------------- Output 관련 함수 ----------------- */

void runMenu() {
    // if (DBO("Schedule")) return;

    MenuState current = MM;
    while (current != EXIT) {
        switch (current) {
            case MM:        current = mainMenu();       break;
            case SIGNUP:    current = signupMenu();     break;
            case LOGIN:     current = LoginMenu();      break;
            default:        current = EXIT;             break;
        }
    }

    // DBC();
}

MenuState mainMenu() {
    int meunNumber = 0;

    P_MENU_TITLE("다이어트 관리 프로그램");
    P_MENU_SB_S("선택", "---");

    printf("1. 회원가입\n");
    printf("2. 로그인\n");
    printf("3. 프로그램 종료\n");

    P_MENU_IN;
    
    while (1) {
        printf("메뉴 선택 : ");
        scanf("%d", &meunNumber);
        getchar();

        switch (meunNumber) {
            case 1: return SIGNUP;
            case 2: return LOGIN;
            case 3: return EXIT;
            default : printf("%d 은 메뉴 번호가 아닙니다.\n", meunNumber); break;
        }
    
        P_MENU_IN;
        P_MENU_END;
    }
}

MenuState signupMenu() {
    SOCKET sock;
    char id[50], pw[50], gender[3];
    int age;
    float height, current_weight, goal_weight;

    P_MENU_TITLE("회원가입");
    P_MENU_IN;

    printf("ID 입력: "); 
    fgets(id, sizeof(id), stdin);
    id[strcspn(id, "\n")] = 0;
    
    printf("PW 입력: "); 
    fgets(pw, sizeof(pw), stdin);
    pw[strcspn(pw, "\n")] = 0;

    /* printf("나이: ");
    scanf("%d", &age);
    getchar();

    printf("성별 ");
    fgets(gender, sizeof(gender), stdin);
    gender[strcspn(gender, "\n")] = 0;

    printf("키 : ");
    scanf("%f", &height);

    printf("현재 체중 : ");
    scanf("%f", &current_weight);
    
    printf("목표 체중 : ");
    scanf("%f", &goal_weight); */

    // sprintf(sendBuf, "SIGNUP %s %s %d %s %f %f %f", id, pw, age, gender, height, current_weight, goal_weight);
    sprintf(sendBuf, "SIGNUP %s %s", id, pw);
    printf("SIGNUP sendBuf : %s\n", sendBuf);
    sendRequest(sock, sendBuf);

    return MM;
}

MenuState LoginMenu() {
    SOCKET sock;
    char id[50], pw[50];
    
    P_MENU_TITLE("로그인");
    P_MENU_IN;

    printf("ID 입력: "); 
    fgets(id, sizeof(id), stdin);
    id[strcspn(id, "\n")] = 0;
    
    printf("PW 입력: "); 
    fgets(pw, sizeof(pw), stdin);
    pw[strcspn(pw, "\n")] = 0;

    P_MENU_IN;
    
    sprintf(sendBuf, "LOGIN %s %s", id, pw);
    printf("LOGIN sendBuf : %s\n", sendBuf);
    sendRequest(sock, sendBuf);

    return MM;
}