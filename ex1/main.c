#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "HackerEnrollment.h"


int main(int argc, char *argv[]){

	
	/*
	char* I1 = ".\\IsraeliQueue C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test1\\students.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test1\\courses.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test1\\hackers.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test1\\queues.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test1\\out.txt";
	char* I2 = ".\\IsraeliQueue C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test2\\students.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test2\\courses.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test2\\hackers.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test2\\queues.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test2\\out.txt";
	char* I3 = ".\\IsraeliQueue C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test3\\students.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test3\\courses.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test3\\hackers.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test3\\queues.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test3\\out.txt";
	char* I4 = ".\\IsraeliQueue C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test4\\students.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test4\\courses.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test4\\hackers.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test4\\queues.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test4\\out.txt";
	char* I5 = ".\\IsraeliQueue C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test5\\students.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test5\\courses.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test5\\hackers.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test5\\queues.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test5\\out.txt";
	char* I6 = ".\\IsraeliQueue C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test6\\students.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test6\\courses.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test6\\hackers.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test6\\queues.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test6\\out.txt";
	char* I7 = ".\\IsraeliQueue C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test7\\students.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test7\\courses.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test7\\hackers.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test7\\queues.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test7\\out.txt";
	char* I8 = ".\\IsraeliQueue C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test8\\students.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test8\\courses.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test8\\hackers.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test8\\queues.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test8\\out.txt";
	char* I9 = ".\\IsraeliQueue C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test9\\students.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test9\\courses.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test9\\hackers.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test9\\queues.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test9\\out.txt";
	char* I10 = ".\\IsraeliQueue C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test10\\students.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test10\\courses.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test10\\hackers.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test10\\queues.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test10\\out.txt";
	char* I11 = ".\\IsraeliQueue C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test11\\students.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test11\\courses.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test11\\hackers.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test11\\queues.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test11\\out.txt";
	char* I12 = ".\\IsraeliQueue C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test12\\students.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test12\\courses.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test12\\hackers.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test12\\queues.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test12\\out.txt";
	char* I13 = ".\\IsraeliQueue C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test13\\students.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test13\\courses.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test13\\hackers.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test13\\queues.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test13\\out.txt";
	char* I14 = ".\\IsraeliQueue C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test14\\students.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test14\\courses.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test14\\hackers.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test14\\queues.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test14\\out.txt";
	char* I15 = ".\\IsraeliQueue C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test15\\students.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test15\\courses.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test15\\hackers.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test15\\queues.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test15\\out.txt";
	char* I16 = ".\\IsraeliQueue C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test16\\students.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test16\\courses.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test16\\hackers.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test16\\queues.txt C:\\Users\\refae\\Desktop\\mtm\\ex1\\ExampleTest\\Tests\\Tests\\Test16\\out.txt";
	*/


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

    if (enSystem == NULL){

        // error
        return 1;
    }

    SetNameFlag(enSystem, nameFlag);


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



