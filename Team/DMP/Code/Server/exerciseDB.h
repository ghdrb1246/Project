#define CSV_FILE "Server/CSV/exercises.csv"
#define DB_FILE "Server/DB/exercise.db"

int needConvert(const char *csv, const char *db);
void convertCSVtoDB();
float inputWorkoutAndCalc(char *exercise);
float METM(float met, float minutes, float user_weight);