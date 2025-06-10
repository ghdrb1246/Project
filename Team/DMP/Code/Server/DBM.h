#ifndef _DBM_
#define _DBM_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h> // mkdir
#include "sqlite/sqlite3.h"
#include "InputInfo.h"

// Windows 환경 
#ifdef _WIN32
    #include <direct.h>
    #define mkdir(dir, mode) _mkdir(dir)
    
// macOS 환경
#else 
    #include <sys/types.h>
    #include <unistd.h>
#endif

// DB 관리 모듈

/**
 * @brief DB 파일을 "DB/..filename" 폴더에 생성 또는 오픈
 * 
 * @param filename DB 파일 이름
 * @return int DB파일 열 떄 0:성공, 1:오류
 */
int DBO(const char *filename);

/**
 * @brief DB 닫기
 */
void DBC();

/**
 * @brief 현재 DB 파일에 테이블 생성 여부 검사
 * 
 * @param tableName 확인할 DB 테이블 이름
 * @return int 테이블이 존재하지 않으면 0, 존재하면 1를 반환
 */
int tableExists(const char *tableName);

/**
 * @brief DB 테이블 생성
 * 
 */
void tableAdd();

/**
 * @brief 사용자 id가 존재하는지 확인
 * 
 * @param userId 확인할 id
 * @return int | 성공 1, 실패 0
 */
int userExists(const char *userId);

/**
 * @brief 회인가입
 * 
 * @param USI 사용자 가입 정보
 * @return int | 성공 1, 실패 0
 */
int signupUser(UserSignupInfo *USI);

/**
 * @brief 로그인
 * 
 * @param userId  사용자 id
 * @param pw  사용자 pw
 * 
 * @return int | 성공 1, 실패 0
 */
int loginUser(const char *userId, const char *pw);

/**
 * @brief 식단 저장
 * 
 * @param MII 식단 입력 데이터
 */
void insertMeal(MealInputInfo *MII);

/**
 * @brief 운동 저장
 * 
 * @param WOII 운동 입력 데이터
 */
void insertWorkout(WorkOutInputInfo *WOII);

/**
 * @brief 체중 저장
 * 
 * @param WII 체중 입력 데이터
 */
void insertWeight(WeightInputInfo *WII);

/**
 * @brief 체중 조회 함수
 * 
 * @param userId 사용자 id
 * @return float 조회한 체중 반횐
 */
float selectWeight(const char *userId);

/**
 * @brief 사용자 ID로 연관 데이터 삭제 함수
 * 
 * @param userId 사용자 id
 */
void deleteUserData(const char *userId);

/**
 * @brief 날짜별 기록 조회
 * 
 * @param userId 사용자 id
 * @param date 획인할 날짜
 * @return char* 식단:Data|...#운동:Data|...#체중:Data|...
 */
char *viewRecordsByDate(const char *userId, const char *date);

/**
 * @brief 진행률 계산
 * 
 * @param userId 사용자 id
 * @return char* "진행률:Data...|초기 체중:Data...|목표 체중:Data...|현재 체중:Date...
 */
char *checkWeightLossProgress(char *userId);

/**
 * @brief 피드백 
 * 
 * @param userId 사용자 id
 * @return char* "최신 체중:Data...|전전 체중:Data...
 */
char *feedBack(char *userId);

#endif