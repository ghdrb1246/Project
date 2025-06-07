#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h> // mkdir
#include "sqlite/sqlite3.h"
#include "DBM.h"
#include "InputInfo.h"


#ifdef _WIN32
    // Windows 환경 
    #include <direct.h>
    #define mkdir(dir, mode) _mkdir(dir)

#else 
    // macOS 환경
    #include <sys/types.h>
    #include <unistd.h>
#endif

// DB 관리 모듈

static sqlite3 *db;

int DBO(const char *filename) {
    const char *foldername = "Server/DB";   // 폴더 이름
    char path[256];                         // 파일 경로 생성 문자열
    int mode = 0755;                        // 생성하려는 디렉터리에 대한 접근 권한 설정 값
    
    // DB 폴더 확인 및 생성
    #ifdef _WIN32
        // Windows 환경일 때
        struct _stat st = { 0 };
        if (_stat(foldername, &st) != 0) {
            if (mkdir(foldername, mode) != 0) {
                perror("DB 폴더 생성 실패");
                return 1;
            }
        }
    #else
        // macOS 환경일 때
        struct stat st = { 0 };
        if (stat(foldername, &st) != 0) {
            if (mkdir(foldername, mode) != 0) {
                perror("DB 폴더 생성 실패");
                return 1;
            }
        }
    #endif

    // 파일 경로 생성
    snprintf(path, sizeof(path), "%s/%s.db", foldername, filename);

    // SQLite DB 열기 또는 생성
    if (sqlite3_open(path, &db) != SQLITE_OK) {
        fprintf(stderr, "DB 열기 실패: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    // 테이블이 존재하지 않으면 0, 존재하면 1를 반환
    //  if (tableExists("users") != 1) usersTableDB();

    tableAdd();

    return 0; // 성공
}

void DBC() {
    sqlite3_close(db);
}

int tableExists(const char *tableName) {
    sqlite3_stmt *stmt;
    char *sql = sqlite3_mprintf("SELECT COUNT(*) FROM sqlite_master WHERE type = 'table' AND name = '%s';", tableName);
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    int real_id = -1;
    
    if (rc == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            real_id = sqlite3_column_int(stmt, 0);
            
            sqlite3_finalize(stmt);
        }
        else fprintf(stderr, "'%s' 해당 파일에 테이블이 존재하지 않습니다\n", sqlite3_errmsg(db));

    }
    else printf("SQL 실행 실패 : %s\n", sqlite3_errmsg(db));

    sqlite3_free(sql);  // SQL 문자열 메모리 해제

    return real_id;
}

void tableAdd() {
    // 유저 정보
    // CREATE TABLE IF NOT EXISTS [TABLE]
    // 테이블이 없다면 테이블을 추가 -> 따라서 tableExists()로 해당 테이블 확인할 필요 없다
    char *users_sql = "CREATE TABLE IF NOT EXISTS user (user_id TEXT PRIMARY KEY, password TEXT, gender TEXT, age INTEGER, height REAL, exercise_weight REAL, goal_weight REAL);";
    char *msal_sql = "CREATE TABLE IF NOT EXISTS meal (id INTEGER PRIMARY KEY AUTOINCREMENT, user_id TEXT, datetime TEXT, food_name TEXT, gram REAL, kcal REAL, FOREIGN KEY (user_id) REFERENCES user(id) ON DELETE CASCADE);";
    char *workout_sql = "CREATE TABLE IF NOT EXISTS workout (id INTEGER PRIMARY KEY AUTOINCREMENT, user_id TEXT, datetime TEXT, exercise_name TEXT, minutes REAL, kcal REAL, FOREIGN KEY (user_id) REFERENCES user(id) ON DELETE CASCADE);";
    char *weightrecord_sql = "CREATE TABLE IF NOT EXISTS weightRecord (user_id TEXT, date TEXT, weight REAL, PRIMARY KEY (user_id, date), FOREIGN KEY (user_id) REFERENCES user(id) ON DELETE CASCADE);";
    char *sql[4] = { users_sql, msal_sql, workout_sql, weightrecord_sql };
    char *err_msg = NULL;

    for (int i = 0 ; i < 4; i++) {
        if (sqlite3_exec(db, sql[i], 0, 0, &err_msg) != SQLITE_OK) {
            printf("%d 인텍스의 테이블 생성 실패: %s\n", i + 1, err_msg);
            sqlite3_free(err_msg);
            return;
        }
    }
    /* 
    // user 테이블
    if (sqlite3_exec(db, users_sql, 0, 0, &err_msg) != SQLITE_OK) {
        printf("user 테이블 생성 실패: %s\n", err_msg);
        sqlite3_free(err_msg);
        return;
    }

    // 식단 테이블
    if (sqlite3_exec(db, msal_sql, 0, 0, &err_msg) != SQLITE_OK) {
        printf("식단 테이블 생성 실패: %s\n", err_msg);
        sqlite3_free(err_msg);
        return;
    }

    // 운동 테이블
    if (sqlite3_exec(db, workout_sql, 0, 0, &err_msg) != SQLITE_OK) {
        printf("운동 테이블 생성 실패: %s\n", err_msg);
        sqlite3_free(err_msg);
        return;
    }
    
    // 체중 테이블
    if (sqlite3_exec(db, weightrecord_sql, 0, 0, &err_msg) != SQLITE_OK) {
        printf("체중 테이블 생성 실패: %s\n", err_msg);
        sqlite3_free(err_msg);
        return;
    } 
    */
}

int userExists(const char *userId) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT * FROM user WHERE id = ?;";

    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    sqlite3_bind_text(stmt, 1, userId, -1, SQLITE_STATIC);
    
    int result = (sqlite3_step(stmt) == SQLITE_ROW);
    
    sqlite3_finalize(stmt);

    return result;
}

