#ifndef MENU_IO_H
#define MENU_IO_H

#include "InputInfo.h"

#define P_MENU_TITLE(s)     printf("================= [ %s ] =================\n", s)
#define P_MENU_IN           printf("----------------------------------------------------------\n")
#define P_MENU_END          printf("==========================================================\n\n")
#define P_MENU_SB_S(s, l)   printf("[ %s %s------------------------------------------ ]\n", s, l)

/* OUT */

/**
 * @brief 메인메뉴
 * 
 * @return int 메인 번호 반환
 */
int mainMenu();

/**
 * @brief 회원가입 메뉴
 * 
 * @param USI 회원가입 데이터 구조체
 */
void signupMenu(UserSignupInfo *USI);

/**
 * @brief 로그인 메뉴
 * 
 * @param id 사용자 id 
 * @param pw 사용자 pw
 */
void loginMenu(char *id, char *pw);

/**
 * @brief 사용자 메뉴
 * 
 * @param id 사용자 id 
 * @return int 메인 번호 반환
 */
int userMenu(char *id);

/**
 * @brief 식단 입력 메뉴
 * 
 * @param MII 식단 데이터 구조체
 */
void mealMenu(MealInputInfo *MII);

/**
 * @brief 운동 입력 메뉴
 * 
 * @param WOII 운동 데이터 구조체
 */
void workOutMenu(WorkOutInputInfo *WOII);

/**
 * @brief 체중 입력 메뉴
 * 
 * @param WII 체중 데이터 구조체
 */
void weightMenu(WeightInputInfo *WII);

/**
 * @brief 날짜별 기록 조회 출력 메뉴
 * 
 * @param rds 기록 데이터 문자열
 * @param date 기록을 획인할 날짜
 */
void viewRecordsByDate_OUT_Menu(char *rds, char *date);

/**
 * @brief 감량 진행률 조회 메뉴
 * 
 * @param initialWeight  초기 체중
 * @param goalWeight     목표 체중
 * @param exerciseWeight 현재 체중
 */
void checkWeightLossProgressMenu(const float initialWeight, const float goalWeight, const float exerciseWeight);

/**
 * @brief 피드백 메뉴
 * 
 * @param weight 키
 * @param kcal   칼로리
 */
void feedBackMenu(const float weight, const float kcal);

/**
 * @brief 로그아웃 메뉴
 * 
 * @param id 사용자 id 
 */
void logOutMenu(const char *id);

/**
 * @brief 회원탈퇴 메뉴
 * 
 * @param id 사용자 id
 */
void deleteIdMenu(const char *id);

/* In */

/**
 * @brief fgets()함수 활용 문자열 입력
 * 
 * @param prompt 문자열
 * @param buf    변수
 * @param size   문자열 크기
 */
void inputLine(const char *prompt, char *buf, int size);

/**
 * @brief scanf()함수 활용 정수 입력
 * 
 * @param prompt 메뉴 번호 선택 변수
 * @return int 선택한 메뉴 번호 반환
 */
int inputMeunNum(const char *prompt);

/**
 * @brief 날짜/시간 입력
 * 
 * @param dateTime 날짜/시간 문자열 반환
 */
void inputDateTime(char *dateTime);

/**
 * @brief 날짜/시간 유효성 검사
 * 
 * @param datetime 날짜/시간 문자열
 * @return int 
 */
int validDateTime(char *datetime);

/**
 * @brief 날짜별 기록 조회 입력 메뉴
 * 
 * @param date 기록을 획인할 날짜
 */
void viewRecordsByDate_IN_Menu(char *date);

#endif