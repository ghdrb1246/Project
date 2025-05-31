#ifndef _DBM_
#define _DBM_

#include <stdio.h>
#include <stdlib.h>

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
void usersTable();

void usersDietsTable();

void usersDietRecordsTable();

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
 * @param userId 사용자 id
 * @param userPw 시용자 pw
 * @return int | 성공 1, 실패 0
 */
int signupUser(const char *userId, const char *userPw);

/**
 * @brief 로그인
 * 
 * @param userId 사용자 id
 * @param userPw 시용자 pw
 * @return int | 성공 1, 실패 0
 */
int loginUser(const char *userId, const char *userPw);

#endif