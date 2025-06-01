#include <stdio.h>
#include <string.h>

int main() {
    char request[50] = "INPUT_MEAL/2025-06-01 09:00/apple/120.0";
    char s1[50], s2[50];
    float f1;

    sscanf(request, "INPUT_MEAL/%16[^/]/%49[^/]/%f", s1, s2, &f1);

    printf("%s | ", s1); printf("%lu\n", strlen(s1));
    printf("%s | ", s2); printf("%lu\n", strlen(s2));
    printf("%f\n", f1);

    return 0;
}