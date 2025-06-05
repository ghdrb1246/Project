#ifndef USER_INFO_H
#define USER_INFO_H

#define ID_SIZE 50
#define PW_SIZE 50
#define GENDER_SIZE 5
#define DATETIME_SIZE 17
#define FOODNAME_SIZE 50
#define EXERCISENAME_SIZE 50
#define BUF_SIZE 4096
#define PORT 12345

// #define SEND_BUFFER_SIZE 1024
// #define RECV_BUFFER_SIZE 4096

// 회원가입 구조체
/**
 * @brief 회원가입 사용자 정보
 * 
 * @param id id
 * @param pw pw
 * @param gender 남/여
 * @param height 키
 * @param exerciseWeight 현재 체중
 * @param goalWeight 목표 체중
 */
typedef struct {
    char *id;
    char *pw;
    char *gender;
    int age;
    float height;
    float exerciseWeight;
    float goalWeight;
} UserSignupInfo;


// 식단 입력 구조체
/**
 * @brief 식단 정보
 * 
 * @param userId 사용자 id
 * @param dateTime 날짜/시간
 * @param foodName 음식명
 * @param gram 음식량(g)
 * @param kcal 음식 칼로리
 * 
 */
typedef struct {
    char *userId;
    char *dateTime;
    char *foodName;
    float gram;
    float kcal;
} MealInputInfo;

// 운동 입력 구조체
/**
 * @brief 식단 정보
 * 
 * @param userId 사용자 id
 * @param dateTime 날짜/시간
 * @param exerciseName 운동명
 * @param minutes 운동 시간(m)
 * @param kcal 운동 칼로리
 * 
 */
typedef struct {
    char *userId;
    char *dateTime;
    char *exerciseName;
    float minutes;
    float kcal;
} WorkOutInputInfo;

// 체중 입력 구조체
/**
 * @brief 체중 정보
 * 
 * @param userId 사용자 id
 * @param dateTime 날짜/시간
 * @param weight 체중 
 * 
 */
typedef struct {
    char *userId;
    char *date;
    float weight;
} WeightInputInfo;

/**
 * @brief UserSignupInfo 구조체 동적할당
 * 
 * @return UserSignupInfo*
 */
UserSignupInfo *USImalloc();

/**
 * @brief MealInputInfo 구조체 동적할당
 * 
 * @return MealInputInfo*
 */
MealInputInfo *MIImalloc();

/**
 * @brief WorkOutInputInfo 구조체 동적할당
 * 
 * @return WorkOutInputInfo*
 */
WorkOutInputInfo *WOIImalloc();

/**
 * @brief WeightInputInfo 구조체 동적할당
 * 
 * @return WeightInputInfo*
 */
WeightInputInfo *WIImalloc();

/**
 * @brief UserSignupInfo 구조체 동적할당 해제
 * 
 * @param SUI 동적할당 해제할 구조체
 */
void USIfree(UserSignupInfo *SUI);

/**
 * @brief MealInputInfo 구조체 동적할당 해제
 * 
 * @param MII 동적할당 해제할 구조체
 */
void MIIfree(MealInputInfo *MII);

/**
 * @brief WorkOutInputInfo 구조체 동적할당 해제
 * 
 * @param WOII 동적할당 해제할 구조체
 */
void WOIIfree(WorkOutInputInfo *WOII);

/**
 * @brief WeightInputInfo 구조체 동적할당 해제
 * 
 * @param WII 동적할당 해제할 구조체
 */
void WIIfree(WeightInputInfo *WII);

#endif