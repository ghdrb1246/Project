#define _GNU_SOURCE  // strtok_r 사용
#include <stdio.h>
#include <string.h>
#include "MenuIO.h"
#include "InputInfo.h"
/* OUT */

/* 
================== [ 다이어트 관리 프로그램 ] ==================
-----------------------------------------------------------
[ 선택 --------------------------------------------------- ]
1. 회원가입
2. 로그인
3. 프로그램 종료
D : Input Error
-----------------------------------------------------------
메뉴 선택 : 
-----------------------------------------------------------
===========================================================
*/

int mainMenu() {
    int meunNumber = 0;

    P_MENU_TITLE("다이어트 관리 프로그램");
    P_MENU_IN;
    P_MENU_SB_S("선택", "----");
    
    printf("1. 회원가입\n");
    printf("2. 로그인\n");
    printf("3. 프로그램 종료\n");

    P_MENU_IN;

    meunNumber = inputMeunNum("메뉴 선택 : ");
    
    P_MENU_IN;
    
    P_MENU_END;

    return meunNumber;
}

/* 
================== [      회원 가입      ] ==================
-----------------------------------------------------------
ID : _
비밀번호 : _
성별 (남/여) : _
나이 : _ 
키 (cm) : 
초기 체중 (kg) : _
목표 체중 (kg) : _
-----------------------------------------------------------
===========================================================
*/

void signupMenu(UserSignupInfo *USI) {
    P_MENU_TITLE("회원가입");
    P_MENU_IN;
    P_MENU_SB_S("입력", "----");
    
    inputLine("ID 입력: ", USI->id, ID_SIZE);
    inputLine("PW 입력: ", USI->pw, PW_SIZE);
    inputLine("성별 (남/여): ", USI->gender, GENDER_SIZE);
    
    printf("나이: ");
    scanf("%d", &USI->age); getchar();

    printf("키 (cm): ");
    scanf("%f", &USI->height); getchar();
    
    printf("현재 체중 (kg): ");
    scanf("%f", &USI->exerciseWeight); getchar();
    
    printf("목표 체중 (kg): ");
    scanf("%f", &USI->goalWeight); getchar();

    P_MENU_IN;
    P_MENU_END;
}

/* 
================== [       로그인       ] ==================
-----------------------------------------------------------
ID : _
비밀번호 : _
-----------------------------------------------------------
로그인 하시겠습니까?[Y/N] : _
-----------------------------------------------------------
===========================================================
*/

void loginMenu(char *userId, char *pw) {
    P_MENU_TITLE("로그인");
    P_MENU_IN;
    P_MENU_SB_S("입력", "----");
    
    inputLine("ID 입력 : ", userId, 50);
    inputLine("PW 입력 : ", pw, 50);

    P_MENU_IN;
    P_MENU_END;
}

/* 
================== [     사용자 메뉴     ] ==================
(사용자 ID) 님 안녕하세요
-----------------------------------------------------------
[ 선택 --------------------------------------------------- ]
1. 식단 입력
2. 운동 입력
3. 체중 입력
4. 날짜별 기록 조회
5. 감량 진행률 조회
6. 피드백 보기
7. 로그아웃
8. 회원탈퇴
D : Input Error
-----------------------------------------------------------
메뉴 선택 : _
-----------------------------------------------------------
===========================================================
*/

int userMenu(char *userId) {
    int meunNumber = 0;

    P_MENU_TITLE("사용자 메뉴");
    printf("%s 님 안녕하세요\n", userId);

    P_MENU_IN;
    P_MENU_SB_S("선택", "---");
    
    printf("1. 식단 입력\n");
    printf("2. 운동 입력\n");
    printf("3. 체중 입력\n");
    printf("4. 날짜별 기록 조회\n");
    printf("5. 감량 진행률 조회\n");
    printf("6. 피드백 보기\n");
    printf("7. 로그아웃\n");
    printf("8. 회원탈퇴\n");
    // printf("D : Input Error\n");
    
    P_MENU_IN;
    
    meunNumber = inputMeunNum("메뉴 선택 : ");
    
    P_MENU_IN;
    P_MENU_END;

    return meunNumber;
}

/* 
================== [      식단 입력      ] ==================
-----------------------------------------------------------
날짜/시간(YYYY-MM-DD HH:MM) : _
음식명 : _
섭취량(g) : _
-----------------------------------------------------------
===========================================================
*/

