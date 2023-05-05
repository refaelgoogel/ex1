#include "HackerEnrollment.h"
#define FRIENDSHIP_THRESHOLD 20
#define RIVAL_THRESHOLD -20
#define NEUTRAL_THRESHOLD 0


int friendshipMeasureByFile(void *hacker, void* student);
int friendshipMeasureByID(void *hacker, void* student);
int friendshipMeasureByName(void *hacker, void* student);
int ItemsComparisonFunction(void *student1, void *student2);
int returnDiffAsciiName(char* hackerName, char* studentName);



enrollmentSystem createEnrollmentSystem(FILE* students, FILE* courses, FILE* hackers){

    enrollmentSystem new_system = malloc(sizeof(*new_system));

    if (new_system == NULL){

        return NULL;
    }

    // malloc OK

    new_system->courses_file = courses;
    new_system->students_file = students;
    new_system->hackers_file = hackers;
   
    int numberCourses = GetLineNumber(courses);
    int numberStudents = GetLineNumber(students);

    int numberHackers = GetLineNumber(hackers);
    numberHackers = numberHackers/4; // because each hacker has 4 lines

    new_system->coursesQueue = malloc(sizeof(CourseQueue)*(numberCourses+1));

    if (new_system->coursesQueue == NULL){

        return NULL;
    }

    new_system->coursesQueue[numberCourses] = NULL;

    // creting CourseQueue

    new_system->coursesQueue = readFileCourses(new_system->courses_file, numberCourses, new_system->coursesQueue);

    if (new_system->coursesQueue == NULL){

        return NULL;
    }

    // creating the FunctionArr inside the courseQueue (array of function pointers, argument of IsraeliQueueCreate)

    FriendshipFunction *FunctionArr = malloc(sizeof(FunctionArr)*4);

    if (FunctionArr == NULL){

        return NULL;
    }

    FunctionArr[0] = &friendshipMeasureByFile;
    FunctionArr[1] = &friendshipMeasureByID;
    FunctionArr[2] = &friendshipMeasureByName;
    FunctionArr[3] = NULL;

    // creating the comparison function inside the courseQueue (argument of IsraeliQueueCreate)

    ComparisonFunction comp = &ItemsComparisonFunction;

    // creating the israeliQueue inside the courseQueue 

    for (int i = 0; i < numberCourses; i++){

        new_system->coursesQueue[i]->studentQueue = IsraeliQueueCreate(FunctionArr, comp, FRIENDSHIP_THRESHOLD, RIVAL_THRESHOLD);

        if (new_system->coursesQueue[i]->studentQueue == NULL){

            return NULL;
        }

    }

    // finished creating CourseQueue

    
    // creating the students array inside the enrollmentSystem

    new_system->students = malloc(sizeof(Student)*(numberStudents+1));

    if (new_system->students == NULL){

        return NULL;
    }

    new_system->students[numberStudents] = NULL;


    new_system->hackers = malloc(sizeof(Hacker)*(numberHackers+1));

    if (new_system->hackers == NULL){

        return NULL;
    }

    new_system->hackers[numberHackers] = NULL;

    // malloc OK

    

    new_system->students = readFileStudent(new_system->students_file, numberStudents, new_system->students);

    if (new_system->students == NULL){

        return NULL;
    }

    new_system->hackers = readFileHacker(new_system->hackers_file, numberHackers, new_system->hackers);

    if (new_system->hackers == NULL){

        return NULL;
    }

    return new_system;
     
 }

 CourseQueue* readFileCourses(FILE *coursesFile, int coursesNum, CourseQueue *coursesQueue){

  if (coursesFile == NULL || coursesQueue == NULL) 
  {
      return NULL;
  }

  char *fileLine = NULL;
  size_t len = 0; // size_t is an unsigned integer data type

  fopen(coursesFile, "r");

  if (coursesFile == NULL){

    return NULL;
  }


  for (int i = 0; i < coursesNum; i++){
      
    if ((getline(&fileLine, &len, coursesFile)) != -1){

      coursesQueue[i] = CourseQueueCreate(fileLine);
      fileLine = NULL;
      len = 0;
    
    }else {

      return NULL;
    }
  }

  fclose(coursesFile);
  free(fileLine); // COULD BE ERROR: maybe not need to free the memory - still need to check

  return coursesQueue;

 }

