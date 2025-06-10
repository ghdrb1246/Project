#include "MenuIO.h"

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
    while (1) {
        P_MENU_TITLE(" 다이어트 관리 프로그램 ");
        P_MENU_IN;
        P_MENU_SB_S("선택", "----");
        
        printf("1. 회원가입\n");
        printf("2. 로그인\n");
        printf("3. 프로그램 종료\n");
    
        P_MENU_IN;
    
        meunNumber = inputMeunNum("메뉴 선택 : ");
        if (meunNumber > 3 || meunNumber < 1) {
            printf("%d 은 메뉴 번호가 아닙니다.\n", meunNumber);    
        }
        else break;
    }
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
    P_MENU_TITLE("      회원 가입      ");
    P_MENU_IN;
    P_MENU_SB_S("입력", "----");
    
    memset(USI->id, 0, ID_SIZE);
    inputLine("ID 입력: ", USI->id, ID_SIZE);

    memset(USI->pw, 0, PW_SIZE);
    inputLine("PW 입력: ", USI->pw, PW_SIZE);
    
    memset(USI->gender, 0, GENDER_SIZE);
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
    P_MENU_TITLE("        로 그 인        ");
    P_MENU_IN;
    P_MENU_SB_S("입력", "----");
    
    memset(userId, 0, ID_SIZE);
    inputLine("ID 입력 : ", userId, 50);

    memset(pw, 0, PW_SIZE);
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
    
    while (1) {
        P_MENU_TITLE("      사용자  메뉴      ");
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
        if (meunNumber > 8 || meunNumber < 1) {
            printf("%d 은 메뉴 번호가 아닙니다.\n", meunNumber);    
        }
        else break;
    }
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
    P_MENU_TITLE("       식단  입력       ");
    P_MENU_IN;
    
    memset(MII->dateTime, 0, DATETIME_SIZE);
    inputDateTime(MII->dateTime);

    // 음식명 입력
    memset(MII->foodName, 0, FOODNAME_SIZE);
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
    P_MENU_TITLE("       운동  입력       ");
    P_MENU_IN;

    memset(WOII->dateTime, 0, DATETIME_SIZE);
    inputDateTime(WOII->dateTime);

    // 운동명 입력
    inputLine("운동명: ", WOII->exerciseName, EXERCISENAME_SIZE);

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
    P_MENU_TITLE("       체중  입력       ");
    P_MENU_IN; 

    while (1) {
        memset(WII->date, 0, DATETIME_SIZE);
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
날짜 (YYYY-MM-DD): _
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
    P_MENU_TITLE("    날짜별 기록 조회    ");
    P_MENU_IN;

    while (1) {
        memset(date, 0, DATETIME_SIZE - 5);
        printf("날짜 (YYYY-MM-DD): ");
        scanf("%10s", date);
    
        if (validDateTime(date)) break;
        else printf("잘 못된 형식입니다.\n");
    }
}

