#define _GNU_SOURCE
#include "HackerEnrollment.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


#define FRIENDSHIP_THRESHOLD 20
#define RIVAL_THRESHOLD -20
#define NEUTRAL_THRESHOLD 0


int friendshipMeasureByFile(void *hacker, void* student);
int friendshipMeasureByID(void *hacker, void* student);
int friendshipMeasureByName(void *hacker, void* student);
int ItemsComparisonFunction(void *student1, void *student2);
CourseQueue* readFileCourses(FILE *coursesFile, int coursesNum, CourseQueue *coursesQueue);
Student* readFileStudent(FILE *studentFile, int studentNum, Student *students);
Hacker* readFileHackers(FILE *hackerFile, int HackerNum, Hacker *hackers, Student *students);
int returnDiffAsciiName(char* hackerName, char* studentName);
int GetLinesNum(FILE *file);
int min(int a, int b);
Student ReturnStudentFromQueueByIndex(CourseQueue courseQueue, int index);

enrollmentSystem createEnrollmentSystem(FILE* students, FILE* courses, FILE* hackers){

    enrollmentSystem new_system = (enrollmentSystem)malloc(sizeof(*new_system));

    if (new_system == NULL){

        printf("ERROR: inside createEnrollmentSystem line 32\n");
        return NULL;
    }

    // malloc OK

    new_system->courses_file = courses;
    new_system->students_file = students;
    new_system->hackers_file = hackers;
   
    int numberCourses = GetLinesNum(new_system->courses_file);
    int numberStudents = GetLinesNum(new_system->students_file);

    int numberHackers = GetLinesNum(new_system->hackers_file);
    numberHackers = numberHackers/4; // because each hacker has 4 lines

    new_system->coursesQueue = (CourseQueue*)malloc(sizeof(CourseQueue)*(numberCourses+1));

    if (new_system->coursesQueue == NULL){

        printf("ERROR: inside createEnrollmentSystem line 52\n");
        return NULL;
    }

    new_system->coursesQueue[numberCourses] = NULL;

    // creting CourseQueue

    new_system->coursesQueue = readFileCourses(new_system->courses_file, numberCourses, new_system->coursesQueue);

    if (new_system->coursesQueue == NULL){

        printf("ERROR: inside createEnrollmentSystem line 64\n");
        return NULL;
    }

    // NOT creating the FriendshipFunctionArr inside the courseQueue because we dont need it now (we need it only in the readEnrollment function)


    // creating the comparison function inside the courseQueue (argument of IsraeliQueueCreate)

    ComparisonFunction comp = &ItemsComparisonFunction;

    // creating the israeliQueue inside the courseQueue 

    for (int i = 0; i < numberCourses; i++){

        new_system->coursesQueue[i]->studentQueue = IsraeliQueueCreate(NULL, comp, FRIENDSHIP_THRESHOLD, RIVAL_THRESHOLD);

        if (new_system->coursesQueue[i]->studentQueue == NULL){

            printf("ERROR: inside createEnrollmentSystem line 82, round %d\n",i);
            return NULL;
        }

    }

    // finished creating CourseQueue

    
    // creating the students array inside the enrollmentSystem

    new_system->students = (Student*)malloc(sizeof(Student)*(numberStudents+1));

    if (new_system->students == NULL){

        printf("ERROR: inside createEnrollmentSystem line 98\n");
        return NULL;
    }

    new_system->students[numberStudents] = NULL;
    new_system->students = readFileStudent(new_system->students_file, numberStudents, new_system->students);

    if (new_system->students == NULL){

        return NULL;
    }

    new_system->hackers = (Hacker*)malloc(sizeof(Hacker)*(numberHackers+1));

    if (new_system->hackers == NULL){

        printf("ERROR: inside createEnrollmentSystem line 114\n");
        return NULL;
    }

    new_system->hackers[numberHackers] = NULL;

    // malloc OK
    
    new_system->hackers = readFileHackers(new_system->hackers_file, numberHackers,new_system->hackers ,new_system->students);

    if (new_system->hackers == NULL){

        printf("ERROR: inside createEnrollmentSystem line 126\n");
        return NULL;
    }

    new_system->nameFlag = false;

    return new_system;
     
 }

 CourseQueue* readFileCourses(FILE* coursesFile, int coursesNum, CourseQueue *coursesQueue){

  if (coursesFile == NULL || coursesQueue == NULL) 
  {
    printf("ERROR: inside readFilesCourses line 140\n");
    return NULL;
  }

  char *fileLine = NULL;
  size_t len = 0; // size_t is an unsigned integer data type

  //fopen(coursesFile, "r");

  if (coursesFile == NULL){

    printf("ERROR: inside readFilesCourses line 151\n");
    return NULL;
  }

  for (int i = 0; i < coursesNum; i++){
      
    if (getline(&fileLine, &len, coursesFile) != -1){

      coursesQueue[i] = CourseQueueCreate(fileLine);
      fileLine = NULL;
      len = 0;
    
    }else {

       printf("ERROR: inside readFilesCourses line 165 round %d\n",i);
      return NULL;
    }
  }

    free(fileLine); // COULD BE ERROR: maybe not need to free the memory - still need to check

    return coursesQueue;

 }


 void SetNameFlag(enrollmentSystem sys, bool flag){

    if (sys == NULL){

        return;
    }

    sys->nameFlag = flag;

 }

