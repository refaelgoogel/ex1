#include "Hacker.h"
#include "Student.h"


Hacker HackerCreate(Student *students,char **fileLine){

    if (fileLine == NULL || students == NULL) {
		
		return NULL;
	}

    Hacker new_hacker = (Hacker)malloc(sizeof(*new_hacker));

    if (new_hacker == NULL){

        return NULL;
    }

    char* first_line = fileLine[0]; 

	
	char *token = strtok(first_line, " ");

		
	if (token == NULL){return NULL;}

    FixToken(token);

    int i = 0; 
    bool flag = false;

    while (students[i] != NULL){

        if (strcmp(students[i]->studentID, token) == 0){
            
            new_hacker->hacker = students[i];
            new_hacker->hacker->ifHacker = true;
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
    char *secondLineCopy = (char*)malloc(sizeof(char)*(strlen(secondLine)+1));
    strcpy(secondLineCopy, secondLine);

    if (secondLineCopy == NULL){

        return NULL;
    }

    int numberWantedCourses = 0;

    token = strtok(secondLine, " ");
    FixToken(token);

    while (token != NULL && strlen(token) > 0){

        numberWantedCourses++;
        token = strtok(NULL, " ");
    }

    secondLine = secondLineCopy;
    printf("second line is %s\n", secondLine);

    new_hacker->wantedCoursesID = (char**)malloc(sizeof(char*)*(numberWantedCourses+1));

    if (new_hacker->wantedCoursesID == NULL){

        return NULL;
    }

    new_hacker->wantedCoursesID[numberWantedCourses] = NULL;
    new_hacker->wantedCoursesNum = numberWantedCourses;

    token = strtok(secondLine, " ");


    i = 0;

    while (token != NULL && strlen(token) > 0){

        FixToken(token);
        new_hacker->wantedCoursesID[i] = token;
        token = strtok(NULL, " ");
        i++;
    }

    free (secondLineCopy);
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

    new_hacker->friends = (Student*)malloc(sizeof(Student)*(numberFriends+1));

    if (new_hacker->friends == NULL){

        return NULL;
    }

    new_hacker->hacker->friendsID = (char**)malloc(sizeof(char*)*(numberFriends+1));

    if (new_hacker->hacker->friendsID == NULL){

        return NULL;
    }

    new_hacker->friends[numberFriends] = NULL;
    new_hacker->hacker->friendsID[numberFriends] = NULL;

    new_hacker->numberFriend = numberFriends;
    new_hacker->hacker->numberFriend = numberFriends;

    token = strtok(thirdLine, " ");

    flag = false;

    i = 0;

    while (token != NULL && strlen(token) > 0){

        int j = 0;

        FixToken(token);
        bool flag = (strlen(token) == 0);

        while (students[j] != NULL && strlen(token) > 0){

            if (strcmp(students[j]->studentID, token) == 0){
                
                new_hacker->friends[i] = students[j];
                new_hacker->hacker->friendsID[i++] = students[j]->studentID;
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

    token = strtok(fourthLine, " ");
    FixToken(token);

    //printf("length of token %s is %d\n", token, strlen(token));
    //printf("tokennnnnnnnnnnnn: %s\n", token);

    while (token != NULL && strlen(token) > 0){

        numberRivals++;
        token = strtok(NULL, " ");
    }

    fourthLine = fileLine[3];

    new_hacker->rivals = (Student*)malloc(sizeof(Student)*(numberRivals+1));

    if (new_hacker->rivals == NULL){

        
        return NULL;
    }

    new_hacker->hacker->rivalsID = (char**)malloc(sizeof(char*)*(numberRivals+1));

    if (new_hacker->hacker->rivalsID == NULL){

        return NULL;
    }

    

    new_hacker->rivals[numberRivals] = NULL;
    new_hacker->hacker->rivalsID[numberRivals] = NULL;

    new_hacker->numberRival = numberRivals;
    new_hacker->hacker->numberRival = numberRivals;


    token = strtok(fourthLine, " ");

    flag = false;
    i = 0;

    while (token != NULL && strlen(token) > 0){

        int j = 0;
        FixToken(token);
        bool flag = (strlen(token) == 0);

        while (students[j] != NULL && strlen(token) > 0){

            if (strcmp(students[j]->studentID, token) == 0){
                
                new_hacker->rivals[i] = students[j];
                new_hacker->hacker->rivalsID[i++] = students[j]->studentID;
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

    // hacker created! 

    printf("hacker created!\n");    
    
    return new_hacker;

}

void HackerDestroy(Hacker hacker){

    if (hacker == NULL){

        return;
    }

    free(hacker->wantedCoursesID);
    free(hacker->friends);
    free(hacker->rivals);

    free(hacker);
}

void FixToken(char *token){

    if (token == NULL || strlen(token) == 0){

        return;
    }

    if (token[strlen(token)-1] == 10){

        token[strlen(token)-1] = '\0';
            
    }else if (token[0] == 10){

        token[0] = '\0';
    }
}

void PrintHacker(Hacker hacker){


    printf("\n--------HackerPrint---------------------------\n");


    PrintStudent(hacker->hacker);


    printf("\n -------- hacker's friends:-----------------\n");

    for (int i = 0; i<hacker->numberFriend; i++){

        PrintStudent(hacker->friends[i]);
    }

    printf("\n ------ end of friends--------------------\n");


    printf("\n ----------- rivals----------------------\n");

    for (int i = 0; i<hacker->numberRival; i++){

        PrintStudent(hacker->rivals[i]);
    }
    printf("\n ----------- end of rivals-----------------\n");

    printf("\n ----------------- wanted courses number: %d --------- \n",hacker->wantedCoursesNum);

    printf("\n --------  %s wanted courses: ----------------------------------\n",hacker->hacker->studentID);

    for (int i = 0; i<hacker->wantedCoursesNum; i++){

        printf("%d. %s\n" ,i+1,hacker->wantedCoursesID[i]);
    }

    printf("\n ------------ end of wanted courses: ------------\n");

    printf("\n currently got from wanted: %d\n",hacker->gotCourseNum);

    printf("\n--------End of Hacker----------------------\n");


}
