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

Hacker HackerCreate(Student *students,char **fileLine, int studentNum);
int GetNumberSafe(char *token);
Hacker FirstLineFunction(Hacker new_hacker ,char *firstLine,int studentNum, Student *students);
Hacker SecondLineFunction(Hacker new_hacker ,char *secondLine);
Hacker ThirdLineFunction(Hacker new_hacker ,char *thirdLine, Student *students);
Hacker FourthLineFunction(Hacker new_hacker ,char *fourthLine, Student *students);
void HackerDestroy(Hacker curseQueue);
void FixToken(char *token);
void PrintHacker(Hacker hacker);


#endif