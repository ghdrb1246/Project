-- drop table user;
-- drop table meal;
-- drop table workout;
-- drop table weightRecord;

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

CREATE TABLE IF NOT EXISTS weightRecord (
    user_id TEXT,
    date TEXT,            
    weight REAL,
    PRIMARY KEY (user_id, date),
    FOREIGN KEY (user_id) REFERENCES user(id) ON DELETE CASCADE
);

INSERT OR REPLACE INTO weightRecord (user_id, date, weight)
VALUES ('user1', '2025-06-07', 51.0);

SELECT * FROM weightRecord;