int signupUser(UserSignupInfo *USI) {
    const char *sql = "INSERT INTO user(user_id, password, gender, age, height, exercise_weight, goal_weight) VALUES(?, ?, ?, ?, ?, ?, ?);";
    sqlite3_stmt *stmt;

    // 첫 번째 INSERT: users
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("SQL 준비 실패: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    // 바인딩
    sqlite3_bind_text(stmt, 1, USI->id, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, USI->pw, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, USI->gender, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, USI->age);
    sqlite3_bind_double(stmt, 5, USI->height);
    sqlite3_bind_double(stmt, 6, USI->exerciseWeight);
    sqlite3_bind_double(stmt, 7, USI->goalWeight);

    // 실행
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("user INSERT 실패: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);

        return 0;
    }
    sqlite3_finalize(stmt);

    return 1;
}

int loginUser(const char *userId, const char *pw) {
    sqlite3_stmt *stmt;
    char *sql = "SELECT * FROM user WHERE user_id = ? AND password = ?;";
    
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    sqlite3_bind_text(stmt, 1, userId, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, pw, -1, SQLITE_STATIC);
    
    int result = (sqlite3_step(stmt) == SQLITE_ROW);
    
    sqlite3_finalize(stmt);

    return result;
}

void insertMeal(MealInputInfo *MII) {
    const char *sql = "INSERT INTO meal (user_id, datetime, food_name, gram, kcal) VALUES (?, ?, ?, ?, ?);";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("SQL 준비 실패: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_text(stmt, 1, MII->userId, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, MII->dateTime, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, MII->foodName, -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 4, MII->gram);
    sqlite3_bind_double(stmt, 5, MII->kcal);

    // 입력 확인
    // printf("DB 식단 : %s - %lu\n", MII->foodName, strlen(MII->foodName));
    // printf("DB 식단 : %s - 공백 ? : %s(길이|%lu, 공백 위치|%lu)\n", MII->foodName, (strcspn(MII->foodName, "\n") != strlen(MII->foodName)) ? "있음" : "없음", strlen(MII->foodName), strcspn(MII->foodName, "\n"));

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("식단 INSERT 실패: %s\n", sqlite3_errmsg(db));
    } 
    else {
        printf("식단 입력 완료!\n");
    }

    sqlite3_finalize(stmt);
}

void insertWorkout(WorkOutInputInfo *WOII) {
    const char *sql = "INSERT INTO workout (user_id, datetime, exercise_name, minutes, kcal) VALUES (?, ?, ?, ?, ?);";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("SQL 준비 실패: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_text(stmt, 1, WOII->userId, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, WOII->dateTime, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, WOII->exerciseName, -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 4, WOII->minutes);
    sqlite3_bind_double(stmt, 5, WOII->kcal);

    // 입력 확인
    // printf("DB 운동 : %s - %lu\n", WOII->exerciseName, strlen(WOII->exerciseName));
    // printf("DB 운동 : %s - 공백 ? : %s(길이|%lu, 공백 위치|%lu)\n", WOII->exerciseName, (strcspn(WOII->exerciseName, "\n") != strlen(WOII->exerciseName)) ? "있음" : "없음", strlen(WOII->exerciseName), strcspn(WOII->exerciseName, "\n"));

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("운동 INSERT 실패: %s\n", sqlite3_errmsg(db));
    } 
    else {
        printf("운동 입력 완료!\n");
    }

    sqlite3_finalize(stmt);
}

void insertWeight(WeightInputInfo *WII) {
    const char *sql = "INSERT OR REPLACE INTO weightrecord (user_id, date, weight) VALUES (?, ?, ?);";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("SQL 준비 실패: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_text(stmt, 1, WII->userId, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, WII->date, -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 3, WII->weight);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("체중 INSERT 실패: %s\n", sqlite3_errmsg(db));
    } 
    else {
        printf("체중 입력/갱신 완료!\n");
    }

    sqlite3_finalize(stmt);
}

float selectWeight(const char *userId) {
    const char *sql = "SELECT weight FROM weightRecord WHERE user_id = ? ORDER BY date DESC LIMIT 1;";
    float weight = -1;
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("SQL 준비 실패: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    sqlite3_bind_text(stmt, 1, userId, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        printf("체중 조회 완료!\n");

        weight = (float)sqlite3_column_double(stmt, 0);
    } 

    else {
        printf("체중 조회 실패: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);

    return weight;
}

void deleteUserData(const char *userId) {
    sqlite3_stmt *stmt;
    const char *tableNeame[4] = { "meal", "workout", "weightRecord", "user" };
    int conut = 0;

    for (int i = 0; i < 4; i++) {
        char *sql = (char*)malloc((strlen(tableNeame[i]) + 32) * sizeof(char));
        snprintf(sql, (strlen(tableNeame[i]) + 32), "DELETE FROM %s WHERE user_id = ?;", tableNeame[i]);
        printf("%12s -> %s\n", tableNeame[i], sql);

        if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
            printf("%s 의 SQL 준비 실패: %s\n", tableNeame[i], sqlite3_errmsg(db));
            return;
        }
        
        sqlite3_bind_text(stmt, 1, userId, -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            printf("%s 테이블의 데이터 모두 삭제 완료!\n", tableNeame[i]);
            conut++;
        } 
        else {
            printf("%s 테이블 데이터 삭제 실패: %s\n", tableNeame[i], sqlite3_errmsg(db));
        }
        sqlite3_finalize(stmt);
    }
    
    if (conut > 3) {
        printf("회원탈퇴 및 연관 데이터 모두 삭제 완료!\n");
    } 
    else {
        printf("회원탈퇴 실패\n");
    }
}

char *viewRecordsByDate(const char *userId, const char *date)  {
    char *rdstr = (char*)malloc(BUF_SIZE * sizeof(char));
    sqlite3_stmt *stmt;

    // 식단 기록
    strcat(rdstr, "MEAL:");
    const char *sqlMeal = "SELECT time(datetime), food_name, gram, kcal FROM meal WHERE user_id=? AND date(datetime)=?;";
    if (sqlite3_prepare_v2(db, sqlMeal, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, userId, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, date, -1, SQLITE_STATIC);

        int first = 1;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            if (!first) strcat(rdstr, "|");
            char entry[128];
            snprintf(entry, sizeof(entry), "%s %s %.1fg %.1fkcal",
                sqlite3_column_text(stmt, 0),
                sqlite3_column_text(stmt, 1),
                sqlite3_column_double(stmt, 2),
                sqlite3_column_double(stmt, 3));
            strcat(rdstr, entry);
            first = 0;
        }
        sqlite3_finalize(stmt);
    }
    else {
        printf("식단 조회 실패: %s\n", sqlite3_errmsg(db));
    }

    strcat(rdstr, "#");

    // 운동 기록
    strcat(rdstr, "WORKOUT:");
    const char *sqlWorkout = "SELECT time(datetime), exercise_name, minutes, kcal FROM workout WHERE user_id=? AND date(datetime)=?;";
    if (sqlite3_prepare_v2(db, sqlWorkout, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, userId, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, date, -1, SQLITE_STATIC);

        int first = 1;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            if (!first) strcat(rdstr, "|");
            char entry[128];
            snprintf(entry, sizeof(entry), "%s %s %.1fh %.1fkcal",
                sqlite3_column_text(stmt, 0),
                sqlite3_column_text(stmt, 1),
                sqlite3_column_double(stmt, 2),
                sqlite3_column_double(stmt, 3));
            strcat(rdstr, entry);
            first = 0;
        }
        sqlite3_finalize(stmt);
    }
    else {
        printf("운동 조회 실패: %s\n", sqlite3_errmsg(db));
    }
    strcat(rdstr, "#");

    // 체중 기록
    strcat(rdstr, "WEIGHT:");
    const char *sqlWeight = "SELECT weight FROM weightRecord WHERE user_id=? AND date=?;";
    if (sqlite3_prepare_v2(db, sqlWeight, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, userId, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, date, -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            char entry[32];
            snprintf(entry, sizeof(entry), "%.1fkg", sqlite3_column_double(stmt, 0));
            strcat(rdstr, entry);
        } 
        else {
            strcat(rdstr, "기록없음");
        }
        sqlite3_finalize(stmt);
    }
    else {
        printf("체중 조회 실패: %s\n", sqlite3_errmsg(db));
    }

    return rdstr;
}

