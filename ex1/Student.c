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

}   


void studentDestroy(Student student){
  
    return; // this function is empty because we don't need to free the memory of the student struct
}