#ifndef HACKER_H
#define HACKER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "Student.h"


typedef struct Hacker {

    Student hacker;
    char **wantedCoursesID;
    Student *friends;
    int numberFriend;
    Student *rivals;
    int numberRival;
    int wantedCoursesNum;
    int gotCourseNum;
    
} *Hacker;

Hacker HackerCreate(Student *students,char **fileLine);
void HackerDestroy(Hacker curseQueue);

#endif