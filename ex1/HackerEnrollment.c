#define _GNU_SOURCE
#include "HackerEnrollment.h"
#include "Item.h"
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
int friendshipMeasureByNameWithFlag(void *hacker, void* student);
int ItemsComparisonFunction(void *student1, void *student2);
CourseQueue* readFileCourses(FILE *coursesFile, int coursesNum, CourseQueue *coursesQueue);
Student* readFileStudent(FILE *studentFile, int studentNum, Student *students);
Hacker* readFileHackers(FILE *hackerFile, int HackerNum, Hacker *hackers, Student *students);
int returnDiffAsciiName(char* hackerName, char* studentName, bool capitalLetter);
int GetLinesNum(FILE *file);
int min(int a, int b);
Student ReturnStudentFromQueueByIndex(CourseQueue courseQueue, int index);

void PrintSystem(enrollmentSystem sys){

    printf("--------SYSTEM PRINT---------\n");


    printf("number of courses: %d\n",sys->numberCourses);

    for (int i = 0; i<sys->numberCourses; i++){

        PrintCourse(sys->coursesQueue[i]);
    }

    for (int i = 0; i<sys->numberStudents; i++){

        PrintStudent(sys->students[i]);
    }

    for (int i = 0; i< sys->numberHackers; i++){

        PrintHacker(sys->hackers[i]);
    }

    printf("--------END OF SYSTEM PRINT---------\n");

   
}


enrollmentSystem createEnrollmentSystem(FILE* students, FILE* courses, FILE* hackers){

    enrollmentSystem new_system = (enrollmentSystem)malloc(sizeof(*new_system));

    if (new_system == NULL){

        return NULL;
    }

    // malloc OK

    new_system->courses_file = courses;
    new_system->students_file = students;
    new_system->hackers_file = hackers;
   
    int numberCourses = GetLinesNum(new_system->courses_file);

    new_system->numberCourses = numberCourses;

    int numberStudents = GetLinesNum(new_system->students_file);

    int numberHackers = GetLinesNum(new_system->hackers_file);

    numberHackers = numberHackers/4; // because each hacker has 4 lines
    new_system->numberHackers = numberHackers;
    
    new_system->coursesQueue = (CourseQueue*)malloc(sizeof(CourseQueue)*(numberCourses+1));

    if (new_system->coursesQueue == NULL){

        return NULL;
    }

    new_system->coursesQueue[numberCourses] = NULL;

    // creting CourseQueue

    new_system->coursesQueue = readFileCourses(new_system->courses_file, numberCourses, new_system->coursesQueue);

    if (new_system->coursesQueue == NULL){

        return NULL;
    }

    // NOT creating the FriendshipFunctionArr inside the courseQueue because we dont need it now (we need it only in the readEnrollment function)


    // creating the comparison function inside the courseQueue (argument of IsraeliQueueCreate)

    ComparisonFunction comp = &ItemsComparisonFunction;

    // creating the israeliQueue inside the courseQueue 

    for (int i = 0; i < numberCourses; i++){

        new_system->coursesQueue[i]->studentQueue = IsraeliQueueCreate(NULL, comp, FRIENDSHIP_THRESHOLD, RIVAL_THRESHOLD);

        if (new_system->coursesQueue[i]->studentQueue == NULL){

            return NULL;
        }

    }

    // finished creating CourseQueue

    // creating the students array inside the enrollmentSystem

    new_system->students = (Student*)malloc(sizeof(Student)*(numberStudents+1));

    if (new_system->students == NULL){

        return NULL;
    }

    new_system->students[numberStudents] = NULL;

    new_system->numberStudents = numberStudents;

    new_system->students = readFileStudent(new_system->students_file, numberStudents, new_system->students);

    if (new_system->students == NULL){

        return NULL;
    }

    
    new_system->hackers = (Hacker*)malloc(sizeof(Hacker)*(numberHackers+1));

    if (new_system->hackers == NULL){

        return NULL;
    }

    new_system->hackers[numberHackers] = NULL;

    // malloc OK
    
    new_system->hackers = readFileHackers(new_system->hackers_file, numberHackers,new_system->hackers ,new_system->students);

    if (new_system->hackers == NULL){

        printf("hackers NULL!");
        return NULL;
    }

    new_system->nameFlag = false;


    PrintSystem(new_system);
    return new_system;
     
 }



 CourseQueue* readFileCourses(FILE* coursesFile, int coursesNum, CourseQueue *coursesQueue){

  if (coursesFile == NULL || coursesQueue == NULL) 
  {
    return NULL;
  }

  char *fileLine = NULL;
  size_t len = 0; // size_t is an unsigned integer data type


  if (coursesFile == NULL){

    return NULL;
}

fseek(coursesFile, 0, SEEK_SET);

for (int i = 0; i < coursesNum; i++){
      
    if (getline(&fileLine, &len, coursesFile) != -1){

      coursesQueue[i] = CourseQueueCreate(fileLine);
      fileLine = NULL;
      len = 0;
    
    }else {

        return NULL;
    }
    
}



fseek(coursesFile, 0, SEEK_SET);
return coursesQueue;

}


 void SetNameFlag(enrollmentSystem sys, bool flag){

    if (sys == NULL){

        return;
    }

    sys->nameFlag = flag;

 }

