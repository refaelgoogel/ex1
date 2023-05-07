#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "HackerEnrollment.h"


int main(int argc, char *argv[]){

    if (argc > 7 || argc < 6){ printf("bad parameters\n"); return 1;}

    bool nameFlag = false;
    char *studentsPath = NULL, *coursesPath = NULL, *hackersPath = NULL, *queuesPath = NULL, *targetPath = NULL;

    if (strcmp(argv[1], "-i") == 0){nameFlag = true;}

    if (nameFlag){

        for (int i = 2; i < argc; i++){

            switch (i){

                case 2:
                    studentsPath = argv[i];
                    break;
                case 3:
                    coursesPath = argv[i];
                    break;
                case 4:
                    hackersPath = argv[i];
                    break;
                case 5:
                    queuesPath = argv[i];
                    break;
                case 6:
                    targetPath = argv[i];
                    break;
                default:
                    break;
            }
        }

    }else{

        for (int i = 1; i < argc; i++){

            switch (i){

                case 1:
                    studentsPath = argv[i];
                    break;
                case 2:
                    coursesPath = argv[i];
                    break;
                case 3:
                    hackersPath = argv[i];
                    break;
                case 4:
                    queuesPath = argv[i];
                    break;
                case 5:
                    targetPath = argv[i];
                    break;
                default:
                    break;
            }
        }

    }

    // Check if all arguments have been provided
    if (!studentsPath || !coursesPath || !hackersPath || !queuesPath || !targetPath) {

        // error
        return 1;
    }

    // Open files

    FILE *studentsFile = fopen(studentsPath, "r");
    FILE *coursesFile = fopen(coursesPath, "r");
    FILE *hackersFile = fopen(hackersPath, "r");
    FILE *queuesFile = fopen(queuesPath, "r");
    FILE *targetFile = fopen(targetPath, "w");

    if (studentsFile == NULL || coursesFile == NULL || hackersFile == NULL || queuesFile == NULL || targetFile == NULL) {

        // error
        return 1;
    }

    enrollmentSystem enSystem = createEnrollmentSystem(studentsFile, coursesFile, hackersFile);
    printf("enSystem created\n");
    if (enSystem == NULL){

        // error
        return 1;
    }


    SetNameFlag(enSystem, nameFlag);

    printf("nameFlag set\n");

    readEnrollment(enSystem, queuesFile);
    printf("enrollment read\n");
    
    hackEnrollment(enSystem, targetFile);
    printf("enrollment hacked\n");

    // close files 

    int result1 = fclose(studentsFile);
    int result2 = fclose(coursesFile);
    int result3 = fclose(hackersFile);
    int result4 = fclose(queuesFile);
    int result5 = fclose(targetFile);

    if (result1 == EOF || result2 == EOF || result3 == EOF || result4 == EOF || result5 == EOF){

        // error
        return 1;
    }

    // free things
    free(enSystem);


    return 0;
}



