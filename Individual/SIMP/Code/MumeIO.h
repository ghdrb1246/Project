#ifndef _MIO_
#define _MIO_

#include <stdio.h>
#include "UserStructure.h"

// 메뉴 입출력 모듈

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

#endif