void mealMenu(MealInputInfo *MII) {
    P_MENU_TITLE("식단 입력");
    P_MENU_IN;
    
    inputDateTime(MII->dateTime);

    // 음식명 입력
    inputLine("음식명: ", MII->foodName, FOODNAME_SIZE);

    // 섭취량 입력
    printf("섭취량(g): ");
    scanf("%f", &MII->gram);
    getchar();  // 개행 제거

    P_MENU_IN;
    P_MENU_END;
}

/* 
================== [      운동 입력      ] ==================
-----------------------------------------------------------
날짜/시간(YYYY-MM-DD HH:MM) : _
운동명 : _
시간(H) : _
-----------------------------------------------------------
운동을 저장 하시겠습니까?[Y/N] : _
-----------------------------------------------------------
===========================================================
*/

void workOutMenu(WorkOutInputInfo *WOII) {
    P_MENU_TITLE("운동 입력");
    P_MENU_IN;

    inputDateTime(WOII->dateTime);

    // 운동명 입력
    inputLine("운동명: ", WOII->exerciseName, EXERCISENAME_SIZE);
    //입력한 운동명이 DB에 있는지 여부

    // 시간(H) 입력
    printf("시간 (m) : ");
    scanf("%f", &WOII->minutes);
    getchar();  // 개행 제거

    P_MENU_IN;
    P_MENU_END;
}

/* 
================== [      체중 입력      ] ==================
-----------------------------------------------------------
날짜/시간(YYYY-MM-DD HH:MM) : _
체중 : _
-----------------------------------------------------------
체중을 저장 하시겠습니까?[Y/N] : _
-----------------------------------------------------------
===========================================================
*/

void weightMenu(WeightInputInfo *WII) {
    P_MENU_TITLE("체중 입력");
    P_MENU_IN;

    while (1) {
        printf("날짜 (YYYY-MM-DD): ");
        scanf("%10s", WII->date);
    
        if (validDateTime(WII->date)) break;
        else printf("잘 못된 형식입니다.\n");
    }

    // 체중 입력
    printf("체중(kg): ");
    scanf("%f", &WII->weight);
    getchar();  // 개행 제거

    P_MENU_IN;
    P_MENU_END;
}

/* 
================== [   날짜별 기록 조회    ] ==================
-----------------------------------------------------------
[ 기록 ---------------------------------------------------- ]
1. 2025-05-20
2. 2025-05-21
3. 2025-05-23
n. ...
-1 : 기록이 존재 하지 않습니다.
-----------------------------------------------------------
[ 선택 ---------------------------------------------------- ]
확인할 날짜 선택 : _
D : 입력한 날짜는 존재하지 않습니다.
-----------------------------------------------------------
해당 (선택한 날짜)의 기록
식단 | 운동 | 체중
-----------------------------------------------------------
뒤로 가기[Y/N] : 
-----------------------------------------------------------
Y : 뒤로 갑니다.
N : 취소.
D : Input Error
-----------------------------------------------------------
===========================================================
*/

void viewRecordsByDate_IN_Menu(char *date) {
    P_MENU_TITLE("날짜별 기록 조회");
    P_MENU_IN;

    while (1) {
        printf("날짜 (YYYY-MM-DD): ");
        scanf("%10s", date);
    
        if (validDateTime(date)) break;
        else printf("잘 못된 형식입니다.\n");
    }
}

void viewRecordsByDate_OUT_Menu(char *rds ,char *date) {
    char *saveptr1, str[2] = "";
    char *section = strtok_r(rds, "#", &saveptr1);

    P_MENU_IN;
    
    printf("%s 의 기록\n", date);

    P_MENU_IN;

    // printf("Menu : %s\n", rds);
    
    while (section) {
        if (strncmp(section, "MEAL:", 5) == 0) {
            printf("식단 ----\n");
            char *data = section + 5;
            char *saveptr2;
            char *entry = strtok_r(data, "|", &saveptr2);
            int mealCount = 1;

            while (entry) {
                printf("%d. %s\n", mealCount++, entry);
                entry = strtok_r(NULL, "|", &saveptr2);
            }
            printf("-------------------------------\n");
        }

        else if (strncmp(section, "WORKOUT:", 8) == 0) {
            printf("운동 ----\n");
            char *data = section + 8;
            char *saveptr2;
            char *entry = strtok_r(data, "|", &saveptr2);
            int workoutCount = 1;
            
            while (entry) {
                printf("%d. %s\n", workoutCount++, entry);
                entry = strtok_r(NULL, "|", &saveptr2);
            }
            printf("-------------------------------\n");
        }

        else if (strncmp(section, "WEIGHT:", 7) == 0) {
            printf("체중 ----\n%s\n", section + 7);
            printf("-------------------------------\n");
        }
        section = strtok_r(NULL, "#", &saveptr1);
    }
    
    P_MENU_IN;

    while (1) {
        printf("확인[Y/N] : ");
        scanf("%s", str);
        P_MENU_IN;

        if (strcmp(str, "Y") == 0 || strcmp(str, "y") == 0) {
            printf("\n뒤로 갑니다.\n\n");
            break;
        }
        else if (strcmp(str, "N") == 0 || strcmp(str, "n") == 0) { 
            printf("\n취소.\n\n");
        }
        else printf("%s은 잘못 된 입력입니다.\n", str);
    }

    P_MENU_IN;
    P_MENU_END;
}

