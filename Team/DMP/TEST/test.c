#include <stdio.h>
#include<string.h>
#include "UserInfo.h"

void inputLine(const char *prompt, char *buf, int size);
void signupMenu(UserSignupInfo *info);

int main() {
    // 구조체 메모리 할당
    UserSignupInfo *info = USImalloc();
    if (!info) {
        printf("메모리 할당 실패!\n");
        return 1;
    }

    // 사용자 입력 받기
    signupMenu(info);

    // 입력받은 값 출력 (확인용)
    printf("==== 입력된 정보 ====\n");
    printf("ID: %s\n", info->id);
    printf("PW: %s\n", info->pw);
    printf("성별: %s\n", info->gender);
    printf("나이: %d\n", info->age);
    printf("키: %.1f\n", info->height);
    printf("초기 체중: %.1f\n", info->initialWeight);
    printf("목표 체중: %.1f\n", info->goalWeight);

    // 메모리 해제
    USIfree(info);

    return 0;
}

// 안전하게 문자열 입력을 위한 함수
void inputLine(const char *prompt, char *buf, int size) {
    printf("%s", prompt);
    if (fgets(buf, size, stdin) != NULL) {
        buf[strcspn(buf, "\n")] = '\0';  // 개행 문자 제거
    }
}
void signupMenu(UserSignupInfo *USI) {
    printf("회원가입\n");
    printf("\n");
    // P_MENU_TITLE("회원가입");
    // P_MENU_IN;
    // P_MENU_SB_S("입력", "----");
    
    inputLine("ID 입력: ", USI->id, ID_SIZE);
    inputLine("PW 입력: ", USI->pw, PW_SIZE);
    inputLine("성별 (남/여): ", USI->gender, GENDER_SIZE);
    // getchar();
    
    printf("나이: ");
    scanf("%d", &USI->age); getchar();

    printf("키 (cm): ");
    scanf("%f", &USI->height); getchar();
    
    printf("초기 체중 (kg): ");
    scanf("%f", &USI->initialWeight); getchar();
    
    printf("목표 체중 (kg): ");
    scanf("%f", &USI->goalWeight); getchar();

    // P_MENU_IN;
    // P_MENU_END;
}