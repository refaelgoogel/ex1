#include "Hacker.h"
#include "Student.h"

Hacker HackerCreate(Student *students,char **fileLine, int studentNum){

    if (fileLine == NULL || students == NULL){
		
		return NULL;
	}

    Hacker new_hacker = (Hacker)malloc(sizeof(*new_hacker));

    if (new_hacker == NULL){

        return NULL;
    }

    char* first_line = fileLine[0]; 

    new_hacker = FirstLineFunction(new_hacker, first_line, studentNum,students);

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

    new_hacker = ThirdLineFunction(new_hacker, thirdLine, students);

    if (new_hacker == NULL){

        return NULL;
    }

    //free (thirdLineCopy);
    // now we have the hacker's friends
    // now let's find the hacker's enemies

    char *fourthLine = fileLine[3];

    new_hacker = FourthLineFunction(new_hacker, fourthLine, students);

    if (new_hacker == NULL){

        return NULL;
    }

    // now we have the hacker's enemies

    new_hacker->gotCourseNum = 0;

    // hacker created! 

    printf("hacker created!\n");    
    
    return new_hacker;

}

Hacker FirstLineFunction(Hacker new_hacker ,char *firstLine, int studentNum, Student *students){

    if (firstLine == NULL || new_hacker == NULL){

        return NULL;
    }

    char *CopyFirstLine = (char*)malloc(sizeof(char)*(strlen(firstLine)+1));

    if (CopyFirstLine == NULL){

        return NULL;
    }

    strcpy(CopyFirstLine, firstLine);

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

Hacker SecondLineFunction(Hacker new_hacker, char *SecondLine){
    
    if (SecondLine == NULL || new_hacker == NULL){

        return NULL;
    }

    if (strlen(SecondLine) == 0){
        
        new_hacker->wantedCoursesID = NULL;
        new_hacker->wantedCoursesNum = 0;
        return new_hacker;
    }

    char *CopySecondLine = (char*)malloc(sizeof(char)*(strlen(SecondLine)+1));

    if (CopySecondLine == NULL){

        return NULL;
    }
    strcpy(CopySecondLine, SecondLine);


    int numberWantedCourses = GetNumberSafe(CopySecondLine);

    if (numberWantedCourses == -1 || numberWantedCourses == 0){

        new_hacker->wantedCoursesID = NULL;
        new_hacker->wantedCoursesNum = 0;
        free(CopySecondLine);
        return NULL;
    }

    new_hacker->wantedCoursesID = (char**)malloc(sizeof(char*)*(numberWantedCourses+1));

    if (new_hacker->wantedCoursesID == NULL){

        free(CopySecondLine);
        return NULL;
    }

    new_hacker->wantedCoursesID[numberWantedCourses] = NULL;
    new_hacker->wantedCoursesNum = numberWantedCourses;


    int i = 0;

    char *token = strtok(CopySecondLine, " ");

    while (token != NULL && strlen(token) > 0){

        printf("token is: %s\n", token);

        new_hacker->wantedCoursesID[i] = (char*)malloc(sizeof(char)*(strlen(token)+1));

        if (new_hacker->wantedCoursesID[i] == NULL){

            free(CopySecondLine);
            return NULL;
        }
        strcpy(new_hacker->wantedCoursesID[i], token);
        printf("new_hacker->wantedCoursesID[i] is: %s\n", new_hacker->wantedCoursesID[i]);

        token = strtok(NULL, " ");
        i++;
    }


    free(CopySecondLine);

    PrintHacker(new_hacker);

    return new_hacker;

}

Hacker ThirdLineFunction(Hacker new_hacker ,char *thirdLine, Student *students){

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

    if (numberFriends == -1 || numberFriends == 0){

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

    new_hacker->friends[numberFriends] = NULL;
    new_hacker->hacker->friendsID[numberFriends] = NULL;

    new_hacker->numberFriend = numberFriends;
    new_hacker->hacker->numberFriend = numberFriends;


    char *token = strtok(CopyThirdLine, " ");

    bool flag = false;
    int i = 0;

    while (token != NULL && strlen(token) > 0){

        int j = 0;

        trim(token);

        flag = (strlen(token) == 0);

        while (students[j] != NULL && strlen(token) > 0){

            if (strcmp(students[j]->studentID, token) == 0){
                
                new_hacker->friends[i] = students[j];
                new_hacker->hacker->friendsID[i] = (char*)malloc(sizeof(char)*(strlen(students[j]->studentID)+1));
                if (new_hacker->hacker->friendsID[i] == NULL){

                    free(CopyThirdLine);
                    return NULL;
                }
                strcpy(new_hacker->hacker->friendsID[i], students[j]->studentID);
                i++;
                flag = true;
                break;
            }

            j++;
        }

        if (!flag){

            free(CopyThirdLine);
            return new_hacker;
        }

        flag = false;
        token = strtok(NULL, " ");
    }

    free(CopyThirdLine);
    return new_hacker;
    
}

Hacker FourthLineFunction(Hacker new_hacker, char *fourthLine, Student* students){

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

    int numberRival = GetNumberSafe(CopyFourthLine);

    if (numberRival == -1 || numberRival == 0){

        new_hacker->rivals = NULL;
        new_hacker->numberRival = 0;
    
        new_hacker->hacker->rivalsID = NULL;
        new_hacker->hacker->numberRival = 0;
        free(CopyFourthLine);
        return new_hacker;
    }

    new_hacker->rivals = (Student*)malloc(sizeof(Student)*(numberRival+1));

    if (new_hacker->rivals == NULL){

        free(CopyFourthLine);
        return NULL;
    }

    new_hacker->hacker->rivalsID = (char**)malloc(sizeof(char*)*(numberRival+1));

    if (new_hacker->hacker->rivalsID == NULL){

        free(CopyFourthLine);
        return NULL;
    }

    new_hacker->rivals[numberRival] = NULL;
    new_hacker->hacker->rivalsID[numberRival] = NULL;

    new_hacker->numberRival = numberRival;
    new_hacker->hacker->numberRival = numberRival;


    char *token = strtok(CopyFourthLine, " ");

    bool flag = false;
    int i = 0;

    while (token != NULL && strlen(token) > 0){

        int j = 0;

        trim(token);

        flag = (strlen(token) == 0);

        while (students[j] != NULL && strlen(token) > 0){

            if (strcmp(students[j]->studentID, token) == 0){
                
                new_hacker->rivals[i] = students[j];
                new_hacker->hacker->rivalsID[i] = (char*)malloc(sizeof(char)*(strlen(students[j]->studentID)+1));
                if (new_hacker->hacker->rivalsID[i] == NULL){

                    free(CopyFourthLine);
                    return NULL;
                }
                strcpy(new_hacker->hacker->rivalsID[i], students[j]->studentID);
                i++;
                flag = true;
                break;
            }

            j++;
        }

        if (!flag){

            free(CopyFourthLine);
            return new_hacker;
        }

        flag = false;
        token = strtok(NULL, " ");
    }

    free(CopyFourthLine);
    return new_hacker;
}

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
    free(line);

    return number;
}


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
