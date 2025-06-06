-- drop table user;
-- drop table meal;
-- drop table workout;
-- drop table weightReco/rd;

/* CREATE TABLE IF NOT EXISTS users (
    id TEXT PRIMARY KEY,
    pw TEXT NOT NULL,
    gender TEXT,
    age INTEGER
);

CREATE TABLE IF NOT EXISTS user_diets (
    user_id TEXT PRIMARY KEY, 
    height REAL, 
    initial_weight REAL, 
    goal_weight REAL, 
    FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE
); */

-- INSERT INTO user_diets (height, initial_weight, goal_weight) VALUES (160, 54, 50);

-- DELETE FROM users WHERE id = 'gh2';
/* 
CREATE TABLE IF NOT EXISTS meal (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id TEXT,
    datetime TEXT,
    food_name TEXT,
    gram REAL,
    kcal REAL,
    FOREIGN KEY (user_id) REFERENCES user(id) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS workout (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id TEXT,
    datetime TEXT,
    exercise_name TEXT,
    hour REAL,
    kcal REAL,
    FOREIGN KEY (user_id) REFERENCES user(id) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS weight (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id TEXT,
    date TEXT,
    weight REAL,
    FOREIGN KEY (user_id) REFERENCES user(id) ON DELETE CASCADE
); */


-- SELECT * FROM users;

/* INSERT INTO user(
    id, 
    pw, 
    gender, 
    age, 
    height, 
    current_weight, 
    goal_weight
)
VALUES('gh1', '1234', 'ska', 24, 160, 45, 42); */

-- DELETE FROM weight WHERE date = 'gh1';

/* CREATE TABLE IF NOT EXISTS weightRecord (
    user_id TEXT,
    date TEXT,            
    weight REAL,
    PRIMARY KEY (user_id, date),
    FOREIGN KEY (user_id) REFERENCES user(id) ON DELETE CASCADE
);

INSERT OR REPLACE INTO weightRecord (user_id, date, weight)
VALUES ('user1', '2025-06-07', 51.0);

SELECT * FROM weightRecord; */

-- SELECT weight FROM weightRecord WHERE user_id = 'gh1' ORDER BY date DESC LIMIT 1;

-- DELETE FROM user WHERE user_id = 'gh1';
-- DELETE FROM meal WHERE iuser_id = 'gh1';
-- DELETE FROM workout WHERE id = 'gh1';
-- DELETE FROM weightRecord WHERE user_id = 'gh1';

/* CREATE TABLE IF NOT EXISTS workout (
    id INTEGER PRIMARY KEY AUTOINCREMENT, 
    user_id TEXT,
    datetime TEXT,
    exercise_name TEXT,
    minutes REAL,
    kcal REAL,
    FOREIGN KEY (user_id) REFERENCES user(id) ON DELETE CASCADE
);
*/
 

-- INSERT INTO user (user_id, password, gender, age, height, exercise_weight, goal_weight) VALUES ('gh1', '1234', 'M', 24, 162, 50, 43);
-- INSERT INTO meal (user_id, datetime, food_name, gram, kcal) VALUES ('gh1', '2025-06-03 09:00', 'A', 100, 10);
-- INSERT INTO workout (user_id, datetime, exercise_name, minutes, kcal) VALUES ('gh1', '2025-06-02 09:00', '요가', 40, 10.32);
-- INSERT INTO workout (user_id, datetime, exercise_name, minutes, kcal) VALUES ('gh2', '2025-06-03 11:45', 'test', 41, 10.32);
-- INSERT INTO workout (user_id, datetime, exercise_name, minutes, kcal) VALUES ('gh2', '2025-06-02 12:45', 'test', 50, 10.32);
-- INSERT INTO workout (user_id, datetime, exercise_name, minutes, kcal) VALUES ('gh3', '2025-06-03 09:45', 'test', 40, 10.32);
-- INSERT INTO weightRecord (user_id, date, weight) VALUES ('gh1', '2025-06-03', 50);


-- DELETE FROM user WHERE user_id = 'gh3';
-- DELETE FROM meal WHERE user_id = 'gh3';
-- DELETE FROM workout WHERE user_id = 'gh3';
-- DELETE FROM weightRecord WHERE user_id = 'gh3';

-- DELETE FROM meal WHERE user_id = 'gh2'

-- SELECT time(datetime), food_name, gram, kcal FROM meal WHERE user_id = 'gh1' AND date(datetime) = '2025-06-02';
-- SELECT time(datetime), exercise_name, minutes, kcal FROM workout WHERE user_id = 'gh1' AND date(datetime) = '2025-06-02';
-- SELECT weight FROM weightRecord WHERE user_id = 'gh1' AND date = '2025-06-02';

-- SELECT exercise_weight, goal_weight FROM user WHERE user_id = 'gh1';
/* 
초기 체중: 80kg

목표 체중: 60kg

현재 체중: 75kg 
*/

-- INSERT INTO user (user_id, password, gender, age, height, exercise_weight, goal_weight) VALUES ('gh3', '1234', 'M', 24, 162, 80, 60);
-- INSERT INTO meal (user_id, datetime, food_name, gram, kcal) VALUES ('gh1', '2025-06-03 09:00', 'A', 100, 10);
-- INSERT INTO workout (user_id, datetime, exercise_name, minutes, kcal) VALUES ('gh1', '2025-06-02 09:00', '요가', 40, 10.32);
-- INSERT INTO weightRecord (user_id, date, weight) VALUES ('gh3', '2025-06-03', 75);

SELECT kcal FROM foods WHERE name = 'test';
