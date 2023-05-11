#include "HackerEnrollment.h"
#include "Item.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define FRIENDSHIP_THRESHOLD_FUNCTION 20
#define RIVAL_THRESHOLD_FUNCTION -20
#define NEUTRAL_THRESHOLD_FUNCTION 0
#define FRIENDSHIP_THRESHOLD 20
#define RIVAL_THRESHOLD 0

int friendshipMeasureByFile(void *hacker, void* student);
int friendshipMeasureByID(void *hacker, void* student);
int friendshipMeasureByName(void *hacker, void* student);
int friendshipMeasureByNameWithFlag(void *hacker, void* student);
int ItemsComparisonFunction(void *student1, void *student2);
CourseQueue* readFileCourses(char** coursesLines, int coursesNum, CourseQueue *coursesQueue);
Student* readFileStudent(char** studentLines, int studentNum, Student *students);
Hacker* readFileHackers(char** hackerLines, int hackerNum, Hacker *hackers, Student *students, int studentNum);
int returnDiffAsciiName(char* hackerName, char* studentName, bool capitalLetter);
int min(int a, int b);
Student ReturnStudentFromQueueByIndex(CourseQueue courseQueue, int index);
char* readLine(FILE* file);
char** readFileLines(FILE* file, int* referenceToLineNumber);
bool getHackerInToTheRequiredCourses(CourseQueue *courseQueue, Hacker hacker,char** wantedCourseID, int wantedCoursesNum, int coursesNum);
bool ifHackerGetCourse(CourseQueue courseQueue ,Hacker h);
int returnIndexOfCourseByID(CourseQueue *courseQueue, char *courseID, int coursesNum);
int comparisonRequireToAccept(int requireCourses, int accepteCourses);


void PrintSystem(enrollmentSystem sys){

    printf("\n\n\n------------SYSTEM PRINT------------\n");

    printf("%d courses in sys\n",sys->numberCourses);

    for (int i = 0; i<sys->numberCourses; i++){

        PrintCourse(sys->coursesQueue[i]);
    }

    printf("\n%d students in sys:\n",sys->numberStudents);
\
    for (int i = 0; i<sys->numberStudents; i++){

        PrintStudent(sys->students[i]);
    }

    printf("\n%d hackers in sys:\n",sys->numberHackers);

    for (int i = 0; i< sys->numberHackers; i++){

        PrintHacker(sys->hackers[i]);
    }

    printf("-------------END OF SYSTEM PRINT-------------\n\n\n");
   
}


enrollmentSystem createEnrollmentSystem(FILE* students, FILE* courses, FILE* hackers){

    enrollmentSystem new_system = (enrollmentSystem)malloc(sizeof(*new_system));


    if (new_system == NULL){

        return NULL;
    }

    // malloc OK

    // initializing the files inside the enrollmentSystem
    new_system->courses_file = courses;
    new_system->students_file = students;
    new_system->hackers_file = hackers;



    // GETTING STUDETNS

    int numberStudents = 0;
    char **studentsLines = readFileLines(new_system->students_file, &numberStudents);
    if (studentsLines == NULL){
        return NULL;
    }
    new_system->numberStudents = numberStudents;

    // GETTING COURSES 
    int numberCourses = 0;
    char** coursesLines = readFileLines(new_system->courses_file, &numberCourses);

    if (coursesLines == NULL){

        return NULL;
    }
    new_system->numberCourses = numberCourses;


    // GETTING HACKERS
    int numberHackers = 0;
    char** hackersLine = readFileLines(new_system->hackers_file, &numberHackers);

    if (hackersLine == NULL){

        return NULL;
    }

    numberHackers = numberHackers/4; // EACH HACKER HAS 4 LINES
    new_system->numberHackers = numberHackers;
    

    // CREATING THE COURSES QUEUE
    new_system->coursesQueue = (CourseQueue*)malloc(sizeof(CourseQueue)*(numberCourses+1));
    if (new_system->coursesQueue == NULL){

        return NULL;
    }
    new_system->coursesQueue[numberCourses] = NULL;

    // READS THE COURSES FROM THE FILE AND CREATES THE COURSES QUEUE
    new_system->coursesQueue = readFileCourses(coursesLines, numberCourses, new_system->coursesQueue);

    if (new_system->coursesQueue == NULL){
        return NULL;
    }

    // NOT creating the FriendshipFunctionArr inside the courseQueue because we dont need it now 
    //(we need it only in the readEnrollment function)


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


    new_system->students = readFileStudent(studentsLines, numberStudents, new_system->students);

    if (new_system->students == NULL){

        return NULL;
    }

    
    new_system->hackers = (Hacker*)malloc(sizeof(Hacker)*(numberHackers+1));
    if (new_system->hackers == NULL){
        return NULL;
    }
    new_system->hackers[numberHackers] = NULL;
    // malloc OK
    
    new_system->hackers = readFileHackers(hackersLine, numberHackers,new_system->hackers ,new_system->students, numberStudents);

    if (new_system->hackers == NULL){

        return NULL;
    }

    new_system->nameFlag = false;

    return new_system;
 }


 CourseQueue* readFileCourses(char** coursesLines, int coursesNum, CourseQueue *coursesQueue){

    if (coursesLines == NULL || coursesQueue == NULL){
        
        return NULL;
    }

    for (int i = 0; i < coursesNum; i++){
    
        if (coursesLines[i] == NULL || strlen(coursesLines[i]) == 0){

            return NULL;
        }

        coursesQueue[i] = CourseQueueCreate(coursesLines[i]);

        if (coursesQueue[i] == NULL){

            return NULL;
        }
    
    }

    return coursesQueue;

}


 void SetNameFlag(enrollmentSystem sys, bool flag){

    if (sys == NULL){

        return;
    }

    sys->nameFlag = flag;

 }

