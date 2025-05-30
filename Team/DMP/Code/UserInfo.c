#include <stdlib.h>
#include "UserInfo.h"

UserSignupInfo *USImalloc() {
    UserSignupInfo *USI = (UserSignupInfo*)malloc(sizeof(UserSignupInfo));
    if (!USI) return NULL;

    USI->id = (char*)malloc(ID_SIZE * sizeof(char));
    USI->pw = (char*)malloc(PW_SIZE  * sizeof(char));
    USI->gender = (char*)malloc(GENDER_SIZE  * sizeof(char));

    return USI;
}

void USIfree(UserSignupInfo *SUI) {
    if (!SUI) return;

    free(SUI->id);
    free(SUI->pw);
    free(SUI->gender);
    free(SUI);
}