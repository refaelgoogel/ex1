#ifndef HACKER_ENROLLMENT_H
#define HACKER_ENROLLMENT_H

#include "CourseQueue.h"
#include "Hacker.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct EnrollmentSystem
{
    FILE* students_file;
    FILE* courses_file; 
    FILE* hackers_file;
	CourseQueue *coursesQueue;
    Student *students;
    Hacker *hackers;
    int numberHackers;
    int numberStudents;
    int numberCourses;
    bool nameFlag;
    
} *enrollmentSystem;


typedef enum { EnrollmentSystem_SUCCESS, EnrollmentSystem_ALLOC_FAILED, EnrollmentSystem_FILE_ACCESS_FAILED, EnrollmentSystem_BAD_PARAM, EnrollmentSystem_ERROR } enrollmentSystemError;

/**Error clarification:
 * EnrollmentSystem_SUCCESS: Indicates the function has completed its task successfully with no errors.
 * EnrollmentSystem_ALLOC_FAILED: Indicates memory allocation failed during the execution of the function.
 * EnrollmentSystem_FILE_ACCESS_FAILED: Indicates file access failed during the execution of the function.
 * EnrollmentSystem_BAD_PARAM: Indicates an illegal parameter was passed.
 * EnrollmentSystem_ERROR: Indicates any error beyond the above.
 * */


enrollmentSystem createEnrollmentSystem(FILE* students, FILE* courses, FILE* hackers);
enrollmentSystem readEnrollment(enrollmentSystem sys, FILE* queues); 
void hackEnrollment(enrollmentSystem sys, FILE* out); 


// maybe required to implement
enrollmentSystemError AddingNewStudent(int id, int credits, double gpa, char* name, char* surname, char* city, char* department);
bool ifStudentExist(int id); 
bool ifCourseExist(int courseNumber);
bool ifHackerGetCourse(CourseQueue courseQueue ,Hacker h);
int returnIndexOfCourseByID(CourseQueue *courseQueue, char *courseID, int coursesNum);
int comparisonRequireToAccept(int requireCourses, int accepteCourses);
bool getHackerInToTheRequiredCourses(CourseQueue *courseQueue, Hacker hacker,char** wantedCourseID, int wantedCoursesNum, int coursesNum);
void SetNameFlag(enrollmentSystem sys, bool flag);

int friendshipMeasureByFile(void *hacker, void* student);
int friendshipMeasureByID(void *hacker, void* student);
int friendshipMeasureByName(void *hacker, void* student);
int friendshipMeasureByNameWithFlag(void *hacker, void* student);
int ItemsComparisonFunction(void *student1, void *student2);
CourseQueue* readFileCourses(char** coursesLines, int coursesNum, CourseQueue *coursesQueue);
Student* readFileStudent(char** studentLines, int studentNum, Student *students);
Hacker* readFileHackers(char** hackerLines, int hackerNum, Hacker *hackers, Student *students, int studentNum);
int returnDiffAsciiName(char* hackerName, char* studentName, bool capitalLetter);
int upperToLower(int a);


#endif // HACKER_ENROLLMENT_H