Student* readFileStudent(FILE *studentFile, int studentNum, Student *students){

    if (studentFile == NULL || students == NULL){
        
        return NULL;
    }

    char *fileLine = NULL;
    size_t len = 0;
  
    if (studentFile == NULL){

        return NULL;
    
    }


    fseek(studentFile, 0, SEEK_SET);


    for (int i = 0; i < studentNum; i++){
        
        if ((getline(&fileLine, &len, studentFile)) != -1){

            students[i] = studentCreate(fileLine);
            //printf("create Student function returns:\n");
            //PrintStudent(students[i]);
            fileLine = NULL;
            len = 0;

        }else{

            return NULL;

        } 

    }

    fseek(studentFile, 0, SEEK_SET);
    return students;

}

Hacker* readFileHackers(FILE *hackerFile, int hackerNum, Hacker *hackers, Student *students){

    if (hackerFile == NULL || hackers == NULL){

        return NULL;
    }

    char *fileLine = NULL;
    size_t len = 0;

    char *hackerLines[5] = {NULL};

    fseek(hackerFile, 0, SEEK_SET);

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

        //printf("create hacker function returns:\n");
        hackers[i] = HackerCreate(students,hackerLines);
        //PrintHacker(hackers[i]);

        if (hackers[i] == NULL){

            return NULL;
        }

    }


    fseek(hackerFile, 0, SEEK_SET);
    return hackers;

 }

 int GetLinesNum(FILE *file){
	
	char *line = NULL;
    size_t len = 0;
    int read = 0;
	int counter = 0; 

    fseek(file, 0, SEEK_SET);

    while ((read = getline(&line, &len, file)) != -1) {
		
		// exist line 

        if (strlen(line) == 0){

          break;

        }


        counter++;

    }
	
    fseek(file, 0, SEEK_SET);
    free(line);

	return counter;
}



