#include "MumeIO.h"
#include "DB_Management.h"

int main() {
   int test = 1;

    while (test) {
        test = DBO("DB/DBSC.db");
    }

    main_meun();

    DBC();
}