Student* readFileStudent(FILE *studentFile, int studentNum, Student *students){

  if (studentFile == NULL || students == NULL) 
  {
      return NULL;
  }

  char *fileLine = NULL;
  size_t len = 0;

  //fopen(studentFile, "r");
  
  if (studentFile == NULL){
    
    return NULL;
  }

  for (int i = 0; i < studentNum; i++){
      
    if ((getline(&fileLine, &len, studentFile)) != -1){

      students[i] = studentCreate(fileLine);
      fileLine = NULL;
      len = 0;

    }else{

      return NULL;
    } 

   }

   free(fileLine);// COULD BE ERROR: maybe not need to free the memory - still need to check
   return students;
 }

Hacker* readFileHackers(FILE *hackerFile, int hackerNum, Hacker *hackers, Student *students){

    if (hackerFile == NULL || hackers == NULL){

        return NULL;
    }

    char *fileLine = NULL;
    size_t len = 0;

    char *hackerLines[5] = {NULL};

    //fopen(hackerFile, "r");

    for (int i = 0; i < hackerNum; i++){ // hackerNum divided by 4 because each hacker has 4 lines
      
        for (int j=0; j<4; j++){

            if ((getline(&fileLine, &len, hackerFile)) != -1){

                hackerLines[j] = fileLine;
                fileLine = NULL;
                len = 0;

            }else {

                return NULL;
            }

        } 

        hackers[i] = HackerCreate(students,hackerLines);

        if (hackers[i] == NULL){

            return NULL;
        }

    }

    free(fileLine); // do we need to free the memory? still need to check
    return hackers;

 }

 int GetLinesNum(FILE *file){
	
	char *line = NULL;
    size_t len = 0;
    int read = 0;
	int counter = 0; 

    while ((read = getline(&line, &len, file)) != -1) {
		
		// exist line 

        if (strlen(line) == 0){

          break;

        }


        counter++;

    }
	
	return counter;
}



// The function reads a file describing the registration queues for courses. The file format: 
//<Course Number> <Student IDs>* and will return an EnrollmentSystem structure representing the registration queues
enrollmentSystem readEnrollment(enrollmentSystem sys, FILE* queues){

    if (sys == NULL || queues == NULL){

        return NULL;
    }

    char *fileLine = NULL;
    size_t len = 0;

    // this is for the israeliqueue for after the regular adding students to the course
    FriendshipFunction *FunctionArr = (FriendshipFunction*)malloc(sizeof(FunctionArr)*4);

    if (FunctionArr == NULL){

        return NULL;
    }

    FunctionArr[0] = &friendshipMeasureByFile;
    FunctionArr[1] = &friendshipMeasureByID;
    FunctionArr[2] = &friendshipMeasureByName;
    FunctionArr[3] = NULL;
    // end of friendship function array

    // now we will read the file and insert the students to the courses
    while ((getline(&fileLine, &len, queues)) != -1){


        if (strlen(fileLine) == 0){

          break;
        }

        char *courseNum = strtok(fileLine, " ");

        //int i = 0;
        
        int indexOfCourse = returnIndexOfCourseByID(sys->coursesQueue, courseNum);

        if (indexOfCourse == -1){

            return NULL;
        }
        
        if (InsertStudentsToCourseQueue(sys->coursesQueue[indexOfCourse], fileLine, sys->students) == NULL){

            return NULL;
        }

        IsraeliQueueAddFriendshipMeasure(sys->coursesQueue[indexOfCourse]->studentQueue, FunctionArr[0]);
        IsraeliQueueAddFriendshipMeasure(sys->coursesQueue[indexOfCourse]->studentQueue, FunctionArr[1]);
        IsraeliQueueAddFriendshipMeasure(sys->coursesQueue[indexOfCourse]->studentQueue, FunctionArr[2]);
        
    } // finished reading the file and inserting the students to the courses

    free(fileLine); // COULD BE ERROR: maybe not need to free the memory - still need to check

    return sys;
}

