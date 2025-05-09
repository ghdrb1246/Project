#include "MumeIO.h"
#include "DB_Management.h"
/* #include <stdio.h>
#include "UserStructure.h"
#include "Utils.h" */

int main() {
   int test = DBO("DB/DBSC.db");
   if (test) return 1;

   main_meun();
   // DBC();
   return 0;
}