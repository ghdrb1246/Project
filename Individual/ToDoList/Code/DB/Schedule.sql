/*
CREATE TABLE schedules (
    id INTEGER PRIMARY KEY,
    title TEXT NOT NULL,
    scheduled_date_time TEXT,
	end_date_time TEXT,
    tag TEXT,
    priority INTEGER,
    status TEXT DEFAULT 'TODO'
);
*/
/*
INSERT INTO schedules (
    title, 
    scheduled_date_time, 
    end_date_time, 
    tag, 
    priority
)
VALUES (
    '과제 제출', 
    '2025-04-21 23:59', 
    NULL, 
    '수업', 
    3
);

-- SELECT * FROM schedules;
 
INSERT INTO schedules (
    title, 
    scheduled_date_time, 
    end_date_time, 
    tag, 
    priority
) VALUES (
    'JAVA 시험', 
    '2025-04-28 09:00', 
    NULL, 
    '시험', 
    3
);
 
INSERT INTO schedules (
    title, 
    scheduled_date_time, 
    end_date_time, 
    tag, 
    priority
) VALUES (
    'JAVA 시험', 
    '2025-04-28 09:00', 
    NULL, 
    '시험', 
    3
);

SELECT * FROM schedules;
*/

/* 
DELETE FROM schedules;
SELECT * FROM schedules;
*/
/* UPDATE schedules
SET id = 3
WHERE id = 5;

SELECT * FROM schedules;
 */

/*  
DELETE FROM schedules;
SELECT * FROM schedules;
 */