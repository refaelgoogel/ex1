#include "CourseQueue.h"
#include "Hacker.h"


CourseQueue CourseQueueCreate(char *fileLine){
	
	CourseQueue new_CourseQueue = (CourseQueue)malloc(sizeof(*new_CourseQueue));

	if (new_CourseQueue == NULL){
	
		return NULL;
	}

	new_CourseQueue->studentQueue = NULL;

	char *temp = (char*)malloc(sizeof(char)*(strlen(fileLine)+1));
	strcpy(temp, fileLine);

	if (temp == NULL){
	
		return NULL;
	}
	
	char *token = strtok(temp, " ");
		
	if (token == NULL){return NULL;}
	
	new_CourseQueue->courseID = (char*)malloc(sizeof(char)*(strlen(token)+1));

	if (new_CourseQueue->courseID == NULL){return NULL;}
	strcpy(new_CourseQueue->courseID, token);

			
    token = strtok(NULL, " ");

	if (token == NULL){return NULL;}

	new_CourseQueue->courseSize = atoi(token); 
	new_CourseQueue->currentSize = 0;


	new_CourseQueue->studentQueue = (IsraeliQueue)malloc(sizeof(IsraeliQueue));
    
	if (new_CourseQueue->studentQueue == NULL){return NULL;}

	free(temp);

	return new_CourseQueue;
	
}


CourseQueue InsertStudentsToCourseQueue(CourseQueue courseQueue, char *fileLine, Student *students, int studentNum){

	if (courseQueue == NULL || fileLine == NULL || strlen(fileLine) == 0 || students == NULL) {
		
		return NULL;
	}

	char *token = strtok(fileLine, " ");
		
	if (token == NULL || strlen(token) == 0){return NULL;}

	// now the token has the courseID

	// let's get the students ID one by one 

	bool foundStudent = false;

	token = strtok(NULL, " "); // get the first student

	while (token != NULL && strlen(token) > 0){ // running on all over the studentId
		
		for (int i = 0; i < studentNum; i++){ // running on all over the students

			trim(students[i]->studentID);
			trim(token);
		
			if (strcmp(students[i]->studentID, token) == 0){ // we found the student

				IsraeliQueueEnqueue(courseQueue->studentQueue, students[i]); // insert the student to the course queue
				courseQueue->currentSize++;
				foundStudent = true;
				break;
			}

		}

		// TODO - what if we didn't find the student?

		if (!foundStudent){

			return NULL;
		}

		token = strtok(NULL, " ");
	}

	// now we have the courseQueue with all the students by order

	// we finished to insert all the students to the course queue so we can now return the course queue

	return courseQueue;
}

void  CourseQueueDestroy(CourseQueue curseQueue){
    
	if (curseQueue == NULL){return;}

	free(curseQueue->courseID);
	IsraeliQueueDestroy(curseQueue->studentQueue);
}


void PrintCourse(CourseQueue curseQueue){

	printf("\n--------Course print---------\n");

	printf("courseID: %s\n",curseQueue->courseID);
	printf("course size: %d\n",curseQueue->courseSize);
	printf("currentSize: %d\n",curseQueue->currentSize);
	PrintIsraeliQueue(curseQueue->studentQueue);
	

	printf("\n-------- End of course---------\n");
	
}
