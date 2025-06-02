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
    const char *foldername = "Server/DB";  // 폴더 이름
    char path[256];                 // 파일 경로 생성 문자열
    int mode = 0755;                // 생성하려는 디렉터리에 대한 접근 권한 설정 값
    
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
    char *users_sql = "CREATE TABLE IF NOT EXISTS user (id TEXT PRIMARY KEY, password TEXT, gender TEXT, age INTEGER, height REAL, current_weight REAL, goal_weight REAL);";
    char *msal_sql = "CREATE TABLE IF NOT EXISTS meal (id INTEGER PRIMARY KEY AUTOINCREMENT, user_id TEXT, datetime TEXT, food_name TEXT, gram REAL, kcal REAL, FOREIGN KEY (user_id) REFERENCES user(id) ON DELETE CASCADE);";
    char *workout_sql = "CREATE TABLE IF NOT EXISTS workout (id INTEGER PRIMARY KEY AUTOINCREMENT, user_id TEXT, datetime TEXT, exercise_name TEXT, hour REAL, kcal REAL, FOREIGN KEY (user_id) REFERENCES user(id) ON DELETE CASCADE);";
    char *weightrecord_sql = "CREATE TABLE IF NOT EXISTS weightRecord (user_id TEXT, date TEXT, weight REAL, PRIMARY KEY (user_id, date), FOREIGN KEY (user_id) REFERENCES user(id) ON DELETE CASCADE);";
    char *err_msg = NULL;

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
}

int userExists(const char *id) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT * FROM user WHERE id = ?;";

    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    sqlite3_bind_text(stmt, 1, id, -1, SQLITE_STATIC);
    
    int result = (sqlite3_step(stmt) == SQLITE_ROW);
    
    sqlite3_finalize(stmt);

    return result;
}

int signupUser(UserSignupInfo *USI) {
    const char *sql = "INSERT INTO user(id, password, gender, age, height, current_weight, goal_weight) VALUES(?, ?, ?, ?, ?, ?, ?);";
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
    sqlite3_bind_double(stmt, 6, USI->initialWeight);
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

int loginUser(const char *id, const char *pw) {
    sqlite3_stmt *stmt;
    char *sql = "SELECT * FROM user WHERE id = ? AND password = ?;";
    
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    sqlite3_bind_text(stmt, 1, id, -1, SQLITE_STATIC);
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

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("식단 INSERT 실패: %s\n", sqlite3_errmsg(db));
    } else {
        printf("식단 입력 완료!\n");
    }

    sqlite3_finalize(stmt);
}

void insertWorkout(WorkOutInputInfo *WOII) {
    const char *sql = "INSERT INTO workout (user_id, datetime, exercise_name, hour, kcal) VALUES (?, ?, ?, ?, ?);";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("SQL 준비 실패: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_text(stmt, 1, WOII->userId, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, WOII->dateTime, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, WOII->exerciseName, -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 4, WOII->hour);
    sqlite3_bind_double(stmt, 5, WOII->kcal);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("운동 INSERT 실패: %s\n", sqlite3_errmsg(db));
    } else {
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
    } else {
        printf("체중 입력/갱신 완료!\n");
    }

    sqlite3_finalize(stmt);
}