// The function reads a file describing the registration queues for courses. The file format: 
//<Course Number> <Student IDs>* and will return an EnrollmentSystem structure representing the registration queues
enrollmentSystem readEnrollment(enrollmentSystem sys, FILE* queues){

    
    if (sys == NULL || queues == NULL){

        return NULL;
    }

    //printf("******************** system before adding students to courses ********************\n");
    //PrintSystem(sys);
    //printf("*********************************************************************************\n");

    char *fileLine = NULL;
    size_t len = 0;

    // this is for the israeliqueue for after the regular adding students to the course
    FriendshipFunction *FunctionArr = (FriendshipFunction*)malloc(sizeof(FunctionArr)*4);

    if (FunctionArr == NULL){

        return NULL;
    }

    FunctionArr[0] = &friendshipMeasureByFile;
    FunctionArr[1] = &friendshipMeasureByID;

    if (sys->nameFlag == true){

        FunctionArr[2] = &friendshipMeasureByNameWithFlag;
    
    }else{

        FunctionArr[2] = &friendshipMeasureByName;
    }

    FunctionArr[3] = NULL;
    // end of friendship function array

    fseek(queues, 0, SEEK_SET);


    // now we will read the file and insert the students to the courses
    while ((getline(&fileLine, &len, queues)) != -1){


        char *temp = (char*)malloc((strlen(fileLine) + 1) * sizeof(char));
        strcpy(temp, fileLine);

        if (strlen(fileLine) == 0){

            break;
        }

        char *courseID = strtok(fileLine, " ");
        FixToken(courseID);


        //int i = 0;
        
        int indexOfCourse = returnIndexOfCourseByID(sys->coursesQueue, courseID, sys->numberCourses);

        if (indexOfCourse == -1){

            return NULL;
        }
        
        if (InsertStudentsToCourseQueue(sys->coursesQueue[indexOfCourse], temp, sys->students, sys->numberStudents) == NULL){

            return NULL;
        }

        IsraeliQueueAddFriendshipMeasure(sys->coursesQueue[indexOfCourse]->studentQueue, FunctionArr[0]);
        IsraeliQueueAddFriendshipMeasure(sys->coursesQueue[indexOfCourse]->studentQueue, FunctionArr[1]);
        IsraeliQueueAddFriendshipMeasure(sys->coursesQueue[indexOfCourse]->studentQueue, FunctionArr[2]);


        free(temp);
        
    } // finished reading the file and inserting the students to the courses


    fseek(queues, 0, SEEK_SET);

    printf("\n******************** system after adding students to courses ********************\n");
    PrintSystem(sys);
    printf("\n*********************************************************************************\n");

    return sys;
}

void hackEnrollment(enrollmentSystem sys, FILE* out){

    printf("\n******************** system before hackenrollment courses ********************\n");
    PrintSystem(sys);
    printf("\n*********************************************************************************\n");

    if (sys == NULL || out == NULL){

        return;
    }

    int i = 0;

    for (; i < sys->numberHackers; i++){

        if (getHackerInToTheRequiredCourses(sys->coursesQueue,sys->hackers[i]->hacker ,sys->hackers[i]->wantedCoursesID,sys->hackers[i]->wantedCoursesNum, sys->numberCourses) == false){

            return; // flag of error, need to print error message to the file and return everything to it's original state
        }
       
    }

    //printf("\n******************** system after getting hacker to wanted courses ********************\n");
    //PrintSystem(sys);
    //printf("\n*********************************************************************************\n");

    // we have enqueue every hacker to the courses he wanted to

    // now we gonna check if every hacker got the courses he wanted by the ex rules
   

   int index = 0;
   i = 0;
   int j = 0;


    for (; i < sys->numberHackers; i++){
    
        printf("PPPPPPPPPPPPPPPPPPPPhacker %s has %d wanted courses\n",sys->hackers[i]->hacker->studentID, sys->hackers[i]->wantedCoursesNum);

        for (; j < sys->hackers[i]->wantedCoursesNum; j++){

            printf("wanted course id = %s\n",sys->hackers[i]->wantedCoursesID[j]);       
            index = returnIndexOfCourseByID(sys->coursesQueue, sys->hackers[i]->wantedCoursesID[j], sys->numberCourses);
            printf("index of wanted course = %d\n",index);

            if (ifHackerGetCourse(sys->coursesQueue[index] ,sys->hackers[i]) == true){

                sys->hackers[i]->gotCourseNum++;
                printf("hacker got course!\n");
            }


        }

        j = 0;  


    }
    
    printf("\n******************** system after checking if hacker get course ********************\n");
    PrintSystem(sys);
    printf("\n*********************************************************************************\n");

    i = 0; 

    for (; i < sys->numberHackers; i++){
    
        int compared = comparisonRequireToAccept(sys->hackers[i]->wantedCoursesNum, sys->hackers[i]->gotCourseNum);

        printf("compared = %d\n",compared);

        if (compared == 0 || compared == -1){// then there is a problem

            if (compared == 0){

                fprintf(out,"Cannot satisfy constraints for %s",sys->hackers[i]->hacker->studentID);
                return;

            }else if (compared == -1){

                return;
            }

        }

    }

    for (i = 0; i < sys->numberCourses; i++){

        if (sys->coursesQueue[i]->currentSize > 0){
            
            fprintf(out, "%s ",sys->coursesQueue[i]->courseID);

            for (j = 0; j < sys->coursesQueue[i]->currentSize; j++){

                Student student = ReturnStudentFromQueueByIndex(sys->coursesQueue[i], j);

                fprintf(out, "%s ",student->studentID);
       
            }

            fprintf(out, "\n");

        }

    }
   
}

