#ifndef _DBM_
#define _DBM_

#include <stdio.h>
#include <stdlib.h>
#include "InputInfo.h"
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
 * @param id 확인할 id
 * @return int | 성공 1, 실패 0
 */
int userExists(const char *id);

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
 * @param id  사용자 id
 * @param pw  사용자 pw
 * 
 * @return int | 성공 1, 실패 0
 */
int loginUser(const char *id, const char *pw);

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

#endif