Student* readFileStudent(char **studentLines, int studentNum, Student *students){


    if (studentLines == NULL || students == NULL){
        
        return NULL;
    }

    for (int i = 0; i < studentNum; i++){

        if (studentLines[i] == NULL && strlen(studentLines[i]) == 0){

            return NULL;
        }

        students[i] = studentCreate(studentLines[i]);
        
        if (students[i] == NULL){

            return NULL;
        }

    }

    return students;

}

Hacker* readFileHackers(char** hackerLines, int hackerNum, Hacker *hackers, Student *students, int studentNum){

    if (hackerLines == NULL || hackers == NULL){

        return NULL;
    }

    char *hackerLinesSend[5] = {NULL};

    for (int i = 0; i < hackerNum; i++){ // hackerNum divided by 4 because each hacker has 4 lines
      
        for (int j = 0; j < 4; j++){

            hackerLinesSend[j] = hackerLines[i*4+j];
        } 

        hackerLinesSend[4] = NULL;

        
        hackers[i] = HackerCreate(students ,hackerLinesSend, studentNum);

        if (hackers[i] == NULL){
            
            return NULL;
        }

    }

    return hackers;

 }


// The function reads a file describing the registration queues for courses. The file format: 
//<Course Number> <Student IDs>* and will return an EnrollmentSystem structure representing the registration queues
enrollmentSystem readEnrollment(enrollmentSystem sys, FILE* queues){

    if (sys == NULL || queues == NULL){

        return NULL;
    }

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

    int NumLines = 0;
    char** queuesLines = readFileLines(queues, &NumLines);

    
    for (int i = 0; i< NumLines; i++){

        if (queuesLines[i] == NULL){

            return NULL;
        }

        trim(queuesLines[i]);

        if (strlen(queuesLines[i]) == 0){
            
            break;
        }

        char *temp1 = (char*)malloc((strlen(queuesLines[i]) + 1) * sizeof(char));
        char *temp2 = (char*)malloc((strlen(queuesLines[i]) + 1) * sizeof(char));

        if (temp1 == NULL || temp2 == NULL){

            return NULL;
        }

        strcpy(temp1, queuesLines[i]);
        strcpy(temp2, queuesLines[i]);

        if (temp1 == NULL || temp2 == NULL){

            return NULL;
        }

        char *courseID = strtok(temp1, " ");
        trim(courseID);

        
        int indexOfCourse = returnIndexOfCourseByID(sys->coursesQueue, courseID, sys->numberCourses);

        
        if (indexOfCourse == -1){
            
            free(temp1);
            free(temp2);
            return NULL;
        }

        free(temp1);

        if (InsertStudentsToCourseQueue(sys->coursesQueue[indexOfCourse], temp2, sys->students, sys->numberStudents) == NULL){
            
            free(temp2);
            return NULL;
        }

        free(temp2);

        IsraeliQueueAddFriendshipMeasure(sys->coursesQueue[indexOfCourse]->studentQueue, FunctionArr[0]);
        IsraeliQueueAddFriendshipMeasure(sys->coursesQueue[indexOfCourse]->studentQueue, FunctionArr[1]);
        IsraeliQueueAddFriendshipMeasure(sys->coursesQueue[indexOfCourse]->studentQueue, FunctionArr[2]);
    }


    for (int i = 0; i<sys->numberCourses; i++){

        if (sys->coursesQueue[i]->currentSize == 0){

            IsraeliQueueAddFriendshipMeasure(sys->coursesQueue[i]->studentQueue, FunctionArr[0]);
            IsraeliQueueAddFriendshipMeasure(sys->coursesQueue[i]->studentQueue, FunctionArr[1]);
            IsraeliQueueAddFriendshipMeasure(sys->coursesQueue[i]->studentQueue, FunctionArr[2]);
        
        }

    }

    return sys;
}

