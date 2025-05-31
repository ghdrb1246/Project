CREATE TABLE IF NOT EXISTS user_diets (
    user_id TEXT PRIMARY KEY, 
    height REAL, 
    initial_weight REAL, 
    goal_weight REAL, 
    FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE
);

-- INSERT INTO user_diets (height, initial_weight, goal_weight) VALUES (160, 54, 50);

SELECT * FROM users