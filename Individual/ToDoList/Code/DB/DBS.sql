/* CREATE TABLE schedules (
    id INTEGER PRIMARY KEY,
    title TEXT NOT NULL,
    scheduled_date_time TEXT,
	end_date_time TEXT,
    tag TEXT,
    priority INTEGER,
    status TEXT DEFAULT 'TODO'
); */

/* 
INSERT INTO schedules (
    title, 
    scheduled_date_time, 
    end_date_time, 
    tag, 
    priority
) 
VALUES ('과제 제출', '2025-04-21 23:59', NULL, '수업', 3),
('과제 제출1', '2025-04-21 09:00', NULL, '수업1', 3),
('과제 제출2', '2025-04-22 10:00', NULL, '수업2', 3),
('과제 제출3', '2025-04-23 11:00', NULL, '수업3', 3),
('과제 제출4', '2025-04-24 12:00', NULL, '수업4', 3),
('과제 제출5', '2025-04-25 13:00', NULL, '수업5', 3),
('과제 제출6', '2025-04-26 14:00', NULL, '수업6', 3),
('과제 제출7', '2025-04-27 15:00', NULL, '수업7', 3),
('과제 제출8', '2025-04-28 16:00', NULL, '수업8', 3),
('과제 제출9', '2025-04-29 17:00', NULL, '수업9', 3),
('과제 제출10', '2025-04-30 18:00', NULL, '수업10', 3);

SELECT * FROM schedules; 
*/

/* SELECT 
    ROW_NUMBER() OVER (ORDER BY scheduled_date_time) AS no,
    id,
    title,
    scheduled_date_time,
    tag,
    priority,
    status
FROM schedules
WHERE status = 'TODO';

SELECT id FROM (
    SELECT ROW_NUMBER() OVER (ORDER BY scheduled_date_time) AS no, 
    id FROM schedules WHERE status = 'TODO'
) WHERE no = 2;

UPDATE schedules 
SET title = '과제 제출2', scheduled_date_time = '2025-04-23 11:00', end_date_time = '2025-04-23 12:00', tag = '수업3', priority = 2
WHERE id = 1;

SELECT * FROM schedules;  */

/* UPDATE schedules
SET status = 'DONE'
WHERE id = 11;
UPDATE schedules
SET status = 'DOING'
WHERE id = 5;
UPDATE schedules
SET status = 'DOING'
WHERE id = 3; */

-- SELECT * FROM schedules;

/* SELECT 
    title,
    scheduled_date_time,
    end_date_time,
    tag,
    priority
FROM schedules
WHERE id = 2; */

/* SELECT ROW_NUMBER() OVER (ORDER BY scheduled_date_time) AS no, id, tag FROM schedules;

SELECT tag FROM (
    SELECT ROW_NUMBER() OVER (
        ORDER BY scheduled_date_time
    )
    AS no, 
    tag FROM schedules
) WHERE no = 2; */

/* INSERT INTO schedules (
    title, 
    scheduled_date_time, 
    end_date_time, 
    tag, 
    priority
) 
VALUES ('과제 제출13', '2025-04-21 23:59', NULL, '수업4', 3),
('과제 제출11', '2025-04-21 09:00', NULL, '수업1', 3),
('과제 제출12', '2025-04-22 10:00', NULL, '수업2', 3);

SELECT * FROM schedules;  */
/* 
SELECT tag, COUNT(*) AS count
FROM schedules
WHERE tag = '수업4'; */
/* 
UPDATE schedules
SET end_date_time = NULL
WHERE id = 15;
UPDATE schedules
SET end_date_time = NULL
WHERE id = 16;
UPDATE schedules
SET end_date_time = NULL
WHERE id = 17;
SELECT * FROM schedules; */

/* SELECT id FROM (
    SELECT ROW_NUMBER() OVER (
        ORDER BY scheduled_date_time) AS no, 
        id FROM schedules WHERE status = 'TODO'
    )
WHERE no = 1; */

/* UPDATE schedules
SET status = 'TODO'
WHERE id = 2;
 */

/* DELETE FROM schedules WHERE id = 15;

SELECT * FROM schedules; */

-- SELECT ROW_NUMBER() OVER (ORDER BY scheduled_date_time) AS no, tag FROM schedules;


/* SELECT ROW_NUMBER() OVER (ORDER BY tag) AS no, tag
FROM (SELECT DISTINCT tag FROM schedules);
 */

-- SELECT tag FROM (SELECT ROW_NUMBER() OVER (ORDER BY tag) AS no, tag FROM schedules) WHERE no = 1;

/* SELECT COUNT(*) AS count FROM schedules WHERE tag = '수업1';

SELECT * FROM schedules; */

/* SELECT tag FROM (SELECT ROW_NUMBER() OVER (ORDER BY tag) AS no, tag FROM schedules) WHERE no = 1;
SELECT COUNT(*) AS count FROM schedules WHERE tag = 'test2'; */

/* SELECT 
    temp_tag.tag, 
    (SELECT COUNT(*) FROM schedules WHERE tag = temp_tag.tag) AS count
FROM (
    SELECT tag 
    FROM (
        SELECT ROW_NUMBER() OVER (ORDER BY tag) AS no, tag 
        FROM schedules 
        WHERE tag IS NOT NULL
    ) 
    WHERE no = 4
) AS temp_tag; */

SELECT * FROM schedules;

SELECT ROW_NUMBER() OVER (ORDER BY tag) AS no, tag
FROM (SELECT DISTINCT tag FROM schedules WHERE tag IS NOT NULL);

SELECT COUNT(*) AS count FROM schedules WHERE tag = '수업2';

SELECT temp_tag.tag, (SELECT COUNT(*) FROM schedules WHERE tag = temp_tag.tag) AS count FROM (SELECT tag FROM (SELECT ROW_NUMBER() OVER (ORDER BY tag) AS no, tag FROM (SELECT DISTINCT tag FROM schedules WHERE tag IS NOT NULL)) WHERE no = 3) AS temp_tag;

SELECT tag FROM (SELECT ROW_NUMBER() OVER (ORDER BY tag) AS no, tag FROM (SELECT DISTINCT tag FROM schedules WHERE tag IS NOT NULL)) WHERE no = 3;