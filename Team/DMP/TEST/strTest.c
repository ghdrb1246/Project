#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void inputLine(const char *prompt, char *buf, int size);

int main() {
    char *str = (char*)malloc(50 * sizeof(char));

    inputLine("문자입력 : ", str, 50);
    if (strcspn(str, "\n") != strlen(str)) str[strcspn(str, "\n")] = '\0';
    printf("문자입력 : %s - 공백 ? : %s(길이|%lu, 공백 위치|%lu)\n", str, (strcspn(str, "\n") != strlen(str)) ? "있음" : "없음", strlen(str), strcspn(str, "\n"));
    
    return 0;
}

void inputLine(const char *prompt, char *buf, int size) {
    printf("%s", prompt);

    fgets(buf, size, stdin);
    buf[strcspn(buf, "\n")] = '\0'; 
}