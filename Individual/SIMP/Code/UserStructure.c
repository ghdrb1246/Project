#include <stdlib.h>
#include "UserStructure.h"

// 연거체, 구조체 정의

Schedule *smalloc() {
    Schedule *s = (Schedule*)malloc(sizeof(Schedule));
    if (!s) return NULL;
    
    s->title = (char*)malloc(100);
    s->scheduled_date_time = (char*)malloc(20);
    s->end_date_time = (char*)malloc(20);
    s->tag = (char*)malloc(10);
    s->status = (char*)malloc(5);

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

    tc->tag = (char*)malloc(10 * sizeof(char));

    return tc;
}

void tcfree(TagCount *tc) {
    if (!tc) return;

    free(tc->tag);
    free(tc);
}