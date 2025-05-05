// #ifndef _DBM_H_
// #define _DBM_H_

#include <stdio.h>
#include <stdlib.h>

struct Schedule {
    char *title;
    char *scheduled_date_time;
    char *end_date_time;
    char *tag;
    int priority;
    char *status;
};

/**
 * @brief 일정 구조체
 * 
 * @param title 제목
 * @param scheduled_date_time 예정 날짜/시간
 * @param end_date_time 종료 날짜/시간
 * @param tag 일정 관련 태그
 * @param priority 우선순위 (0:없음, 1:낮음, 2:중간, 3:높음)
 * @param status 상태(TODO, DOING, DONE)
 * 
 */
typedef struct Schedule Schedule;

/**
 * @brief 메모리 할당
 * 
 * @return Schedule 
 */
Schedule smalloc();

/**
 * @brief DB을 열기
 * 
 * @param F DB 파일
 * @return int DB파일 열 떄 0:성공, 1:오류
 */
int DBO(char *F);

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
void saveDB(Schedule s);

/**
 * @brief DB 데이터 업데이트
 * 
 * @param s 일정 정보 구조체
 * @param id id
 */
void updateDB(Schedule s, int id);

/**
 * @brief DB 항목 삭제
 * 
 * @param s 일정 정보 구조체
 */
void deleteDB(int id);

/**
 * @brief DB 조회
 * 
 * @param s 일정 정보 구조체
 */
void viewDB();

/**
 * @brief DB 상테별 전체 조회
 * 
 * @param status TODO, DOING, DONE 중 선택
 */
void viewAllByStatus(char *status);

/**
 * @brief 특정 인덱스 위치 id 반환
 * 
 * @param status 상태
 * @param user_no 사용자 메뉴 선택 번호
 * @return int 
 */
int statusIndexToId(char *status, int user_no);

/**
 * @brief 특정 id 일정 조회
 * 
 * @param id 특정 일정 id
 * @return Schedule
 */
Schedule idToStatusView(int id);

/**
 * @brief 상태 : TODO -> DOING
 * 
 * @param id 특정 일정 id
 */
void todoToDoing(int id);

/**
 * @brief 상태 : DOING -> DONE
 * 
 * @param id 특정 일정 id
 */
void doingToDone(int id);

/**
 * @brief 일정 태그 전체 조회
 * 
 */
void viewAllByTag();

/**
 * @brief 특정 인덱스 tag 반환
 * 
 * @param user_no 사용자 메뉴 선택 번호
 * @return char* 
 */
char *tagIndexToId(int user_no);

/**
 * @brief tag 건수 반환
 * 
 * @param tag 특정 tag
 * @return int 
 */
int tagCount(char *tag);

/**
 * @brief 특정 tag 전채 조회
 * 
 * @param tag 특정 tag
 */
void viewTagByschedule(char *tag);

// #endif