char *checkWeightLossProgress(char *userId) {
    sqlite3_stmt *stmt;
    char *cwlpstr = (char*)malloc(BUF_SIZE * sizeof(char));
    float progress = 0.0f, initialWeight = -1.0f, goalWeight = -1.0f, currentWeight = -1.0f;
    
    // 1) 초기/목표 체중
     const char *dietQuery = "SELECT exercise_weight, goal_weight FROM user WHERE user_id = ?;";
    if (sqlite3_prepare_v2(db, dietQuery, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, userId, -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            initialWeight = (float)sqlite3_column_double(stmt, 0);
            goalWeight = (float)sqlite3_column_double(stmt, 1);
        }
        else {
            strcat(cwlpstr, "초기/목표 체중 데이터 없음");
        }
        sqlite3_finalize(stmt);
    }
    else {
        printf("초기/목표 체중 조회 실패: %s\n", sqlite3_errmsg(db));
    }

    // 2) 최신 체중
    const char *weightQuery = "SELECT weight FROM weightRecord WHERE user_id = ? ORDER BY date DESC LIMIT 1;";
    if (sqlite3_prepare_v2(db, weightQuery, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, userId, -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            currentWeight = (float)sqlite3_column_double(stmt, 0);
        }
        else {
            strcat(cwlpstr, "최신 체중 데이터 없음");
        }

        sqlite3_finalize(stmt);
    }
    else {
        printf("최신 체중 조회 실패: %s\n", sqlite3_errmsg(db));
    }

    progress = ((initialWeight - currentWeight) / (initialWeight - goalWeight)) * 100.0f;
    if (progress < 0) progress = 0.0f;  // 음수 보정

    // 5) 결과 출력
    snprintf(
        cwlpstr, 
        BUF_SIZE,
        "progress:%.1f|initialWeight:%.1fkg|goalWeight:%.1fkg|currentWeight:%.1fkg",
        progress, initialWeight, goalWeight, currentWeight
    );
/* 
    // 3) 값이 유효한지 확인
    if (initialWeight < 0 || goalWeight < 0 || currentWeight < 0) {
        snprintf(cwlpstr, BUF_SIZE, "[오류] 진행률 계산에 필요한 데이터가 없습니다.");
    }
    else {
        // 4) 진행률 계산
        float progress = ((initialWeight - currentWeight) / (initialWeight - goalWeight)) * 100.0f;
        if (progress < 0) progress = 0.0f;  // 음수 보정
    
        // 5) 결과 출력
        snprintf(
            cwlpstr, 
            BUF_SIZE,
            "progress:%.1f|initialWeight:%.1fkg|goalWeight:%.1fkg|currentWeight:%.1fkg",
            progress, initialWeight, goalWeight, currentWeight
        );
    }
 */
    return cwlpstr;
}

