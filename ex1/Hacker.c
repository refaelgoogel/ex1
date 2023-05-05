#include "Hacker.h"
#include "Student.h"

Hacker HackerCreate(Student *students,char **fileLine){

    if (fileLine == NULL || strlen(fileLine) == 0 || students == NULL) {
		
		return NULL;
	}

    Hacker new_hacker = malloc(sizeof(*new_hacker));

    if (new_hacker == NULL){
    
        return NULL;
    }

    char* first_line = fileLine[0]; 
	
	char *token = strtok(fileLine[0], " ");
		
	if (token == NULL){return NULL;}

    int i = 0; 
    bool flag = false;

    while (students[i] != NULL){

        if (strcmp(students[i]->studentID, token) == 0){
            
            new_hacker->hacker = students[i];
            flag = true;
            break;
        }

        i++;
    }

    if (!flag){

        return NULL;
    }

    // now we have the student

    // le'ts get the wanted courses

    // count the number of wanted courses

    char *secondLine = fileLine[1];
    int numberWantedCourses = 0;

    token = strtok(secondLine, " ");

    while (token != NULL && strlen(token) > 0){

        numberWantedCourses++;
        token = strtok(NULL, " ");
    }

    secondLine = fileLine[1];

    new_hacker->wantedCoursesID = malloc(sizeof(char*)*(numberWantedCourses+1));

    if (new_hacker->wantedCoursesID == NULL){

        return NULL;
    }

    new_hacker->wantedCoursesID[numberWantedCourses] = NULL;
    new_hacker->wantedCoursesNum = numberWantedCourses;

    token = strtok(secondLine, " ");

    while (token != NULL && strlen(token) > 0){

        new_hacker->wantedCoursesID[i] = token;

        token = strtok(NULL, " ");
    }


    // now we have the wanted courses ID and the number of wanted courses

    // now let's find the hacker's friends

    char *thirdLine = fileLine[2];

    int numberFriends = 0;

    token = strtok(thirdLine, " ");

    while (token != NULL && strlen(token) > 0){

        numberFriends++;
        token = strtok(NULL, " ");
    }

    thirdLine = fileLine[2];

    new_hacker->friends = malloc(sizeof(Student)*(numberFriends+1));

    if (new_hacker->friends == NULL){

        return NULL;
    }

    new_hacker->friends[numberFriends] = NULL;

    new_hacker->numberFriend = numberFriends;

    token = strtok(thirdLine, " ");

    flag = false;

    int i = 0;

    while (token != NULL && strlen(token) > 0){

        int j = 0;
        bool flag = false;

        while (students[j] != NULL){

            if (strcmp(students[j]->studentID, token) == 0){
                
                new_hacker->friends[i++] = students[j];
                flag = true;
                break;
            }

            j++;
        }

        if (!flag){

            return NULL;
        }

        flag = false;
        token = strtok(NULL, " ");
    }

    // now we have the hacker's friends

    // now let's find the hacker's enemies

    char *fourthLine = fileLine[3];

    int numberRivals = 0;

    token = strtok(thirdLine, " ");

    while (token != NULL && strlen(token) > 0){

        numberRivals++;
        token = strtok(NULL, " ");
    }

    fourthLine = fileLine[3];

    new_hacker->rivals = malloc(sizeof(Student)*(numberRivals+1));

    if (new_hacker->rivals == NULL){

        return NULL;
    }

    new_hacker->rivals[numberRivals] = NULL;
    new_hacker->numberRival = numberRivals;

    token = strtok(thirdLine, " ");

    flag = false;
    i = 0;

    while (token != NULL && strlen(token) > 0){

        int j = 0;
        bool flag = false;

        while (students[j] != NULL){

            if (strcmp(students[j]->studentID, token) == 0){
                
                new_hacker->rivals[i++] = students[j];
                flag = true;
                break;
            }

            j++;
        }

        if (!flag){

            return NULL;
        }

        flag = false;
        token = strtok(NULL, " ");
    }

    // now we have the hacker's enemies

    new_hacker->gotCourseNum = 0;

    return new_hacker;

}

Hacker update

void HackerDestroy(Hacker hacker){

    if (hacker == NULL){

        return;
    }

    free(hacker->wantedCoursesID);
    free(hacker->friends);
    free(hacker->rivals);

    free(hacker);
}