int returnIndexOfCourseByID(CourseQueue *courseQueue, char *courseID, int courseNum){

    if (courseID == NULL || courseQueue == NULL || strlen(courseID) == 0){

        return -1;
    }

    for (int i = 0; i < courseNum; i++){

        FixToken(courseQueue[i]->courseID);
        FixToken(courseID);

        if (strcmp(courseQueue[i]->courseID, courseID) == 0){

            return i;
        }

        
    }

    return -1;
}

bool ifHackerGetCourse(CourseQueue courseQueue ,Hacker hacker){


    if (courseQueue == NULL || hacker == NULL){

        return false;
    }

    if (courseQueue->currentSize <= 0){

        return false;
    }

    IsraeliQueue tempQueue = IsraeliQueueClone(courseQueue->studentQueue);

    if (tempQueue == NULL){

        return false;
    }

    printf("original and cloned queue before:\n");
    PrintIsraeliQueue(courseQueue->studentQueue);
    PrintIsraeliQueue(tempQueue);


    int searchingIndex = 0;

    if (courseQueue->currentSize > courseQueue->courseSize){

        searchingIndex = courseQueue->courseSize;

    }else { // courseQueue->currentSize <= courseQueue->courseSize

        searchingIndex = courseQueue->currentSize;
    }

    for (int i = 0; i < searchingIndex; i++){

        Item item = IsraeliQueueDequeue(tempQueue);
        Student student = (Student)item;

        if (strcmp(student->studentID, hacker->hacker->studentID) == 0){
            

            //printf("original and cloned queue after:\n");
            //PrintIsraeliQueue(courseQueue->studentQueue);
            //PrintIsraeliQueue(tempQueue);
            free(tempQueue);
            //PrintIsraeliQueue(courseQueue->studentQueue);
            return true;
        }
    }

    free(tempQueue);
    return false;

}

bool getHackerInToTheRequiredCourses(CourseQueue *courseQueue, Student hacker,char** wantedCourseID, int wantedCoursesNum, int coursesNum){
  
   int coursePlace = 0;
   
   for (int i = 0; i < wantedCoursesNum; i++){
    
        coursePlace = returnIndexOfCourseByID(courseQueue, wantedCourseID[i],coursesNum);

        if (coursePlace == -1){
            
            
            printf("******************************\n");
            printf("error in returnIndexOfCourseByID\n");
            return false;
        }

        if (IsraeliQueueEnqueue(courseQueue[coursePlace]->studentQueue, hacker) != ISRAELIQUEUE_SUCCESS){
            
            
            printf("******************************\n");
            printf("error in enqueue\n");
            return false;
        }

        courseQueue[coursePlace]->currentSize++;

        // enqueue success

    }

    return true;

}