void hackEnrollment(enrollmentSystem sys, FILE* out){

    if (sys == NULL || out == NULL){

        return;
    }

    int i = 0;

    while (sys->hackers[i]){

        if (getHackerInToTheRequiredCourses(sys->coursesQueue , sys->hackers[i]) == false){

            return; // flag of error, need to print error message to the file and return everything to it's original state
        }
       
        i++;
    }

    // we have enqueue every hacker to the courses he wanted to

    // now we gonna check if every hacker got the courses he wanted by the ex rules
   

   int index = 0;
   i = 0;
   int j = 0;

    while (sys->hackers[i]){
    
        for (; j < sys->hackers[i]->wantedCoursesNum; j++){
       
            index = returnIndexOfCourseByID(sys->coursesQueue, sys->hackers[i]->wantedCoursesID[j]);

                if (ifHackerGetCourse(sys->coursesQueue[index] ,sys->hackers[i]) == true){

                    sys->hackers[i]->gotCourseNum++;
                }

        }  

        int compared = comparisonRequireToAccept(sys->hackers[i]->wantedCoursesNum, sys->hackers[i]->gotCourseNum);

        if (compared == 0 || compared == -1){// then there is a problem


            if (compared == 0){

                fprintf(out,"Cannot satisfy constraints for %s",sys->hackers[i]->hacker->studentID);
                return;//

            }else if (compared == -1){

                return;//
            }

        }

        i++;

    }
	
    //fopen(out,"w");

    for (i = 0; i < sys->numberCourses; i++){

        fprintf(out, "%s ",sys->coursesQueue[i]->courseID);

        for (j = 0; j < sys->coursesQueue[i]->currentSize; j++){

            Student student = ReturnStudentFromQueueByIndex(sys->coursesQueue[i], j);
            fprintf(out, "%s ",student->studentID);
       
        }

        fprintf(out, "\n");

    }
   
}

int returnIndexOfCourseByID(CourseQueue *courseQueue, char *courseID){

    if (courseID == NULL || courseQueue == NULL || strlen(courseID) == 0){

        return -1;
    }

    int i = 0;

    while (courseQueue[i]){

        if (strcmp(courseQueue[i]->courseID, courseID) == 0){

            return i;
        }

        i++;
    }

    return -1;
}

bool ifHackerGetCourse(CourseQueue courseQueue ,Hacker hacker){

    int i = 0;

    while (i < courseQueue->courseSize){

        Student student = ReturnStudentFromQueueByIndex(courseQueue, i);

        if (strcmp(hacker->hacker->studentID,student->studentID) == 0){

            return true;
        }
    } 

    return false;
}

bool getHackerInToTheRequiredCourses(CourseQueue *courseQueue, Hacker hacker){
  
   int coursePlace = 0;
   
   for (int i = 0; i < hacker->wantedCoursesNum; i++ ){
    
        coursePlace = returnIndexOfCourseByID(courseQueue, hacker->wantedCoursesID[i]);

        if (coursePlace == -1){

            return false;
        }



        if (IsraeliQueueEnqueue(courseQueue[coursePlace]->studentQueue, hacker) != ISRAELIQUEUE_SUCCESS){

            return false;
        }

        // enqueue success

    }

    return true;

}

