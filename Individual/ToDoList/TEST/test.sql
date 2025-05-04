/* CREATE TABLE Members (
	code INTEGER PRIMARY KEY AUTOINCREMENT,
	id TEXT(12),
	pw TEXT(18),
	fname TEXT,
	lname TEXT,
	ph TEXT,
	email TEXT,
	disabled NUMERIC
);
 
INSERT INTO Members
(code, id, pw, fname, lname, ph, email, disabled)
VALUES
(1, 'aaa', '1111', 'Richard', 'Choi', '010-1234', 'abcd@a.com', 0),
(2, 'bbb', '222', 'Olivia', 'Kim', '017-9999', 'zz@b.com', 0),
(3, 'ccc', '33', 'Rose', 'Park', '019-6788', 'vvv@c.com', 1),
(4, 'd', '4', 'C++', 'Lee', '054-123', 'a@a.com', 0),
(5, 'e', '55', 'Python', 'Ryu', '02-88', 'b@b.com', 0),
(6, 'f', '666', 'Java', 'Jo', '070-876', 'c@c.com', 1); */

-- SELECT * FROM Members;