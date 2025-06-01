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

    usersTable();
    usersDietsTable();
    usersDietRecordsTable();

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

void usersTable() {
    // 유저 정보
    // CREATE TABLE IF NOT EXISTS [TABLE]
    // 테이블이 없다면 테이블을 추가 -> 따라서 tableExists()로 해당 테이블 확인할 필요 없다
    char *sql = "CREATE TABLE IF NOT EXISTS users (id TEXT PRIMARY KEY, pw TEXT NOT NULL, gender TEXT, age INTEGER)";
    char *err_msg = 0;

    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) { 
        fprintf(stderr, "테이블 생성 오류: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
}

void usersDietsTable() {
    // 유저 다이어트 정보
    // CREATE TABLE IF NOT EXISTS [TABLE]
    // 테이블이 없다면 테이블을 추가 -> 따라서 tableExists()로 해당 테이블 확인할 필요 없다
    char *sql = "CREATE TABLE IF NOT EXISTS user_diets (user_id TEXT PRIMARY KEY, height REAL, initial_weight REAL, goal_weight REAL, FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE);";
    char *err_msg = 0;

    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) { 
        fprintf(stderr, "테이블 생성 오류: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
}

void usersDietRecordsTable() {
    // 유저 다이어트 기록
    // CREATE TABLE IF NOT EXISTS [TABLE]
    // 테이블이 없다면 테이블을 추가 -> 따라서 tableExists()로 해당 테이블 확인할 필요 없다
    char *sql = "CREATE TABLE IF NOT EXISTS diet_records (record_id INTEGER PRIMARY KEY AUTOINCREMENT, user_id TEXT, date TEXT, meal TEXT, meal_gram REAL, workout TEXT, workout_duration REAL, weight REAL, FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE);";
    char *err_msg = 0;

    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) { 
        fprintf(stderr, "테이블 생성 오류: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
}

int userExists(const char *id) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT * FROM users WHERE id = ?;";

    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    sqlite3_bind_text(stmt, 1, id, -1, SQLITE_STATIC);
    
    int result = (sqlite3_step(stmt) == SQLITE_ROW);
    
    sqlite3_finalize(stmt);

    return result;
}

int signupUser(UserSignupInfo *USI) {
    const char *users_sql =      "INSERT INTO users(id, pw, gender, age) VALUES(?, ?, ?, ?);";
    const char *user_diets_sql = "INSERT INTO user_diets(user_id, height, initial_weight, goal_weight) VALUES(?, ?, ?, ?);";

    sqlite3_stmt *stmt;

    // 첫 번째 INSERT: users
    if (sqlite3_prepare_v2(db, users_sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("SQL 준비 실패: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    // 바인딩
    sqlite3_bind_text(stmt, 1, USI->id, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, USI->pw, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, USI->gender, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, USI->age);

    // 실행
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("users INSERT 실패: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return 0;
    }
    sqlite3_finalize(stmt);

    // 두 번째 INSERT: user_diets
    if (sqlite3_prepare_v2(db, user_diets_sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("user_diets SQL 준비 실패: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    // 바인딩
    sqlite3_bind_text(stmt, 1, USI->id, -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 2, USI->height);
    sqlite3_bind_double(stmt, 3, USI->initialWeight);
    sqlite3_bind_double(stmt, 4, USI->goalWeight);

    // 실행
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("user_diets INSERT 실패: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return 0;
    }
    sqlite3_finalize(stmt);
    
    return 1;
}

int loginUser(const char *id, const char *pw) {
    sqlite3_stmt *stmt;
    char *sql = "SELECT * FROM users WHERE id = ? AND pw = ?;";
    
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    sqlite3_bind_text(stmt, 1, id, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, pw, -1, SQLITE_STATIC);
    
    int result = (sqlite3_step(stmt) == SQLITE_ROW);
    
    sqlite3_finalize(stmt);

    return result;
}