/* 
================== [   감량 진행률 조회    ] ==================
-----------------------------------------------------------
초기 / 목표 / 현재 체중 + 달성률(%) 출력
-----------------------------------------------------------
뒤로 가기[Y/N] : 
-----------------------------------------------------------
Y : 뒤로 갑니다.
N : 취소.
D : Input Error
-----------------------------------------------------------
===========================================================
*/

void checkWeightLossProgressMenu(char *cwlpstr, char *userId) {
    char *saveptr1, str[2] = "";
    // char *section = strtok_r(cwlpstr, "#", &saveptr1);
    
    P_MENU_TITLE("감량 진행률 조회");
    P_MENU_IN;

    printf("%s 님의 진행률\n", userId);

    P_MENU_IN;
    // "progress:%.1f|initialWeight:%.1fkg|goalWeight:%.1fkg|currentWeight:%.1fkg",
//    "[진행률] %.1f%%\n초기 체중: %.1fkg, 목표 체중: %.1fkg, 현재 체중: %.1fkg\n",
        /* 
        if (strncmp(cwlpstr, "progress:", 9) == 0) {

            char *data = cwlpstr + 9;
            char *saveptr2;
            char *entry = strtok_r(data, "|", &saveptr2);
            printf("진행률 : %s\n", entry);
        }

        else if (strncmp(cwlpstr, "initialWeight:", 14) == 0) {
            char *data = cwlpstr + 14;
            char *saveptr2;
            char *entry = strtok_r(data, "|", &saveptr2);
            printf("초기 체중 : %s\n", entry);
        }

        else if (strncmp(cwlpstr, "goalWeight:", 11) == 0) {
            char *data = cwlpstr + 11;
            char *saveptr2;
            char *entry = strtok_r(data, "|", &saveptr2);
            printf("목표 체중 : %s\n", entry);
        }

        else if (strncmp(cwlpstr, "currentWeight:", 14) == 0) {
            printf("현재 체중 : %s\n", cwlpstr + 14);
        }
     */
   char *saveptr;
    char *token = strtok_r(cwlpstr, "|", &saveptr);
    while (token) {
        if (strncmp(token, "progress:", 9) == 0) {
            float progress;
            sscanf(token + 9, "%f", &progress);
            printf("진행률 : %.1f%%\n", progress);
        } 
        else if (strncmp(token, "initialWeight:", 14) == 0) {
            float initial;
            sscanf(token + 14, "%f", &initial);
            printf("초기 체중 : %.1fkg\n", initial);
        } 
        else if (strncmp(token, "goalWeight:", 11) == 0) {
            float goal;
            sscanf(token + 11, "%f", &goal);
            printf("목표 체중 : %.1fkg\n", goal);
        } 
        else if (strncmp(token, "currentWeight:", 14) == 0) {
            float current;
            sscanf(token + 14, "%f", &current);
            printf("현재 체중 : %.1fkg\n", current);
        }
        token = strtok_r(NULL, "|", &saveptr);
    }
    P_MENU_IN;

    while (1) {
        printf("확인[Y/N] : ");
        scanf("%s", str);
        P_MENU_IN;

        if (strcmp(str, "Y") == 0 || strcmp(str, "y") == 0) {
            printf("\n뒤로 갑니다.\n\n");
            break;
        }
        else if (strcmp(str, "N") == 0 || strcmp(str, "n") == 0) { 
            printf("\n취소.\n\n");
        }
        else printf("%s은 잘못 된 입력입니다.\n", str);
    }

    P_MENU_IN;
    P_MENU_END;
}

/* 
================== [       피드백       ] ==================
-----------------------------------------------------------
체중 증가 또는 칼로리 초과시 추천 출력
-----------------------------------------------------------
뒤로 가기[Y/N] : 
-----------------------------------------------------------
===========================================================
*/

