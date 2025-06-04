#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    const char *tableNeame[4] = { "meal", "workout", "weightRecord", "user" };
    int conut = 0;

    for (int i = 0; i < 4; i++) {
        char *sql = (char*)malloc((strlen(tableNeame[i]) + 32) * sizeof(char));
        snprintf(sql, (strlen(tableNeame[i]) + 32), "DELETE FROM %s WHERE user_id = ?;", tableNeame[i]);
        printf("%12s -> %s\n", tableNeame[i], sql);
    }

    return 0;
}