Student* readFileStudent(FILE *studentFile, int studentNum, Student *students){

  if (studentFile == NULL || students == NULL) 
  {
      return NULL;
  }

  char *fileLine = NULL;
  size_t len = 0;

  fopen(studentFile, "r");
  
  if (studentFile == NULL){
    
    return NULL;
  }

  for (int i = 0; i < studentNum; i++){
      
    if ((getline(&fileLine, &len, studentFile)) != -1){

      students[i] = StudentCreate(*fileLine);
      fileLine = NULL;
      len = 0;

    }else{

      return NULL;
    } 

   }

   fclose(studentFile);
   free(fileLine);// COULD BE ERROR: maybe not need to free the memory - still need to check

   return students;
 }

Hacker* readFileHackers (FILE *hackerFile, int HackerNum, Hacker *hackers, Student *students){

  if (hackerFile == NULL || hackers == NULL) 
  {
      return NULL;
  }

  char *fileLine = NULL;
  size_t len = 0;
  char *hackerLines[5] = {NULL};

  fopen(hackerFile, "r");

  if (hackerFile == NULL){

    return NULL;
  }

  for (int i = 0; i < HackerNum; i++){ // hackerNum divided by 4 because each hacker has 4 lines
      
    for (int j=0; j<4; j++){

        if ((getline(&fileLine, &len, hackerFile)) != -1){

          hackerLines[j] = fileLine;
          fileLine = NULL;
          len = 0;

      }else {

        return NULL;
      }


    } 

    hackers[i] = HackerCreate(hackerLines, students);

  }

  fclose(hackerFile);
  free(fileLine);
  return hackers;

 }

int GetLineNumber(FILE *file){
	
	char *line = NULL;
    unsigned int len = 0;
    int read = 0;
	int counter = 0; 

    while ((read = getline(&line, &len, file)) != -1) {
		
		// exist line 
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

    fopen(queues, "r");

    while ((getline(&fileLine, &len, queues)) != -1){


        if (strlen(fileLine) == 0){

          break;
        }

        char *courseNum = strtok(fileLine, " ");

        int i = 0;
        
        int indexOfCourse = returnIndexOfCourseByID(sys->coursesQueue, courseNum);

        if (indexOfCourse == -1){

            return NULL;
        }
        
        if (InsertStudentsToCourseQueue(sys->coursesQueue[indexOfCourse], fileLine, sys->students) == NULL){

            return NULL;
        }
        
    }

    fclose(queues);
    free(fileLine); // COULD BE ERROR: maybe not need to free the memory - still need to check

    return sys;
}

void hackEnrollment(enrollmentSystem sys, FILE* out){

    if (sys == NULL || out == NULL){

        return;
    }

    int i = 0;

    while (sys->hackers[i]){

        if (getHackerInToTheRequiredCourses(sys->coursesQueue , sys->hackers[i]) == NULL){

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

                fopen(out,"w" );
                fprintf(out,"Cannot satisfy constraints for %s",sys->hackers[i]->hacker->studentID);
                fclose(out);
                return;//

            }else if (compared == -1){

                return;//
            }

        }

        i++;

    }
	
    fopen(out,"w");

    for (i = 0; i < sys->numberCourses; i++){

        fprintf(out, "%s ",sys->coursesQueue[i]->courseID);

        for (j = 0; j < sys->coursesQueue[i]->studentQueue->size; j++){

            Student student = (Student)(sys->coursesQueue[i]->studentQueue->array[j]->data);
            fprintf(out, "%s ",student->studentID);
       
        }

        fprintf(out, "\n");

    }
   
	fclose(out);

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

        Student student = (Student)(courseQueue->studentQueue->array[i]->data);

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

        if (IsraeliQueueEnqueue(courseQueue[coursePlace]->studentQueue, hacker) == NULL){

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
    
}



