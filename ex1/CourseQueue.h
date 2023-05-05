#ifndef COURSE_QUEUE_H
#define COURSE_QUEUE_H

#include "IsraeliQueue.c"
#include "Student.h"

typedef struct CourseQueue {

    IsraeliQueue studentQueue;
	char *courseID; 
    int courseSize;

} *CourseQueue;

CourseQueue CourseQueueCreate(char *fileLine);
CourseQueue InsertStudentsToCourseQueue(CourseQueue courseQueue, char *fileLine, Student *students);
void CourseQueueDestroy(CourseQueue curseQueue);

#endif