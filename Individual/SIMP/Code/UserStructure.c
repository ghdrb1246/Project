#include <stdlib.h>
#include "UserStructure.h"

// 연거체, 구조체 정의

Schedule *smalloc() {
    Schedule *s = (Schedule*)malloc(sizeof(Schedule));
    if (!s) return NULL;

    s->title = (char*)malloc(TITLE_SIZE * sizeof(char));
    s->scheduled_date_time = (char*)malloc(DT_SIZE  * sizeof(char));
    s->end_date_time = (char*)malloc(DT_SIZE  * sizeof(char));
    s->tag = (char*)malloc(TAG_SIZE  * sizeof(char));
    s->status = (char*)malloc(STATUS_SIZE  * sizeof(char));

    return s;
}

void sfree(Schedule *s) {
    if (!s) return;

    free(s->title);
    free(s->scheduled_date_time);
    free(s->end_date_time);
    free(s->tag);
    free(s->status);
    free(s);
}

TagCount *tcmalloc() {
    TagCount *tc = (TagCount*)malloc(sizeof(TagCount));
    if (!tc) return NULL;

    tc->tag = (char*)malloc(TAG_SIZE * sizeof(char));

    return tc;
}

void tcfree(TagCount *tc) {
    if (!tc) return;

    free(tc->tag);
    free(tc);
}