#include "CourseQueue.h"


CourseQueue CourseQueueCreate(char *fileLine){
	
	if (fileLine == NULL || strlen(fileLine) == 0) {
		
		return NULL;
	}
	
	CourseQueue new_CourseQueue = malloc(sizeof(*new_CourseQueue));

	if (new_CourseQueue == NULL){
	
		return NULL;
	}

	new_CourseQueue->studentQueue = NULL;
	
	char *token = strtok(fileLine, " ");
		
	if (token == NULL){return NULL;}
		
	new_CourseQueue->courseID = token;
			
    token = strtok(NULL, " ");

	if (token == NULL){return NULL;}

	new_CourseQueue->courseSize = atoi(token); 

	// 

	new_CourseQueue->studentQueue = malloc(sizeof(IsraeliQueue));
    
	if (new_CourseQueue->studentQueue == NULL){return NULL;}

	return new_CourseQueue;
	
}


CourseQueue InsertStudentsToCourseQueue(CourseQueue courseQueue, char *fileLine, Student *students){

	if (courseQueue == NULL || fileLine == NULL || strlen(fileLine) == 0 || students == NULL) {
		
		return NULL;
	}

	char *token = strtok(fileLine, " ");
		
	if (token == NULL || strlen(token) == 0){return NULL;}

	// now the token has the courseID

	// let's get the students ID one by one 

	token = strtok(NULL, " "); // get the first student

	while (token != NULL && strlen(token) > 0){ // running on all over the studentId

		
		int i = 0;

		while (students[i]->studentID != NULL){ // running on all over the students

			if (strcmp(students[i]->studentID, token) == 0){ // we found the student

				IsraeliQueueEnqueue(courseQueue->studentQueue, students[i]); // insert the student to the course queue
				break;
			}

			i++;
		}

		// TODO - what if we didn't find the student?

		token = strtok(NULL, " ");
	}

	return courseQueue;
}

void  CourseQueueDestroy(CourseQueue curseQueue){
    
	IsraeliQueueDestroy(curseQueue->studentQueue);
}


