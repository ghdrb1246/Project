#ifndef USER_INFO_H
#define USER_INFO_H

#define ID_SIZE 50
#define PW_SIZE 50
#define GENDER_SIZE 5

struct UserSignupInfo {
    char *id;
    char *pw;
    char *gender;
    int age;
    float height;
    float initialWeight;
    float goalWeight;
};
/**
 * @brief 회원가입 사용자 정보
 * 
 * @param id id
 * @param pw pw
 * @param gender 남/여
 * @param height 키
 * @param initialWeight 초기 체중
 * @param goalWeight 목표 체중
 */
typedef struct UserSignupInfo UserSignupInfo;

/**
 * @brief UserSignupInfo 구조체 동적할당
 * 
 * @return UserSignupInfo*
 */
UserSignupInfo *USImalloc();

/**
 * @brief UserSignupInfo 구조체 동적할당 해제
 * 
 * @param SUI 동적할당 해제할 구조체
 */
void USIfree(UserSignupInfo *SUI);

#endif