void hackEnrollment(enrollmentSystem sys, FILE* out){


    if (sys == NULL || out == NULL){

        return;
    }

    int i = 0;

    for (; i < sys->numberHackers; i++){
        

        if (getHackerInToTheRequiredCourses(sys->coursesQueue,sys->hackers[i] ,sys->hackers[i]->wantedCoursesID,sys->hackers[i]->wantedCoursesNum, sys->numberCourses) == false){

            return; // flag of error, need to print error message to the file and return everything to it's original state
        }
       
    }


    // we have enqueue every hacker to the courses he wanted to

    // now we gonna check if every hacker got the courses he wanted by the ex rules
   

   int index = 0;
   i = 0;
   int j = 0;


    for (; i < sys->numberHackers; i++){
    

        for (; j < sys->hackers[i]->wantedCoursesNum; j++){

            index = returnIndexOfCourseByID(sys->coursesQueue, sys->hackers[i]->wantedCoursesID[j], sys->numberCourses);

            

            if (ifHackerGetCourse(sys->coursesQueue[index] ,sys->hackers[i]) == true){

                sys->hackers[i]->gotCourseNum++;
            }


        }

        j = 0;  


    }
    
    
    i = 0; 

    for (; i < sys->numberHackers; i++){
    
        int compared = comparisonRequireToAccept(sys->hackers[i]->wantedCoursesNum, sys->hackers[i]->gotCourseNum);
           
    
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

        trim(courseQueue[i]->courseID);
        trim(courseID);

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

    if (IsraeliQueueSize(courseQueue->studentQueue) <= 0){

        return false;
    }

    IsraeliQueue tempQueue = IsraeliQueueClone(courseQueue->studentQueue);

    if (tempQueue == NULL){

        return false;
    }

    int searchingIndex = 0;

    if (IsraeliQueueSize(courseQueue->studentQueue) > courseQueue->courseSize){

        searchingIndex = courseQueue->courseSize;

    }else { // IsraeliQueueSize(courseQueue->studentQueue) <= courseQueue->courseSize

        searchingIndex = IsraeliQueueSize(courseQueue->studentQueue);
    }

    for (int i = 0; i < searchingIndex; i++){

        Item item = IsraeliQueueDequeue(tempQueue);
        Student student = (Student)item;

        if (strcmp(student->studentID, hacker->hacker->studentID) == 0){
            

            free(tempQueue);
            return true;
        }
    }

    free(tempQueue);
    return false;

}

bool getHackerInToTheRequiredCourses(CourseQueue *courseQueue, Hacker hacker,char** wantedCourseID, int wantedCoursesNum, int coursesNum){
  
   int coursePlace = 0;
   
   for (int i = 0; i < wantedCoursesNum; i++){//runing on the wanted courses and enqueue the hacker.
    
        coursePlace = returnIndexOfCourseByID(courseQueue, hacker->wantedCoursesID[i],coursesNum);

        if (coursePlace == -1){
            
            return false;
        }

        if (IsraeliQueueEnqueue(courseQueue[coursePlace]->studentQueue, hacker->hacker) != ISRAELIQUEUE_SUCCESS){
            
            
            return false;
        }

        courseQueue[coursePlace]->currentSize = IsraeliQueueSize(courseQueue[coursePlace]->studentQueue);

        // enqueue success

    }

    return true;

}

int friendshipMeasureByFile(void *hacker, void* student){


    Student h = (Student)hacker;
    Student s = (Student)student;

    if (h->ifHacker == false && s->ifHacker == false){

        return NEUTRAL_THRESHOLD_FUNCTION;
    }

    if (h->ifHacker == true){

        for (int i = 0; i < h->numberFriend; i++){

            if (strcmp(h->friendsID[i] ,s->studentID) == 0){

                return FRIENDSHIP_THRESHOLD_FUNCTION;
            }
        }


        for (int i = 0; i < h->numberRival; i++){

            if (strcmp(h->rivalsID[i],s->studentID) == 0){

                return RIVAL_THRESHOLD_FUNCTION;

            }
        }

    }

    return NEUTRAL_THRESHOLD_FUNCTION;
}    

int friendshipMeasureByID(void *hacker, void* student){


    Student h = (Student)hacker;
    Student s = (Student)student;

    int hID = atoi(h->studentID);
    int sID = atoi(s->studentID);

    int difID =  (hID - sID);

    int difInAbs = abs(difID);


    return difInAbs;
}

int upperToLower(int a){
    
    if (a >= 65 && a <= 90){

        return (a + 32);
    }

    return a;
}

int returnDiffAsciiName(char* hackerName, char* studentName, bool capitalLetter){
	
    if (hackerName == NULL || studentName == NULL) {
        
        return -1; 
    }

	int i = 0;
	int diff = 0;
    char hackerValueLetter = 0;
    char studentValueLetter = 0;

    for (;i < min(strlen(hackerName),strlen(studentName)); i++){

        if (capitalLetter){

            hackerValueLetter = upperToLower((int)hackerName[i]);
            studentValueLetter = upperToLower((int)studentName[i]);

        }else{

            hackerValueLetter = (int)hackerName[i];
            studentValueLetter = (int)studentName[i];
        }

        int AbsDif = abs(hackerValueLetter - studentValueLetter);

        diff = diff + abs(AbsDif);
    }
    
    if (strlen(hackerName) < strlen(studentName)){

        while (i < strlen(studentName)){

            if (capitalLetter){

                studentValueLetter = upperToLower((int)studentName[i]);
                diff = diff + studentValueLetter;

            }else{

                diff = diff + (int)studentName[i];
                
            }

            i++;

        }

    }else if (strlen(hackerName) > strlen(studentName)){

        while (i < strlen(hackerName)){
             
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

    Student h = (Student)hacker;
    Student s = (Student)student;
    int diffName = returnDiffAsciiName(h->name, s->name, false);
    int diffSurname = returnDiffAsciiName(h->surname, s->surname, false);

    return (diffName+diffSurname);  

}

int friendshipMeasureByNameWithFlag(void *hacker, void* student){

    Student h = (Student)hacker;
    Student s = (Student)student;
    int diffName = returnDiffAsciiName(h->name, s->name, true);
    int diffSurname = returnDiffAsciiName(h->surname, s->surname, true);

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


char* readLine(FILE* file){
	
    char* buffer = NULL;
    int bufferSize = 0;
    int length = 0;
    int ch;

    while ((ch = fgetc(file)) != EOF && ch != '\n') {
        if (length + 1 >= bufferSize) {
            bufferSize = (bufferSize == 0) ? 2 : bufferSize * 2;
            char* newBuffer = realloc(buffer, bufferSize);
            if (newBuffer == NULL) {
                
                free(buffer);
                return NULL;
            }
            buffer = newBuffer;
        }
        buffer[length++] = ch;
    }

    if (length == 0 && ch == EOF) {
        return NULL;  // Reached end of file
    }

    buffer = realloc(buffer, length + 1);  // Resize buffer to actual length
    buffer[length] = '\0';
    return buffer;
}

char** readFileLines(FILE* file, int* referenceToLineNumber){
	
    if (file == NULL) {
		
        
        return NULL;
    }

    char* line = NULL;
	
	int counter = 0;
	
    while ((line = readLine(file)) != NULL){
		
		counter++;
        free(line);
    }
	
	*(referenceToLineNumber) = counter;
	
    fseek(file, 0, SEEK_SET);
	
	char** temp = (char**)malloc(sizeof(char*)*(counter+1));
	
	if (temp == NULL){
		
		return NULL;
	}
	
	int i = 0;

	while ((line = readLine(file)) != NULL){
		
		temp[i] = line;
		i++;
        
    }
	
    fseek(file, 0, SEEK_SET);
	return temp;
		
}


void destroyEnrollmentSystem(enrollmentSystem sys){

    if (sys == NULL){

        return;
    }

    // checking if the sys files need to be closed

    if (sys->students_file != NULL){

        fclose(sys->students_file);
    }

    if (sys->hackers_file != NULL){

        fclose(sys->hackers_file);
    }

    if (sys->courses_file != NULL){

        fclose(sys->courses_file);
    }

    if (sys->coursesQueue != NULL){

        for (int i = 0; i < sys->numberCourses; i++){

            if (sys->coursesQueue[i] != NULL){

                CourseQueueDestroy(sys->coursesQueue[i]);
            }
        }

        free(sys->coursesQueue);
    }

    if (sys->students != NULL){

        for (int i = 0; i < sys->numberStudents; i++){

            if (sys->students[i] != NULL){

                studentDestroy(sys->students[i]);
            }
        }

        free(sys->students);

    }

    if (sys->hackers != NULL){

        for (int i = 0; i < sys->numberHackers; i++){

            if (sys->hackers[i] != NULL){

                HackerDestroy(sys->hackers[i]);
            }
        }

        free(sys->hackers);

    }

    free(sys);
    return;

}

