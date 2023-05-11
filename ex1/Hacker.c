#include "Hacker.h"
#include "Student.h"
#include <string.h>


Hacker FirstLineFunction(Hacker new_hacker ,char *firstLine,int studentNum, Student *students);
Hacker SecondLineFunction(Hacker new_hacker ,char *secondLine);
Hacker ThirdLineFunction(Hacker new_hacker ,char *thirdLine, Student *students, int studentNum);
Hacker FourthLineFunction(Hacker new_hacker ,char *fourthLine, Student *students, int studentNum);
int GetNumberSafe(char *token);

// this function creates a new hacker with the wanted courses, friends and rivals using the functions below
Hacker HackerCreate(Student *students,char **fileLine, int studentNum){

    if (fileLine == NULL || students == NULL){
		
		return NULL;
	}

    Hacker new_hacker = (Hacker)malloc(sizeof(*new_hacker));

    if (new_hacker == NULL){

        return NULL;
    }

    char* first_line = fileLine[0]; 

    new_hacker = FirstLineFunction(new_hacker, first_line, studentNum , students);

    if (new_hacker->hacker == NULL){

        return NULL;
    }
	
    // now we have the student
    // le'ts get the wanted courses
    // count the number of wanted courses

    char *secondLine = fileLine[1];

    new_hacker = SecondLineFunction(new_hacker, secondLine);

    if (new_hacker == NULL){

        return NULL;
    }

    // now we have the wanted courses ID and the number of wanted courses
    // now let's find the hacker's friends

    char *thirdLine = fileLine[2];

    new_hacker = ThirdLineFunction(new_hacker, thirdLine, students, studentNum);

    if (new_hacker == NULL){

        return NULL;
    }

    //free (thirdLineCopy);
    // now we have the hacker's friends
    // now let's find the hacker's enemies

    char *fourthLine = fileLine[3];

    new_hacker = FourthLineFunction(new_hacker, fourthLine, students, studentNum);

    if (new_hacker == NULL){

        return NULL;
    }

    // now we have the hacker's enemies

    new_hacker->gotCourseNum = 0;

    // hacker created! 

    return new_hacker;

}

// this function gets hacker ID
Hacker FirstLineFunction(Hacker new_hacker ,char *firstLine, int studentNum, Student *students){

    if (firstLine == NULL || new_hacker == NULL){

        return NULL;
    }

    char *CopyFirstLine = (char*)malloc(sizeof(char)*(strlen(firstLine)+1));

    if (CopyFirstLine == NULL){

        return NULL;
    }

    strcpy(CopyFirstLine, firstLine);

    if (CopyFirstLine == NULL){

        return NULL;
    }

    char *token = strtok(CopyFirstLine, " ");
		
	if (token == NULL){

        free(CopyFirstLine);
        return NULL;
    }

    trim(token);

    int i = 0;
    bool flag = false;

    for (i = 0; i < studentNum; i++){
        
        if (strcmp(students[i]->studentID, token) == 0){
            
            new_hacker->hacker = students[i];
            new_hacker->hacker->ifHacker = true;
            flag = true;
            break;
        }
    }

    if (!flag){

        free(CopyFirstLine);
        return NULL;
    }

    free(CopyFirstLine);
    return new_hacker;
}

// this function gets the wanted courses
Hacker SecondLineFunction(Hacker new_hacker, char *SecondLine){


    if (SecondLine == NULL || new_hacker == NULL){

        return NULL;
    }

    if (strlen(SecondLine) == 0){
        
        new_hacker->wantedCoursesID = NULL;
        new_hacker->wantedCoursesNum = 0;
        return new_hacker;
    }

    trim(SecondLine);

    char *CopySecondLine = (char*)malloc(sizeof(char)*(strlen(SecondLine)+1));

    if (CopySecondLine == NULL){

        return NULL;
    }

    strcpy(CopySecondLine, SecondLine);

    if (CopySecondLine == NULL){

        return NULL;
    }

    int numberWantedCourses = GetNumberSafe(CopySecondLine);

    if (numberWantedCourses == 0){

        new_hacker->wantedCoursesID = NULL;
        new_hacker->wantedCoursesNum = 0;
        free(CopySecondLine);
        return new_hacker;
    }

    if (numberWantedCourses == -1){

        free(CopySecondLine);
        return NULL;
    }

    new_hacker->wantedCoursesID = (char**)malloc(sizeof(char*)*(numberWantedCourses+1));

    if (new_hacker->wantedCoursesID == NULL){

        free(CopySecondLine);
        return NULL;
    }

    char *token = strtok(CopySecondLine, " ");

    if (token == NULL){

        free(CopySecondLine);
        return NULL;
    }

    int i = 0;

    while (token != NULL){

        trim(token);

        new_hacker->wantedCoursesID[i] = (char*)malloc(sizeof(char)*(strlen(token)+1));

        if (new_hacker->wantedCoursesID[i] == NULL){

            free(CopySecondLine);
            return NULL;
        }

        strcpy(new_hacker->wantedCoursesID[i], token);

        if (new_hacker->wantedCoursesID[i] == NULL){

            free(CopySecondLine);
            return NULL;
        }
   
        token = strtok(NULL, " ");
        i++;
    }

    new_hacker->wantedCoursesID[i] = NULL;
    new_hacker->wantedCoursesNum = i;

    free(CopySecondLine);
    return new_hacker;
}