int friendshipMeasureByFile(void *hacker, void* student){

    
    Hacker h = (Hacker)hacker;
    Student s = (Student)student;

    for (int i = 0; i < h->numberFriend; i++){

        if (h->friends[i]->studentID == s->studentID){

            return FRIENDSHIP_THRESHOLD;
        }
    }

    for (int i = 0; i < h->numberRival; i++ ){

        if (h->rivals[i]->studentID == s->studentID){

            return RIVAL_THRESHOLD;

        }

    }

    return NEUTRAL_THRESHOLD;
}    

int friendshipMeasureByID(void *hacker, void* student){

    Hacker h = (Hacker)hacker;
    Student s = (Student)student;

    return (abs((atoi(h->hacker->studentID)) - (atoi(s->studentID))));
}


int returnDiffAsciiName(char* hackerName, char* studentName){
	
    // check for error

	int i = 0;
	int diff = 0;


    for (;i < min(strlen(hackerName),strlen(studentName)); i++){
     
        diff = diff + abs((int)hackerName[i]-(int)studentName[i]);

    }

    if (strlen(hackerName) < strlen(studentName)){

        while (i <= strlen(studentName)){

            diff = diff + abs((int)studentName[i]);
            i++;
        }
    }else {

        while (i <= strlen(hackerName)){

            diff = diff + abs((int)hackerName[i]);
            i++;

        }
    }

    return diff;

}

int min(int a, int b){
    
        if (a < b){
    
            return a;
        }
    
        return b;
}

int friendshipMeasureByName(void *hacker, void* student){

    Hacker h = (Hacker)hacker;
    Student s = (Student)student;
    int diffName = returnDiffAsciiName(h->hacker->name, s->name);
    int diffSurname = returnDiffAsciiName(h->hacker->surname, s->surname);

    return (diffName+diffSurname);  

}

int ItemsComparisonFunction(void *student1, void *student2){

  if (student1 == NULL || student2 == NULL){

    return 0; // not equal by default
  }

  Student s1 = (Student)student1;
  Student s2 = (Student)student2;

  // checking if casting is OK

  if (s1 == NULL || s2 == NULL){

    return 0;
  }

  bool ifStudentID = (strcmp(s1->studentID,s2->studentID) == 0);
  bool ifTotalCredits = (s1->totalCredits == s2->totalCredits);
  bool ifGPA = (s1->gpa == s2->gpa);
  bool ifName = (strcmp(s1->name,s2->name) == 0);
  bool ifSurname = (strcmp(s1->surname,s2->surname) == 0);
  bool ifCity = (strcmp(s1->city,s2->city) == 0);
  bool ifDepartment = (strcmp(s1->department,s2->department) == 0);

  if (ifStudentID && ifTotalCredits && ifGPA && ifName && ifSurname && ifCity && ifDepartment){

    return 1; // equal

  }

  return 0; // not equal

}


// return 1 if hacker Received the courses he required respectively.

// return 0 if we need to print the 'cannot' massage, not printing to the file the new_queues, 
// and return the enrollment system in it's original state.

// return -1, the process is failed, we don't print anything to the file, 

// return -2, bad parameters

int comparisonRequireToAccept(int requireCourses, int acceptedCourses){

    if(requireCourses >= 2 ){

        if (acceptedCourses >= 2){

            return 1;
        }

        if (acceptedCourses == 1){

            return 0;
        }

        if (acceptedCourses == 0){

            return -1;
        }

    }

    if (requireCourses == 1){

        if (acceptedCourses == 1){

            return 1;
        }

        if (acceptedCourses == 0){

            return -1;
        }

    }

    if (requireCourses == 0 ){

        return 1;
    }

    return -2; // bad parameters
    
}

Student ReturnStudentFromQueueByIndex(CourseQueue courseQueue, int index){

    if (courseQueue == NULL || index < 0){

        return NULL;
    }

    IsraeliQueue temp = IsraeliQueueClone(courseQueue->studentQueue);

    if (temp == NULL){

        return NULL;
    }

    for (int i = 0; i < index; i++){

        if (IsraeliQueueDequeue(temp) == NULL){

            IsraeliQueueDestroy(temp);
            return NULL;
            
        }
        
    }

    Student student = (Student)IsraeliQueueDequeue(temp);

    if (student == NULL){

        IsraeliQueueDestroy(temp);
        return NULL;
    }

    IsraeliQueueDestroy(temp);
    return student;
}

