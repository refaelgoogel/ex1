#include "Student.h"


Student studentCreate(char *fileLine){

  if (fileLine == NULL || strlen(fileLine) == 0) {
        
    return NULL;

  }

  Student new_student = malloc(sizeof(*new_student));

  if (new_student == NULL){

    return NULL;

  }

  char* temp = (char*)malloc(sizeof(char)*(strlen(fileLine)+1));
  if (temp == NULL){return NULL;}
  strcpy(temp, fileLine);
  if (temp == NULL){return NULL;}

  char *token = strtok(temp, " ");
  if (token == NULL){return NULL;}

  new_student->studentID = malloc(sizeof(char)*(strlen(token)+1));
  if (new_student->studentID == NULL){return NULL;}
  strcpy(new_student->studentID, token);
  if (new_student->studentID == NULL){return NULL;}
    
  token = strtok(NULL, " ");
  if (token == NULL || atoi(token) < 0 ){return NULL;}
  new_student->totalCredits = atoi(token);

  token = strtok(NULL, " ");
  if (token == NULL || atoi(token) < 0 || atoi(token) > 100 ){return NULL;}
  new_student->gpa = atoi(token);

  token = strtok(NULL, " ");
  if (token == NULL ){return NULL;}

  new_student->name = malloc(sizeof(char)*(strlen(token)+1));
  if (new_student->name == NULL){return NULL;}
  strcpy(new_student->name, token);
  if (new_student->name == NULL){return NULL;}

  token = strtok(NULL, " ");
  if (token == NULL ){return NULL;}

  new_student->surname = malloc(sizeof(char)*(strlen(token)+1));
  if (new_student->surname == NULL){return NULL;}
  strcpy(new_student->surname, token);
  if (new_student->surname == NULL){return NULL;}

  token = strtok(NULL, " ");
  if (token == NULL ){return NULL;}
  new_student->city = malloc(sizeof(char)*(strlen(token)+1));
  if (new_student->city == NULL){return NULL;}
  strcpy(new_student->city, token);
  if (new_student->city == NULL){return NULL;}

  token = strtok(NULL, " ");
  if (token == NULL ){return NULL;}
  new_student->department = malloc(sizeof(char)*(strlen(token)+1));
  if (new_student->department == NULL){return NULL;} 
  strcpy(new_student->department, token);
  if (new_student->department == NULL){return NULL;}

  free(temp);

  new_student->ifHacker = false;
  new_student->friendsID = NULL;
  new_student->numberFriend = 0;
  new_student->rivalsID = NULL;
  new_student->numberRival = 0;

  return new_student;

}   

void studentDestroy(Student student){
  
    if (student == NULL) return;

    free(student->studentID);
    free(student->name);
    free(student->surname);
    free(student->city);
    free(student->department);

    for (int i = 0; i < student->numberFriend; i++){

        free(student->friendsID[i]);
    }

    free (student->friendsID);

    for (int i = 0; i < student->numberRival; i++){

        free(student->rivalsID[i]);
    }

    free (student->rivalsID);

    return; // this function is empty because we don't need to free the memory of the student struct
}

void PrintStudent(Student student){

  printf("Student: ID: %s, name: %s, surname: %s\n", student->studentID,student->name,student->surname);
  //printf("totalCredits: %d\n", student->totalCredits);
  //printf("gpa: %d\n", student->gpa);
  //printf("name: %s\n", student->name);
  //printf("surname: %s\n", student->surname);
  //printf("city: %s\n", student->city);
  //printf("department: %s\n", student->department); 
}

