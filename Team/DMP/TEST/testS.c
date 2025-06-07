#include <stdio.h>
#include <string.h>

int main() {
    // 예제 데이터
    char data[] = "MEAL:사과 100g|바나나 200g#WORKOUT:달리기 30분|걷기 20분#WEIGHT:70kg";

    // 3개의 문자열로 구간별 복사
    char mealData[] = "";
    char workoutData[256] = "";
    char weightData[256] = "";

    // 첫 번째 레벨: '#'로 구분
    char *section = strtok(data, "#");
    int sectionCount = 0;

    while (section != NULL) {
        if (sectionCount == 0) {
            strncpy(mealData, section, sizeof(mealData)-1);
        } else if (sectionCount == 1) {
            strncpy(workoutData, section, sizeof(workoutData)-1);
        } else if (sectionCount == 2) {
            strncpy(weightData, section, sizeof(weightData)-1);
        }
        sectionCount++;

        section = strtok(NULL, "#");
    }

    // -------------------------------
    // 식단 데이터 출력
    if (strlen(mealData) > 0) {
        printf("식단 ----\n");
        // ':' 이후 데이터만 분리
        char *dataPart = strchr(mealData, ':');
        if (dataPart) {
            dataPart++;  // ':' 넘기기
            char *entry = strtok(dataPart, "|");
            int mealCount = 1;
            while (entry) {
                printf("%d. %s\n", mealCount++, entry);
                entry = strtok(NULL, "|");
            }
        }
        printf("-------------------------------\n");
    }

    // 운동 데이터 출력
    if (strlen(workoutData) > 0) {
        printf("운동 ----\n");
        char *dataPart = strchr(workoutData, ':');
        if (dataPart) {
            dataPart++;
            char *entry = strtok(dataPart, "|");
            int workoutCount = 1;
            while (entry) {
                printf("%d. %s\n", workoutCount++, entry);
                entry = strtok(NULL, "|");
            }
        }
        printf("-------------------------------\n");
    }

    // 체중 데이터 출력
    if (strlen(weightData) > 0) {
        printf("체중 ----\n");
        char *dataPart = strchr(weightData, ':');
        if (dataPart) {
            dataPart++;
            printf("%s\n", dataPart);
        }
        printf("-------------------------------\n");
    }

    return 0;
}
