#ifndef _CSVDB_
#define _CSVDB_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "sqlite/sqlite3.h"

//운동 CSV 파일 경로
#define EXERCISES_CSV_FILE "Server/CSV/exercise.csv"

// 운동 DB 파일 경로
#define EXERCISES_DB_FILE "Server/DB/exercise.db"

// 음식 CSV 파일 경로
#define FOOD_CSV_FILE "Server/CSV/food.csv"

// 음식 DB 파일 경로
#define FOOD_DB_FILE "Server/DB/food.db"

/**
 * @brief CSV 파일과 DB 파일의 최신화 여부를 검사하는 함수
 *
 * @param csv 최신화 여부를 검사할 CSV 파일 경로
 * @param db 변환된 DB 파일 경로
 * @return 변환이 필요하면 1, 변환이 불필요하면 0 반환
 *
 * @note - CSV 파일과 DB 파일의 마지막 수정 시간을 비교
 * @note - CSV가 DB보다 최신이면 변환 필요 (1 반환)
 * @note - 반대로 DB가 최신이면 변환 불필요 (0 반환)
 */
int needConvert(const char *csv, const char *db);

/**
 * @brief 운동 CSV 파일을 읽어 DB로 변환하는 함수
 *
 * @note - exercise.csv를 파싱하여 운동명, MET 데이터를 읽음
 * @note - 읽어온 데이터를 exercise.db (SQLite)로 변환 저장
 * @note - 기존 DB는 덮어쓰거나 갱신 (최신화 처리)
 */
void exerciseConvertCSVtoDB();

/**
 * @brief 입력받은 운동명을 기준으로 DB에서 MET 값을 조회하는 함수
 *
 * @param exercise 사용자 입력 운동명
 * @return 해당 운동의 MET 값, 없으면 -1 반환
 *
 * @note - exercise.db에서 운동명을 검색
 * @note - 존재하면 MET 값을 반환
 * @note - 검색 실패 시 -1 반환
 */
float inputWorkoutAndCalc(char *exercise);

/**
 * @brief MET 값을 기반으로 운동 칼로리를 계산하는 함수
 *
 * @param met 운동의 MET 값 (운동 강도 지수)
 * @param minutes 운동 시간 (분 단위)
 * @param user_weight 사용자 체중 (kg)
 * @return 운동의 소모 칼로리 (kcal)
 * 
 * @note - 시간(h) = 분 / 60.0f
 * @note - MET 공식: kcal = MET * 체중(kg) * 시간(h)
 */
float METM(float met, float minutes, float user_weight);

/**
 * @brief 음식 CSV 파일을 읽어 DB로 변환하는 함수
 *
 * @note - food.csv를 파싱하여 음식명, 칼로리 데이터를 읽음
 * @note - 읽어온 데이터를 food.db (SQLite)로 변환 저장
 * @note - 기존 DB는 덮어쓰거나 갱신 (최신화 처리)
 */
void foodConvertCSVtoDB();

/**
 * @brief 입력받은 음식명을 기반으로 DB에서 kcal 값을 조회하는 함수
 *
 * @param food 사용자 입력 음식명
 * @return 음식의 kcal 값, 없으면 -1 반환
 *
 * @note - foods.db에서 음식명을 검색
 * @note - 존재하면 kcal 값을 반환
 * @note - 검색 실패 시 -1 반환
 */
float inputFoodAndCalc(char *food);

#endif