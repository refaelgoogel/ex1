#include "CourseQueue.h"
#include "Hacker.h"


CourseQueue CourseQueueCreate(char *fileLine){
	
	if (fileLine == NULL || strlen(fileLine) == 0) {
		
		return NULL;
	}
	
	CourseQueue new_CourseQueue = (CourseQueue)malloc(sizeof(*new_CourseQueue));

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
	new_CourseQueue->currentSize = 0;

	// 
	new_CourseQueue->studentQueue = (IsraeliQueue)malloc(sizeof(IsraeliQueue));
    
	if (new_CourseQueue->studentQueue == NULL){return NULL;}

	return new_CourseQueue;
	
}


CourseQueue InsertStudentsToCourseQueue(CourseQueue courseQueue, char *fileLine, Student *students, int studentNum){

	if (courseQueue == NULL || fileLine == NULL || strlen(fileLine) == 0 || students == NULL) {
		
		return NULL;
	}

	printf("file line is %s\n", fileLine);

	char *token = strtok(fileLine, " ");
		
	if (token == NULL || strlen(token) == 0){return NULL;}

	// now the token has the courseID

	// let's get the students ID one by one 

	bool foundStudent = false;

	token = strtok(NULL, " "); // get the first student

	while (token != NULL && strlen(token) > 0){ // running on all over the studentId
		
		for (int i = 0; i < studentNum; i++){ // running on all over the students

			FixToken(students[i]->studentID);
			FixToken(token);

			printf("student id is %s\n", students[i]->studentID);
			printf("token is %s\n", token);

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
    
	IsraeliQueueDestroy(curseQueue->studentQueue);
}


void PrintCourse(CourseQueue curseQueue){

	printf("--------Course print---------\n");

	printf("courseID: %s\n",curseQueue->courseID);
	printf("course size: %d\n",curseQueue->courseSize);
	printf("currentSize: %d\n",curseQueue->currentSize);
	PrintIsraeliQueue(curseQueue->studentQueue);
	

	printf("-------- End of course---------\n");
	
}
