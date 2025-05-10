#ifndef _DBM_
#define _DBM_

#include <stdio.h>
#include <stdlib.h>
#include "UserStructure.h"

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
 * @brief DB 테이블 생성
 * 
 */
void tableDB();

/**
 * @brief DB에 구조체 저장
 * 
 * @param s 일정 정보 구조체
 */
void saveDB(Schedule *s);

/**
 * @brief DB 데이터 업데이트
 * 
 * @param s 일정 정보 구조체
 * @param id id
 */
void updateDB(Schedule *s, int id);

/**
 * @brief DB 항목 삭제
 * 
 * @param s 일정 정보 구조체
 */
void deleteDB(int id);

/**
 * @brief 전체 일정 상태 예정 날짜/시간(종료) 따라 "TODO" -> "DOING" 변경
 * 
 */
void checkScheduleStatus();

/**
 * @brief DB 상테별 전체 조회
 * 
 * @param status TODO, DOING, DONE 중 선택
 */
void viewAllByStatus(const char *status);

/**
 * @brief 특정 인덱스 위치 id 반환
 * 
 * @param status 상태
 * @param user_no 사용자 메뉴 선택 번호
 * @return int 
 */
int statusIndexToId(const char *status, int user_no);

/**
 * @brief 특정 id 일정 조회
 * 
 * @param id 특정 일정 id
 * @return Schedule*
 */
Schedule *idToStatusView(int id);

/**
 * @brief 특정 상태 변경 (TODO -> DOING -> DONE)
 * 
 * @param status 변경할 상태 (DOING, DONE)
 * @param id 특정 일정 id
 */
void updateStatus(const char *status, int id);

/**
 * @brief 일정 태그 전체 조회
 * 
 */
void viewAllByTag();

/* 
// 특정 인덱스 tag 반환
char *tagIndexToId(int user_no);

// tag 건수 반환
int tagCount(char *tag);
*/

/**
 * @brief 특정 번호에 태그와 해당 태그 계수 반환(tagIndexToId() + tagCount())
 * 
 * @param user_no 특정 사용자 번호
 * @return TagCount *
 */
TagCount *indexToTagCount(int user_no);

/**
 * @brief 특정 tag 전채 조회
 * 
 * @param tag 특정 tag
 */
void viewTagByschedule(char *tag);

#endif