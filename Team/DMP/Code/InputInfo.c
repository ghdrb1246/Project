#include <stdlib.h>
#include "InputInfo.h"

UserSignupInfo *USImalloc() {
    UserSignupInfo *USI = (UserSignupInfo*)malloc(sizeof(UserSignupInfo));
    if (!USI) return NULL;

    USI->id = (char*)malloc(ID_SIZE * sizeof(char));
    USI->pw = (char*)malloc(PW_SIZE  * sizeof(char));
    USI->gender = (char*)malloc(GENDER_SIZE  * sizeof(char));
    
    return USI;
}
MealInputInfo *MIImalloc() {
    MealInputInfo *MII = (MealInputInfo*)malloc(sizeof(MealInputInfo));
    if (!MII) return NULL;

    MII->userId = (char*)malloc(ID_SIZE * sizeof(char));
    MII->dateTime = (char*)malloc(DATETIME_SIZE * sizeof(char));
    MII->foodName = (char*)malloc(FOODNAME_SIZE  * sizeof(char));

    return MII;
}
WorkOutInputInfo *WOIImalloc() {
    WorkOutInputInfo *WOII = (WorkOutInputInfo*)malloc(sizeof(WorkOutInputInfo));
    if (!WOII) return NULL;

    WOII->userId = (char*)malloc(ID_SIZE * sizeof(char));
    WOII->dateTime = (char*)malloc(DATETIME_SIZE * sizeof(char));
    WOII->exerciseName = (char*)malloc(EXERCISENAME_SIZE  * sizeof(char));

    return WOII;
}
WeightInputInfo *WIImalloc() {
    WeightInputInfo *WII = (WeightInputInfo*)malloc(sizeof(WeightInputInfo));
    if (!WII) return NULL;

    WII->userId = (char*)malloc(ID_SIZE * sizeof(char));
    WII->date = (char*)malloc(DATETIME_SIZE * sizeof(char));

    return WII;
}

void USIfree(UserSignupInfo *SUI) {
    if (!SUI) return;

    free(SUI->id);
    free(SUI->pw);
    free(SUI->gender);
    free(SUI);
}
void MIIfree(MealInputInfo *MII) {
    if (!MII) return;

    free(MII->userId);
    free(MII->dateTime);
    free(MII->foodName);
    free(MII);
}
void WOIIfree(WorkOutInputInfo *WOII) {
    if (!WOII) return;

    free(WOII->userId);
    free(WOII->dateTime);
    free(WOII->exerciseName);
    free(WOII);
}
void WIIfree(WeightInputInfo *WII) {
    if (!WII) return;

    free(WII->userId);
    free(WII->date);
    free(WII);
}