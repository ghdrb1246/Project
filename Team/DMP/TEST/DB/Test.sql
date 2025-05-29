CREATE TABLE IF NOT EXISTS users1(userId TEXT PRIMARY KEY, userPw TEXT NOT NULL);
CREATE TABLE IF NOT EXISTS users2(userId TEXT PRIMARY KEY, userPw TEXT NOT NULL);
INSERT INTO users1 (userId, userPw) VALUES ('d1f', '1246');
INSERT INTO users2 (userId, userPw) VALUES ('d1f', '1246');
-- SELECT * FROM users WHERE id = 'gh' AND pw = '1246';

SELECT * FROM users;