// this function gets the hacker's friends
Hacker ThirdLineFunction(Hacker new_hacker ,char *thirdLine, Student *students, int studentNum){

    if (thirdLine == NULL || new_hacker == NULL || students == NULL){

        return NULL;
    }

    if (strlen(thirdLine) == 0){

        new_hacker->friends = NULL;
        new_hacker->numberFriend = 0;
    
        new_hacker->hacker->friendsID = NULL;
        new_hacker->hacker->numberFriend = 0;
        
        return new_hacker;
    }

    char *CopyThirdLine = (char*)malloc(sizeof(char)*(strlen(thirdLine)+1));
    if (CopyThirdLine == NULL){

        return NULL;
    }
    strcpy(CopyThirdLine, thirdLine);

    int numberFriends = GetNumberSafe(CopyThirdLine);

    if (numberFriends == -1){

        free(CopyThirdLine);
        return NULL;
    }

    if (numberFriends == 0){

        new_hacker->friends = NULL;
        new_hacker->numberFriend = 0;
    
        new_hacker->hacker->friendsID = NULL;
        new_hacker->hacker->numberFriend = 0;

        free(CopyThirdLine);
        return new_hacker;
    }

    new_hacker->friends = (Student*)malloc(sizeof(Student)*(numberFriends+1));

    if (new_hacker->friends == NULL){

        free(CopyThirdLine);
        return NULL;
    }

    new_hacker->hacker->friendsID = (char**)malloc(sizeof(char*)*(numberFriends+1));

    if (new_hacker->hacker->friendsID == NULL){

        free(CopyThirdLine);
        return NULL;
    }

    char *token = NULL;

    token = strtok(CopyThirdLine, " ");

    if (token == NULL){

        free(CopyThirdLine);
        return NULL;
    }

    bool flag = false;
    int i = 0;

    while (token != NULL && strlen(token) > 0){

        int j = 0;

        trim(token);

        flag = (strlen(token) == 0);

        for (; j<studentNum; j++){

            if (strcmp(students[j]->studentID, token) == 0){
                
                new_hacker->friends[i] = students[j];
                new_hacker->hacker->friendsID[i] = (char*)malloc(sizeof(char)*(strlen(students[j]->studentID)+1));

                if (new_hacker->hacker->friendsID[i] == NULL){

                    free(CopyThirdLine);
                    return NULL;
                }

                strcpy(new_hacker->hacker->friendsID[i], students[j]->studentID);

                if (new_hacker->hacker->friendsID[i] == NULL){

                    free(CopyThirdLine);
                    return NULL;
                }

                i++;
                flag = true;
                break;
            }

        }

        if (!flag){

            free(CopyThirdLine);
            return new_hacker;
        }

        flag = false;
        token = strtok(NULL, " ");
    }

    new_hacker->friends[i] = NULL;
    new_hacker->hacker->friendsID[i] = NULL;
    new_hacker->numberFriend = i;
    new_hacker->hacker->numberFriend = i;

    free(CopyThirdLine);
    return new_hacker;
    
}

