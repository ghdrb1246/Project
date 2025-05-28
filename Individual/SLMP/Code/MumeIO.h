#ifndef _MIO_
#define _MIO_

#include <stdio.h>
#include "UserStructure.h"

#define P_MENU_TITLE(s)     printf("================= [ %s ] =================\n", s)
#define P_MENU_IN           printf("----------------------------------------------------------\n")
#define P_MENU_END          printf("==========================================================\n\n")
#define P_MENU_SB_S(s, l)   printf("[ %s %s------------------------------------------ ]\n", s, l)
#define P_MENU_SB           printf("[ ------------------------------------------------------ ]\n")
#define P_MENU_TC(t, c)     printf("{ %s | %d }\n", t, c)

/* --------------------- 메뉴 입출력 모듈 --------------------- */

/**
 * @brief 메뉴 뒤로가기
 * 
 */
void runMenu();

/**
 * @brief 메인 메뉴
 * 
 * @return MenuState : SRM, CVM, EXIT
 */
MenuState mainMenu();

/**
 * @brief 일정 등록 메뉴
 * 
 * @return MenuState : MM
 */
MenuState scheduleRegistrationMenu();

/**
 * @brief 일정 조회 메뉴
 * 
 * @return MenuState : SVBSM, TVM, MM
 */
MenuState calendarViewMenu();

/**
 * @brief 일정 조회 메뉴 : 상테별
 * 
 * @return MenuState : SMM, DSM, SVBSM, SCM, SPM
 */
MenuState scheduleViewByStatusMenu();

/**
 * @brief 일정 수정
 *
 * @return MenuState : SVBSM
 */
MenuState scheduleModificationMenu();

/**
 * @brief 일정 삭제
 * 
 * @return MenuState : SVBSM
 */
MenuState deleteScheduleMenu();

/**
 * @brief 일정 완료
 * 
 * @return MenuState : SVBSM
 */
MenuState scheduleCompleteMenu();

/**
 * @brief 일정 연기
 * 
 * @return MenuState : SVBSM
 */
MenuState schedulePostponedMenu();

/**
 * @brief 태그 전체 조회
 * 
 * @return MenuState : SBTM, CVM
 */
MenuState tagViewMenu();

/**
 * @brief 태그별 일정 조회
 * 
 * @return MenuState : TVM
 */
MenuState scheduleByTagMenu();

/* --------------------------------------------------------- */

/* --------------------- Input 관련 함수 --------------------- */

/**
 * @brief 제목 입력
 * 
 * @param title 제목 문자열
 */
void inputTitle(char *title);

/**
 * @brief 예정 날짜/시간 입력
 * 
 * @param sdt 예정 날짜/시간 문자열
 */
void inputSDT(char *sdt);

/**
 * @brief 종료 날짜/시간(선택) 입력
 * 
 * @param sdt 종료 날짜/시간 문자열
 */
void inputEDT(char *edt);

/**
 * @brief 태그 입력
 * 
 * @param tag 태그 문자열
 */
void inputTag(char *tag);

/**
 * @brief 우선순위 입력
 * 
 * @param priority 우선순위 정수
 */
int inputPriority();

/* --------------------------------------------------------- */

#endif