#include "MainServer.h"

int main() {
    #ifdef _WIN32
        WSADATA wsa;
        WSAStartup(MAKEWORD(2, 2), &wsa);
    #endif

    SOCKET serverSock, clientSock;
    struct sockaddr_in server, client;
    
    socklen_t clientSize = sizeof(client);

    serverSock = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    bind(serverSock, (struct sockaddr*)&server, sizeof(server));
    listen(serverSock, 5);

    printf("서버가 포트 %d에서 대기 중입니다...\n", PORT);
    
    DBO("Users");

    while (1) {
        clientSock = accept(serverSock, (struct sockaddr*)&client, &clientSize);
        printf("클라이언트 접속됨\n");
        handleClient(clientSock);
        CLOSESOCKET(clientSock);
    }

    // sqlite3_close(db);
    DBC();
    
    CLOSESOCKET(serverSock);

    #ifdef _WIN32
        WSACleanup();
    #endif 
        return 0;
}

void handleClient(SOCKET clientSock) {
    char buf[BUF_SIZE];
    char response[BUF_SIZE];
    int len;

    while ((len = recv(clientSock, buf, BUF_SIZE - 1, 0)) > 0) {
        buf[len] = '\0';
        printf("[요청] %s\n", buf);
        processRequest(clientSock, buf, response);
        send(clientSock, response, strlen(response), 0);
    }

    memset(response, 0, sizeof(response));
}