int friendshipMeasureByFile(void *hacker, void* student){

    printf("by friendshipMeasureByFile: ");

    Student h = (Student)hacker;
    Student s = (Student)student;

    if (h->ifHacker == false && s->ifHacker == false){

        return NEUTRAL_THRESHOLD;
    }

    if (h->ifHacker == true){

        for (int i = 0; i < h->numberFriend; i++){

            if (strcmp(h->friendsID[i] ,s->studentID) == 0){

                printf("hacker %s is friend with student %s\n",h->studentID,s->studentID);
                return FRIENDSHIP_THRESHOLD;
            }
        }


        for (int i = 0; i < h->numberRival; i++){

            if (strcmp(h->rivalsID[i],s->studentID) == 0){

                printf("hacker %s is rival with student %s\n",h->studentID,s->studentID);
                return RIVAL_THRESHOLD;

            }
        }

    }

    if (s->ifHacker == true){

        for (int i = 0; i < s->numberFriend; i++){

            if (strcmp(s->friendsID[i],h->studentID) == 0){

                printf("hacker %s is friend with student %s\n",s->studentID,h->studentID);
                return FRIENDSHIP_THRESHOLD;
            }
        }



        for (int i = 0; i < s->numberRival; i++){

            if (strcmp(s->rivalsID[i] , h->studentID) == 0){

                printf("hacker %s is rival with student %s\n",s->studentID,h->studentID);
                return RIVAL_THRESHOLD;

            }
        }

    }

    printf("nor friends nor rivals!\n");
    return NEUTRAL_THRESHOLD;
}    

int friendshipMeasureByID(void *hacker, void* student){

    printf("by friendshipMeasureByID: ");

    Student h = (Student)hacker;
    Student s = (Student)student;

    int difInAbs = (abs((atoi(h->studentID)) - (atoi(s->studentID))));

    printf("diff in Abs between %s and %s is %d\n",h->studentID,s->studentID,difInAbs);

    return difInAbs;
}


int upperToLower(int a){
    
    if ( a >= 65 && a <= 90){

        return (a + 32);
    }

    return a;
}


int returnDiffAsciiName(char* hackerName, char* studentName, bool capitalLetter){
	
    // check for error

	int i = 0;
	int diff = 0;
    int hackerValueLetter = 0;
    int studentValueLetter = 0;

    for (;i < min(strlen(hackerName),strlen(studentName)); i++){

        if (capitalLetter){

            hackerValueLetter = upperToLower((int)hackerName[i]);
            studentValueLetter = upperToLower((int)studentName[i]);

        }else{

            hackerValueLetter = (int)hackerName[i];
            studentValueLetter = (int)studentName[i];
        }

        diff = diff + abs(studentValueLetter-hackerValueLetter);
    }
    
    if (strlen(hackerName) < strlen(studentName)){

        while (i <= strlen(studentName)){

            if (capitalLetter){

                studentValueLetter = upperToLower((int)studentName[i]);
                diff = diff + studentValueLetter;
                i++;
            }else{

                diff = diff + (int)studentName[i];
                i++;
            }
        }

    }else{

        while (i <= strlen(hackerName)){
             
            if (capitalLetter){
            
                hackerValueLetter = upperToLower((int)hackerName[i]);
            
            }else{

                hackerValueLetter = (int)hackerName[i];
            }  

            diff = diff + hackerValueLetter;
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

    printf("by friendshipMeasureByName: ");
    Student h = (Student)hacker;
    Student s = (Student)student;
    int diffName = returnDiffAsciiName(h->studentID, s->studentID, false);
    int diffSurname = returnDiffAsciiName(h->surname, s->surname, false);

    printf("diff is  %d\n",diffName+diffSurname);
    return (diffName+diffSurname);  

}

int friendshipMeasureByNameWithFlag(void *hacker, void* student){

    printf("by friendshipMeasureByNameWithFlag: ");
    Student h = (Student)hacker;
    Student s = (Student)student;
    int diffName = returnDiffAsciiName(h->studentID, s->studentID, true);
    int diffSurname = returnDiffAsciiName(h->surname, s->surname, true);

    printf("diff is %d\n",diffName+diffSurname);

    return (diffName+diffSurname);  
}

int ItemsComparisonFunction(void *student1, void *student2){

    printf("by ItemsComparisonFunction: ");

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

    printf("items equal\n");
    return 1; // equal

  }

    printf("items not equal\n");
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

