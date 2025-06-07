#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "sqlite/sqlite3.h"

#define EXERCISES_CSV_FILE "Server/CSV/exercise.csv"
#define EXERCISES_DB_FILE "Server/DB/exercise.db"
#define FOOD_CSV_FILE "Server/CSV/food.csv"
#define FOOD_DB_FILE "Server/DB/food.db"

int needConvert(const char *csv, const char *db);
void exercisesConvertCSVtoDB();
float inputWorkoutAndCalc(char *exercise);
float METM(float met, float minutes, float user_weight);

void foodConvertCSVtoDB();
float inputFoodAndCalc(char *food);