void feedBackMenu(const float weight, const float kcal) {
    P_MENU_TITLE("피드백");
    P_MENU_IN;
    
    printf("체중 증가 또는 칼로리 초과시 추천 출력\n");
    printf("%f | %f\n", weight, kcal);
    printf("음식 API\n");

    P_MENU_IN;

    // 뒤로 가기 [y/n]
    
    P_MENU_IN;
    P_MENU_END;
}

/* 
=================== [     로그 아웃     ] ===================
-----------------------------------------------------------
(사용자 ID) 이/가 로그아웃 되었습니다.
-----------------------------------------------------------
===========================================================
*/

void logOutMenu(const char *userId) {
    P_MENU_TITLE("로그 아웃");
    P_MENU_IN;

    printf("%s 이/가 로그아웃 되었습니다.\n", userId);

    P_MENU_IN;
    P_MENU_END;
}

/* 
=================== [     회원 탈퇴     ] ===================
-----------------------------------------------------------
(사용자 ID) 이/가 회원 탈퇴 처리 되었습니다.
-----------------------------------------------------------
===========================================================
*/

void deleteIdMenu(const char *userId) {
    P_MENU_TITLE("회원 탈퇴");
    P_MENU_IN;

    printf("%s 이/가 회원 탈퇴 처리 되었습니다.\n", userId);
    // printf("DB에서 해당 id 삭제\n");

    P_MENU_IN;
    P_MENU_END;
}

/* In */

void inputLine(const char *prompt, char *buf, int size) {
    printf("%s", prompt);
    /* if (fgets(buf, size, stdin) != NULL) {
        buf[strcspn(buf, "\n")] = '\0';  // 개행 문자 제거
    } */

    fgets(buf, size, stdin);
    buf[strcspn(buf, "\n")] = '\0'; 
}

int inputMeunNum(const char *prompt) {
    int meunNumber;

    printf("%s", prompt);
    scanf("%d", &meunNumber);
    getchar(); // 남은 개행 제거

    return meunNumber;
}

void inputDateTime(char *dateTime) {
    char date[11];
    char time[6];

    while (1) {
        printf("날짜 (YYYY-MM-DD): ");
        scanf("%10s", date);
    
        printf("시간 (HH:MM): ");
        scanf("%5s", time);
    
        // 날짜/시간 합치기
        snprintf(dateTime, DATETIME_SIZE, "%s %s", date, time);
    
        if (validDateTime(dateTime)) break;
        else printf("잘 못된 형식입니다.\n");
    }

    getchar();  // scanf 개행 제거
}

int validDateTime(char *datetime) {
    int y, m, d, h, min;
    int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    
    if (strlen(datetime) >= 16) {
        // printf("YYYY-MM-DD HH:MM\n");
        // "YYYY-MM-DD HH:MM" → 총 16자 (문자열 + 널 포함 17바이트)
        if (strlen(datetime) != 16 || datetime[4] != '-' || datetime[7] != '-' || datetime[10] != ' ' || datetime[13] != ':') {
            return 0;
        }
        
        // 날짜와 시간 추출
        else if (sscanf(datetime, "%4d-%2d-%2d %2d:%2d", &y, &m, &d, &h, &min) != 5) {
            return 0;
        }
        
        // 날짜 범위 검사
        if ((y < 1900 || y > 2100) || (m < 1 || m > 12) || (d < 1 || d > 31)) return 0;
        
        // 월별 최대 일자 확인 (윤년 고려)
        if ((y % 4 == 0 && y % 100 != 0) || y % 400 == 0) {
            daysInMonth[1] = 29;  // 윤년
        }
        if (d > daysInMonth[m - 1]) return 0;
        
        // 시간 범위 검사
        if ((h < 0 || h > 23) || (min < 0 || min > 59)) return 0;
    }
    else {
        // printf("YYYY-MM-DD\n");
         // "YYYY-MM-DD" → 총 10자 (문자열 + 널 포함 11바이트)
        if (strlen(datetime) != 10 || datetime[4] != '-' || datetime[7] != '-') {
            return 0;
        }
        
        // 날짜와 시간 추출
        else if (sscanf(datetime, "%4d-%2d-%2d", &y, &m, &d) != 3) {
            return 0;
        }
        
        // 날짜 범위 검사
        if ((y < 1900 || y > 2100) || (m < 1 || m > 12) || (d < 1 || d > 31)) return 0;
        
        // 월별 최대 일자 확인 (윤년 고려)
        if ((y % 4 == 0 && y % 100 != 0) || y % 400 == 0) {
            daysInMonth[1] = 29;  // 윤년
        }
        if (d > daysInMonth[m - 1]) return 0;
    }

    return 1; // 유효한 날짜+시간
}