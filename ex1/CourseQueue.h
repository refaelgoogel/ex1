#ifndef COURSE_QUEUE_H
#define COURSE_QUEUE_H

#include "Student.h"
#include "IsraeliQueue.h"

typedef struct CourseQueue {

    IsraeliQueue studentQueue;
	char *courseID; 
    int courseSize;
    int currentSize;

} *CourseQueue;

CourseQueue CourseQueueCreate(char *fileLine);
CourseQueue InsertStudentsToCourseQueue(CourseQueue courseQueue, char *fileLine, Student *students);
void CourseQueueDestroy(CourseQueue curseQueue);

#endif