void processRequest(SOCKET clientSock, char *request, char *response) {
    char cmd[16];
    UserSignupInfo *USI = USImalloc();
    MealInputInfo *MII = MIImalloc();
    WorkOutInputInfo *WOII = WOIImalloc();
    WeightInputInfo *WII = WIImalloc();

    sscanf(request, "%[^/]", cmd);

   /*  if (!USI) {
        printf("메모리 할당 실패!\n");
        return;
    } */

    // 회원가입 처리
    if (strcmp(cmd, "SIGNUP") == 0) {
        sscanf(
            request, 
            "SIGNUP/%[^/]/%[^/]/%[^/]/%d/%f/%f/%f",
            USI->id, USI->pw, USI->gender, &USI->age,
           &USI->height, &USI->exerciseWeight, &USI->goalWeight
        );

        printf("-> %s %s %s %d %f %f %f\n", USI->id, USI->pw, USI->gender, USI->age, USI->height, USI->exerciseWeight, USI->goalWeight);

        if (userExists(USI->id)) {
            sprintf(response, "[실패] 이미 존재하는 ID입니다->");
        } 
        else {
            signupUser(USI);
            sprintf(response, "[성공] 회원가입 완료");
        }

        USIfree(USI);
    } 

    // 로그인 처리
    else if (strcmp(cmd, "LOGIN") == 0) {
        sscanf(
            request, 
            "LOGIN/%[^/]/%s", 
            USI->id, USI->pw
        );
        
        if (loginUser(USI->id, USI->pw)) {
            sprintf(response, "[성공] 로그인 성공");
        } 
        else {
            sprintf(response, "[실패] 로그인 실패");
        }
        USIfree(USI);
    } 

    /* 입력 함수 -> */

    // 식단 입력
    else if (strcmp(cmd, "INPUT_MEAL") == 0) {
        sscanf(
            request, 
            "INPUT_MEAL/%[^/]/%[^/]/%[^/]/%f", 
            MII->userId, MII->dateTime, MII->foodName, &MII->gram
        );

        printf("식단 입력 치리 -> %s %s %s %.1f\n", MII->userId, MII->dateTime, MII->foodName, MII->gram);
        
        if (needConvert(FOOD_CSV_FILE, FOOD_DB_FILE)) {
            foodConvertCSVtoDB();
        } 
        else {
            printf("최신화 불필요, 기존 DB 사용!\n");
        }

        MII->kcal = inputFoodAndCalc(MII->foodName);
        printf("%.1f\n", MII->kcal);
        if (MII->kcal != -1) {
            sprintf(response, "[성공] 음식 검사 성공");
            insertMeal(MII);
            // sprintf(response, "[실패] 체중 조회 실패");
        }
        else {
            sprintf(response, "[실패] 검사 실패");
        }
        
        MIIfree(MII);
    } 
    // 운동 입력
    else if (strcmp(cmd, "INPUT_WORKOUT") == 0) {
        sscanf(
            request,
            "INPUT_WORKOUT/%[^/]/%[^/]/%[^/]/%f",
            WOII->userId, WOII->dateTime, WOII->exerciseName, &WOII->minutes
        );

        printf("운동 입력 치리 -> %s %s %s %.1f\n", WOII->userId ,WOII->dateTime, WOII->exerciseName, WOII->minutes);

        if (needConvert(EXERCISES_CSV_FILE, EXERCISES_DB_FILE)) {
            exercisesConvertCSVtoDB();
        } 
        else {
            printf("최신화 불필요, 기존 DB 사용!\n");
        }

        float met = inputWorkoutAndCalc(WOII->exerciseName);

        if (met != -1) {
            sprintf(response, "[성공] 운동 검사 성공");
            WOII->kcal = METM(met, WOII->minutes, selectWeight(WOII->userId));
            insertWorkout(WOII);
            // sprintf(response, "[실패] 체중 조회 실패");
        }
        else {
            sprintf(response, "[실패] 검사 실패");
        }
        WOIIfree(WOII);
    } 
    // 체중 입력
    else if (strcmp(cmd, "INPUT_WEIGHT") == 0) {
        sscanf(
            request,
            "INPUT_WEIGHT/%[^/]/%[^/]/%f",
            WII->userId, WII->date, &WII->weight
        );

        printf("체중 입력 치리 -> %s %s %.1f\n", WII->userId, WII->date, WII->weight);
        insertWeight(WII);

        WIIfree(WII);
    } 
    
    /* 조회 함수 -> */

    // 날짜별 기록 조회
    else if (strcmp(cmd, "GET_RECORD") == 0) {
        char id[ID_SIZE], date[11], *rds;
        
        printf("날짜별 기록 조회\n");

        sscanf(request, "GET_RECORD/%[^/]/%s", id, date);

        rds = viewRecordsByDate(id, date);
        send(clientSock, rds, strlen(rds), 0);
        // printf("MS : %s\n", rds);
    } 

    // 피드백 추천
    else if (strcmp(cmd, "FEEDBACK") == 0) {
        char id[ID_SIZE], date[11], *fbs;
        printf("피드백 추천\n");
        
        sscanf(request, "GET_RECORD/%[^/]/%s", id, date);

        fbs = viewRecordsByDate(id, date);
        send(clientSock, fbs, strlen(fbs), 0);
        // printf("MS : %s\n", rds);
    } 
    
    /* 처리 함수 -> */

    // 감량 진행률 계산
    else if (strcmp(cmd, "CHECK_PROGRESS") == 0) {
        char id[ID_SIZE], *cwlpstr;
        
        printf("감량 진행률 계산\n");

        sscanf(request, "CHECK_PROGRESS/%s", id);

        cwlpstr = checkWeightLossProgress(id);
        send(clientSock, cwlpstr, strlen(cwlpstr), 0);
        printf("MS : %s\n", cwlpstr);
        
    } 
    // 로그아웃 처리
    else if (strcmp(cmd, "LOGOUT") == 0) {
        char id[ID_SIZE];
        sscanf(request, "LOGOUT/%s", id);
        printf("%s 님이 로그아웃 처리 되었습니다.\n", id);
    } 
    // 회원 탈퇴 처리
    else if (strcmp(cmd, "DELETE_ID") == 0) {
        char id[ID_SIZE];
        // printf("회원 탈퇴 처리 해당 사용자 DB 삭제\n");
        sscanf(request, "DELETE_ID/%s", id);

        printf("회원 탈퇴 처리 -> %s\n", id);
        deleteUserData(id);
    }
    
    else {
        sprintf(response, "[오류] 알 수 없는 명령어");
    }
}