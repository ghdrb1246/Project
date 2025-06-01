-- drop table users;

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

DELETE FROM users WHERE id = 'gh2';

SELECT * FROM users;