void viewRecordsByDate_OUT_Menu(char *rds, char *date) {
    char str[2] = "";

    P_MENU_IN;
    printf("%s 의 기록\n", date);
    P_MENU_IN;

    
    // 3개의 문자열로 구간별 복사
    char mealData[BUF_SIZE / 2] = "";
    char workoutData[BUF_SIZE / 2] = "";
    char weightData[BUF_SIZE / 2] = "";

    // 첫 번째 레벨: '#'로 구분
    char *section = strtok(rds, "#");
    int sectionCount = 0;

    while (section != NULL) {
        if (sectionCount == 0) {
            strncpy(mealData, section, sizeof(mealData)-1);
        } else if (sectionCount == 1) {
            strncpy(workoutData, section, sizeof(workoutData)-1);
        } else if (sectionCount == 2) {
            strncpy(weightData, section, sizeof(weightData)-1);
        }
        sectionCount++;

        section = strtok(NULL, "#");
    }

    // -------------------------------
    // 식단 데이터 출력
    if (strlen(mealData) > 0) {
        P_MENU_SB_S("식단", "----");
        // ':' 이후 데이터만 분리
        char *dataPart = strchr(mealData, ':');
        if (dataPart) {
            dataPart++;  // ':' 넘기기
            char *entry = strtok(dataPart, "|");
            int mealCount = 1;
            while (entry) {
                printf("%d. %s\n", mealCount++, entry);
                entry = strtok(NULL, "|");
            }
        }
        P_MENU_IN;
    }

    // 운동 데이터 출력
    if (strlen(workoutData) > 0) {
        P_MENU_SB_S("운동", "----");

        char *dataPart = strchr(workoutData, ':');
        if (dataPart) {
            dataPart++;
            char *entry = strtok(dataPart, "|");
            int workoutCount = 1;
            while (entry) {
                printf("%d. %s\n", workoutCount++, entry);
                entry = strtok(NULL, "|");
            }
        }
        P_MENU_IN;
    }

    // 체중 데이터 출력
    if (strlen(weightData) > 0) {
        P_MENU_SB_S("체중", "----");

        char *dataPart = strchr(weightData, ':');
        if (dataPart) {
            dataPart++;
            printf("%s\n", dataPart);
        }
        P_MENU_IN;
    }

    while (1) {
        printf("확인[Y/N] : ");
        scanf("%s", str);
        // P_MENU_IN;

        if (strcmp(str, "Y") == 0 || strcmp(str, "y") == 0) {
            printf("\n뒤로 갑니다.\n\n");
            break;
        }
        else if (strcmp(str, "N") == 0 || strcmp(str, "n") == 0) {
            printf("\n취소.\n\n");
        }
        else {
            printf("%s 은 잘못 된 입력입니다.\n", str);
        }
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
    char str[2] = "";

    P_MENU_TITLE("    감량 진행률 조회    ");
    P_MENU_IN;

    printf("%s 님의 진행률\n", userId);

    P_MENU_IN;

    char dataCopy[BUF_SIZE];
    strncpy(dataCopy, cwlpstr, sizeof(dataCopy)-1);
    dataCopy[sizeof(dataCopy)-1] = '\0';

    char *token = strtok(dataCopy, "|");
    while (token != NULL) {
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

        token = strtok(NULL, "|");
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
        else printf("%s 은 잘못 된 입력입니다.\n", str);
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

void feedBackMenu(const float latestWeight, const float previousWeight) {
    const char *exerciseRecommendations[] = {
        "자전거", "근육트레이닝", "복싱", "런닝머신", "줄넘기", "수영", "요가 스트레칭"
    };
    const char *mealRecommendations[] = {
        "닭가슴살 샐러드", "그릭 요거트", "연어 포케", "두부 샐러드", "삶은 달걀", "계란 스크램블", "베이글(잼X)"
    };
    char str[2] = "";

    P_MENU_TITLE("        피 드 백         ");
    P_MENU_IN;
    
    if (latestWeight < 0 || previousWeight < 0) {
        printf("[오류] 입력된 몸무게 데이터가 부족합니다.\n");
    } 
    else if (latestWeight > previousWeight) {
        // 운동/음식 추천 배열
        int exerciseCount = sizeof(exerciseRecommendations) / sizeof(exerciseRecommendations[0]);
        int mealCount = sizeof(mealRecommendations) / sizeof(mealRecommendations[0]);

        int exerciseIndex = rand() % exerciseCount;
        int mealIndex = rand() % mealCount;

        printf("[피드백] 최근 체중이 증가했습니다. 식단과 운동을 다시 점검해보세요.\n");
        printf("추천 운동: %s\n", exerciseRecommendations[exerciseIndex]);
        printf("추천 음식: %s\n", mealRecommendations[mealIndex]);
    } 
    else {
        printf("[피드백] 다이어트 잘하고 있어요! 계속 힘내세요.\n");
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
=================== [     로그 아웃     ] ===================
-----------------------------------------------------------
(사용자 ID) 이/가 로그아웃 되었습니다.
-----------------------------------------------------------
===========================================================
*/

void logOutMenu(const char *userId) {
    P_MENU_TITLE("        로그 아웃        ");
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
    P_MENU_TITLE("        회원 탈퇴        ");
    P_MENU_IN; 

    printf("%s 이/가 회원 탈퇴 처리 되었습니다.\n", userId);

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