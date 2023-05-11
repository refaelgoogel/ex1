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
void destroyEnrollmentSystem(enrollmentSystem sys);
void SetNameFlag(enrollmentSystem sys, bool flag);
void PrintSystem(enrollmentSystem sys);


#endif // HACKER_ENROLLMENT_H