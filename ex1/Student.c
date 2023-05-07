#include "Student.h"


Student studentCreate(char *fileLine){

  if (fileLine == NULL || strlen(fileLine) == 0) {
        
    return NULL;

  }

  Student new_student = malloc(sizeof(*new_student));

  if (new_student == NULL){

    return NULL;

  }

  char *token = strtok(fileLine, " ");
        
  if (token == NULL){return NULL;}
    
  new_student->studentID = token;

  token = strtok(NULL, " ");

  if (token == NULL || atoi(token) < 0 ){return NULL;}

  new_student->totalCredits = atoi(token);

  token = strtok(NULL, " ");

  if (token == NULL || atoi(token) < 0 || atoi(token) > 100 ){return NULL;}
    
  new_student->gpa = atoi(token);

  token = strtok(NULL, " ");

  if (token == NULL ){return NULL;}

  new_student->name = token;

  token = strtok(NULL, " ");

  if (token == NULL ){return NULL;}

  new_student->surname = token;

  token = strtok(NULL, " ");

  if (token == NULL ){return NULL;}

  new_student->city = token;

  token = strtok(NULL, " ");

  if (token == NULL ){return NULL;}

  new_student->department = token;

  new_student->ifHacker = false;
  new_student->friendsID = NULL;
  new_student->numberFriend = 0;
  new_student->rivalsID = NULL;
  new_student->numberRival = 0;

  return new_student;

}   

void studentDestroy(Student student){
  
    if (student == NULL) return;

    free(student->friendsID);
    free(student->rivalsID);

    return; // this function is empty because we don't need to free the memory of the student struct
}

void PrintStudent(Student student){

  printf("\n--------Student print---------\n");

  printf("studentID: %s, name: %s, surname: %s\n", student->studentID,student->name,student->surname);
  //printf("totalCredits: %d\n", student->totalCredits);
  //printf("gpa: %d\n", student->gpa);
  //printf("name: %s\n", student->name);
  //printf("surname: %s\n", student->surname);
  //printf("city: %s\n", student->city);
  //printf("department: %s\n", student->department); 

  printf("-------- End of student---------\n");

}

