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
        printf("[ 요청 ] | %s\n", buf);
        processRequest(clientSock, buf, response);

        send(clientSock, response, strlen(response), 0);
        memset(response, 0, sizeof(response));
    }
}

void processRequest(SOCKET clientSock, char *request, char *response) {
    char cmd[16];
    UserSignupInfo *USI = USImalloc();
    MealInputInfo *MII = MIImalloc();
    WorkOutInputInfo *WOII = WOIImalloc();
    WeightInputInfo *WII = WIImalloc();

    memset(response, 0, sizeof(BUF_SIZE));

    if (sscanf(request, "%[^/]", cmd) != 1) {
        sprintf(response, "[오류] 잘못된 입력 형식");
        return;
    }

    // 회원가입 처리
    if (strcmp(cmd, "SIGNUP") == 0) {
        if (sscanf(request, "SIGNUP/%[^/]/%[^/]/%[^/]/%d/%f/%f/%f", USI->id, USI->pw, USI->gender, &USI->age, &USI->height, &USI->exerciseWeight, &USI->goalWeight) != 7) {
            sprintf(response, "[오류] 잘못된 입력 형식");
            return;
        }

        printf("[ 서버 ] | 회원가입 처리 -> %s %s %s %d %f %f %f\n", USI->id, USI->pw, USI->gender, USI->age, USI->height, USI->exerciseWeight, USI->goalWeight);

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
        if (sscanf(request, "LOGIN/%[^/]/%s", USI->id, USI->pw) != 2) {
            sprintf(response, "[오류] 잘못된 입력 형식");
            return;
        }
        
        if (loginUser(USI->id, USI->pw)) {
            sprintf(response, "[성공] 로그인 성공");
        } 
        else {
            sprintf(response, "[실패] 로그인 실패");
        }
        USIfree(USI);
    } 

    // 식단 입력
    else if (strcmp(cmd, "INPUT_MEAL") == 0) {
        if (sscanf(request, "INPUT_MEAL/%[^/]/%[^/]/%[^/]/%f", MII->userId, MII->dateTime, MII->foodName, &MII->gram) != 4) {
            sprintf(response, "[오류] 잘못된 입력 형식");
            return;
        }

        printf("[ 서버 ] | 식단 입력 치리 -> %s %s %s %.1f\n", MII->userId, MII->dateTime, MII->foodName, MII->gram);
        
        if (needConvert(FOOD_CSV_FILE, FOOD_DB_FILE)) {
            foodConvertCSVtoDB();
        } 
        else {
            printf("[ 서버 ] | 최신화 불필요, 기존 DB 사용!\n");
        }

        MII->kcal = inputFoodAndCalc(MII->foodName);

        if (MII->kcal != -1) {
            sprintf(response, "[성공] 음식 검사 성공");
            insertMeal(MII);
        }
        else {
            sprintf(response, "[실패] 검사 실패");
        }
        
        MIIfree(MII);
    } 

    // 운동 입력
    else if (strcmp(cmd, "INPUT_WORKOUT") == 0) {
        if (sscanf(request, "INPUT_WORKOUT/%[^/]/%[^/]/%[^/]/%f", WOII->userId, WOII->dateTime, WOII->exerciseName, &WOII->minutes) != 4) {
            sprintf(response, "[오류] 잘못된 입력 형식");
            return;
        }
        
        printf("[ 서버 ] | 운동 입력 치리 -> %s %s %s %.1f\n", WOII->userId ,WOII->dateTime, WOII->exerciseName, WOII->minutes);

        if (needConvert(EXERCISES_CSV_FILE, EXERCISES_DB_FILE)) {
            exercisesConvertCSVtoDB();
        } 
        else {
            printf("[ 서버 ] | 최신화 불필요, 기존 DB 사용!\n");
        }

        float met = inputWorkoutAndCalc(WOII->exerciseName);

        if (met != -1) {
            sprintf(response, "[성공] 운동 검사 성공");
            WOII->kcal = METM(met, WOII->minutes, selectWeight(WOII->userId));
            insertWorkout(WOII);
        }
        else {
            sprintf(response, "[실패] 검사 실패");
        }
        WOIIfree(WOII);
    } 

    // 체중 입력
    else if (strcmp(cmd, "INPUT_WEIGHT") == 0) {
        if (sscanf(request, "INPUT_WEIGHT/%[^/]/%[^/]/%f", WII->userId, WII->date, &WII->weight) != 3) {
            sprintf(response, "[오류] 잘못된 입력 형식");
            return;
        }
        sscanf(
            request,
            "INPUT_WEIGHT/%[^/]/%[^/]/%f",
            WII->userId, WII->date, &WII->weight
        );

        if (strlen(request) != 0) {
            sprintf(response, "[성공] 체중 입력 성공");
            insertWeight(WII);
            
            printf("[ 서버 ] | 체중 입력 치리 -> %s %s %.1f\n", WII->userId, WII->date, WII->weight);
        }
        else {
            sprintf(response, "[실패] 입력 실패");
        }

        WIIfree(WII);
    } 

    // 날짜별 기록 조회
    else if (strcmp(cmd, "GET_RECORD") == 0) {
        char id[ID_SIZE], date[11], *rds = (char*)malloc(BUF_SIZE * sizeof(char));
        if (sscanf(request, "GET_RECORD/%[^/]/%s", id, date) != 2) {
            sprintf(response, "[오류] 잘못된 입력 형식");
            return;
        }
        printf("[ 서버 ] | 날짜별 기록 조회\n");
        
        memset(rds, 0, sizeof(BUF_SIZE));
        rds = viewRecordsByDate(id, date);
        send(clientSock, rds, strlen(rds), 0);
    } 

    // 감량 진행률 계산
    else if (strcmp(cmd, "CHECK_PROGRESS") == 0) {
        char id[ID_SIZE], *cwlpstr = (char*)malloc(BUF_SIZE * sizeof(char));;
        
        if (sscanf(request, "CHECK_PROGRESS/%s", id) != 1) {
            sprintf(response, "[오류] 잘못된 입력 형식");
            return;
        }
        printf("[ 서버 ] | 감량 진행률 계산\n");
        
        memset(cwlpstr, 0, sizeof(BUF_SIZE));
        cwlpstr = checkWeightLossProgress(id);
        send(clientSock, cwlpstr, strlen(cwlpstr), 0);
        
    } 
    // 피드백 추천
    else if (strcmp(cmd, "FEEDBACK") == 0) {
        char id[ID_SIZE], date[11], *fbs = (char*)malloc(BUF_SIZE * sizeof(char));
        
        if (sscanf(request, "FEEDBACK/%[^/]/%s", id, date) != 2) {
            sprintf(response, "[오류] 잘못된 입력 형식");
            return;
        }
        printf("[ 서버 ] | 피드백 추천\n");
    
        memset(fbs, 0, sizeof(BUF_SIZE));
        fbs = viewRecordsByDate(id, date);
        send(clientSock, fbs, strlen(fbs), 0);
    } 
    
    // 로그아웃 처리
    else if (strcmp(cmd, "LOGOUT") == 0) {
        char id[ID_SIZE];

        if (sscanf(request, "LOGOUT/%s", id) != 1) {
            sprintf(response, "[오류] 잘못된 입력 형식");
            return;
        }

        if (strlen(request) != 0) {
            sprintf(response, "[성공] 로그아웃 성공");
            printf("[ 서버 ] | %s 님이 로그아웃 처리 되었습니다.\n", id);
        }
        else {
            sprintf(response, "[실패] 로그아웃 실패");
        }
    } 

    // 회원 탈퇴 처리
    else if (strcmp(cmd, "DELETE_ID") == 0) {
        char id[ID_SIZE];
        
        if (sscanf(request, "DELETE_ID/%s", id) != 1) {
            sprintf(response, "[오류] 잘못된 입력 형식");
            return;
        }
        
        printf("> %lu\n", strlen(request));
        if (strlen(request) != 0) {
            sprintf(response, "[성공] 회원 탈퇴가 완료 성공");
            deleteUserData(id);
            printf("[ 서버 ] | %s 님이 회원 탈퇴 처리 되었습니다.\n", id);   
        }
        else {
            sprintf(response, "[실패] 회원 탈퇴가 완료 실패");
        }
    }
    
    else {
        sprintf(response, "[오류] 알 수 없는 명령어");
    }
}