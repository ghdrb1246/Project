CREATE TABLE IF NOT EXISTS users(userId TEXT PRIMARY KEY, userPw TEXT NOT NULL);
INSERT INTO users (userId, userPw) VALUES ('d1f', '1246');
-- SELECT * FROM users WHERE id = 'gh' AND pw = '1246';

SELECT * FROM users;