char *feedBack(char *userId) {
    char *fbs = (char*)malloc(BUF_SIZE * sizeof(char));
    char latestDate[11] = "";
    float latestWeight, previousWeight;

    sqlite3_stmt *stmt;

    // 1) 최신 체중
    const char *latestQuery = "SELECT weight, date FROM weightRecord WHERE user_id = ? ORDER BY date DESC LIMIT 1;";
    if (sqlite3_prepare_v2(db, latestQuery, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, userId, -1, SQLITE_STATIC);
        
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            latestWeight = (float)sqlite3_column_double(stmt, 0);
            const unsigned char *dateText = sqlite3_column_text(stmt, 1);
            strncpy(latestDate, (const char*)dateText, sizeof(latestDate));
        }
        else {
            strcat(fbs, "최신 체중 데이터 없음");
        }
        sqlite3_finalize(stmt);
    }
    else {
        printf("최신 체중 조회 실패: %s\n", sqlite3_errmsg(db));
    }

    // 2) 전전 체중
    const char *prevQuery = "SELECT weight FROM weightRecord WHERE user_id = ? ORDER BY date DESC LIMIT 1 OFFSET 1;";
    if (sqlite3_prepare_v2(db, prevQuery, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, userId, -1, SQLITE_STATIC);
        
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            previousWeight = (float)sqlite3_column_double(stmt, 0);
        }
        else {
            strcat(fbs, "전전 체중 데이터 없음");
        }
        sqlite3_finalize(stmt);
    }
    else {
        printf("전전 체중 조회 실패: %s\n", sqlite3_errmsg(db));
    }
    
    // 3) 결과 출력
    snprintf(
        fbs, 
        BUF_SIZE,
        "latestWeight:%.1f|previousWeight:%.1f",
        latestWeight, previousWeight
    );

    return fbs;
}