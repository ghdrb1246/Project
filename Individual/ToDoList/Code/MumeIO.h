// #ifndef _MIO_H_
// #define _MIO_H

#include <stdio.h>

// 메뉴 입출력 모듈

/**
 * @brief 메인 메뉴
 * 
 */
void main_meun();

/**
 * @brief 일정 등록 메뉴
 * 
 */
void scheduleRegistrationMenu();

/**
 * @brief 일정 조회 메뉴
 * 
 */
void calendarViewMenu();

/**
 * @brief 일정 조회 메뉴 : 상테별
 * 
 * @param statusNumer 상태 열거형 상수 인덱스
 */
void scheduleViewMenuByStatus(int statusNumer);

static void _TODO();
static void _DOING();
static void _DONE();

/**
 * @brief 일정 수정
 * 
 * @param id 특정 수정할 일정 id
 */
void scheduleModificationMenu(int id);

/**
 * @brief 일정 삭제
 * 
 * @param id 특정 삭제할 일정 id
 */
void deleteScheduleMenu(int id);

/**
 * @brief 일정 완료
 * 
 * @param id 특정 완료할 일정 id
 */
void scheduleCompleteMenu(int id);

/**
 * @brief 일정 연기
 * 
 * @param id 특정 연기할 일정 id
 */
void schedulePostponedMenu(int id);

/**
 * @brief 태그 전체 조회
 * 
 */
void tagViewMenu();

/**
 * @brief 태그별 일정 조회
 * 
 * @param user_no 특정 사용자 번호
 */
void scheduleMenuByTag(int user_no);

// void scheduleMenuByTag(char *tag);

// #endif