// this function gets the hacker's rivals
Hacker FourthLineFunction(Hacker new_hacker, char *fourthLine, Student* students, int studentNum){

if (fourthLine == NULL || new_hacker == NULL || students == NULL){

        return NULL;
    }

    if (strlen(fourthLine) == 0){

        new_hacker->rivals = NULL;
        new_hacker->numberRival = 0;
    
        new_hacker->hacker->rivalsID = NULL;
        new_hacker->hacker->numberRival = 0;
        
        return new_hacker;
    }

    char *CopyFourthLine = (char*)malloc(sizeof(char)*(strlen(fourthLine)+1));
    if (CopyFourthLine == NULL){

        return NULL;
    }
    strcpy(CopyFourthLine, fourthLine);

    int numberRivals = GetNumberSafe(CopyFourthLine);

    if (numberRivals == -1){

        free(CopyFourthLine);
        return NULL;
    }

    if (numberRivals == 0){

        new_hacker->rivals = NULL;
        new_hacker->numberRival = 0;
    
        new_hacker->hacker->rivalsID = NULL;
        new_hacker->hacker->numberRival = 0;

        free(CopyFourthLine);
        return new_hacker;
    }

    new_hacker->rivals = (Student*)malloc(sizeof(Student)*(numberRivals+1));

    if (new_hacker->rivals == NULL){

        free(CopyFourthLine);
        return NULL;
    }

    new_hacker->hacker->rivalsID = (char**)malloc(sizeof(char*)*(numberRivals+1));

    if (new_hacker->hacker->rivalsID == NULL){

        free(CopyFourthLine);
        return NULL;
    }

    char *token = NULL;

    token = strtok(CopyFourthLine, " ");

    if (token == NULL){

        free(CopyFourthLine);
        return NULL;
    }

    bool flag = false;
    int i = 0;

    while (token != NULL && strlen(token) > 0){

        int j = 0;

        trim(token);

        flag = (strlen(token) == 0);

        for (; j<studentNum; j++){

            if (strcmp(students[j]->studentID, token) == 0){
                
                new_hacker->rivals[i] = students[j];
                new_hacker->hacker->rivalsID[i] = (char*)malloc(sizeof(char)*(strlen(students[j]->studentID)+1));

                if (new_hacker->hacker->rivalsID[i] == NULL){

                    free(CopyFourthLine);
                    return NULL;
                }

                strcpy(new_hacker->hacker->rivalsID[i], students[j]->studentID);

                if (new_hacker->hacker->rivalsID[i] == NULL){

                    free(CopyFourthLine);
                    return NULL;
                }

                i++;
                flag = true;
                break;
            }

        }

        if (!flag){

            free(CopyFourthLine);
            return new_hacker;
        }

        flag = false;
        token = strtok(NULL, " ");
    }

    new_hacker->rivals[i] = NULL;
    new_hacker->hacker->rivalsID[i] = NULL;
    new_hacker->numberRival = i;
    new_hacker->hacker->numberRival = i;

    free(CopyFourthLine);
    return new_hacker;
}

// this function gets number of words in line
int GetNumberSafe(char *line){

    if (line == NULL){

        return -1;
    }

    char *lineCopy = (char*)malloc(sizeof(char)*(strlen(line)+1));
    strcpy(lineCopy, line);

    if (lineCopy == NULL){

        return -1;
    }
    
    int number = 0;

    char* token = strtok(lineCopy, " ");

    trim(token);

    while (token != NULL && strlen(token) > 0){

        number++;
        token = strtok(NULL, " ");
    }

    free(lineCopy);
    return number;
}


// this function destroys the hacker
void HackerDestroy(Hacker hacker){

    if (hacker == NULL){

        return;
    }

    studentDestroy(hacker->hacker);

    for (int i = 0; i< hacker->wantedCoursesNum; i++){

        free(hacker->wantedCoursesID[i]);
    }
    free(hacker->wantedCoursesID);


    for (int i = 0; i< hacker->numberFriend; i++){

        studentDestroy(hacker->friends[i]);
    }
    free(hacker->friends);

    for (int i = 0; i< hacker->numberRival; i++){

        studentDestroy(hacker->rivals[i]);
    }
    free(hacker->rivals);

    free(hacker);
}

// this function trim unwanted characters from the string
void trim(char* str) {


    int start = 0;
    int end = strlen(str) - 1;

    // Find the first non-whitespace character from the start
    while (str[start] != '\0' && (str[start] == ' ' || str[start] == '\t' || str[start] == '\n')) {
        start++;
    }

    // Find the first non-whitespace character from the end
    while (end >= 0 && (str[end] == ' ' || str[end] == '\t' || str[end] == '\n')) {
        end--;
    }

    // Shift the remaining characters to the beginning of the string
    int length = end - start + 1;
    memmove(str, &str[start], length);
    str[length] = '\0';

}

// this function prints the hacker
void PrintHacker(Hacker hacker){

    printf("\n\n-------- Hacker %s ------------\n",hacker->hacker->studentID);
    PrintStudent(hacker->hacker);


    printf("\nfriends:\n");

    for (int i = 0; i<hacker->numberFriend; i++){

        PrintStudent(hacker->friends[i]);
    }

    printf("\nrivals:\n");

    for (int i = 0; i<hacker->numberRival; i++){

        PrintStudent(hacker->rivals[i]);
    }

    printf("\nwant %d courses:\n",hacker->wantedCoursesNum);

    for (int i = 0; i<hacker->wantedCoursesNum; i++){

        printf("%d. %s\n" ,i+1,hacker->wantedCoursesID[i]);
    }

    printf("\ncurrently courses got: %d\n",hacker->gotCourseNum);

    printf("\n--------EndHacker------------\n\n\n");



}
