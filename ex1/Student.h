#ifndef STUDENT_H
#define STUDENT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <math.h>

typedef struct Student {

    char* studentID;
    int totalCredits;
    int gpa;
    char* name;
    char* surname;
    char* city;
    char* department;
    bool ifHacker;
    char** friendsID;
    int numberFriend;
    char** rivalsID;
    int numberRival;

} *Student;

Student studentCreate(char *fileLine);
void PrintStudent(Student student);
void studentDestroy(Student student);

#endif