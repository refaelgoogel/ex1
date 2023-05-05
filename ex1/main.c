#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "Item.h"
#include "IsraeliQueue.h"

int f1(void *a, void *b);
int f2(void *a, void *b);
int f3(void *a, void *b);
int f4(void *a, void *b);
int comparision(void *a, void *b);


int main(int argc, char *argv[]){

    FriendshipFunction *FunctionArr = malloc(sizeof(FunctionArr)*5);

    if (FunctionArr == NULL){

        return -1;
    }

    FunctionArr[0] = &f1;
    FunctionArr[1] = &f2;
    FunctionArr[2] = &f3;
    FunctionArr[3] = &f4;
	FunctionArr[4] = NULL;

    ComparisonFunction comp = comparision;

    IsraeliQueue q = IsraeliQueueCreate(FunctionArr, comp, 5, 3);

    if (q == NULL){

        return -1;
    }
	
	int a1 = 0;
	int a2 = 1;
	int a3 = 2;
	int a4 = 3;
	int a5 = 4;
	int a6 = 5;
	int a7 = 6;
	int a8 = 7;
	int a9 = 8;
	int a10 = 9;
	
	
    IsraeliQueueEnqueue(q, &a1);
    IsraeliQueueEnqueue(q, &a2);
    IsraeliQueueEnqueue(q, &a3);
    IsraeliQueueEnqueue(q, &a4);
    IsraeliQueueEnqueue(q, &a5);
    IsraeliQueueEnqueue(q, &a6);
    IsraeliQueueEnqueue(q, &a7);
    IsraeliQueueEnqueue(q, &a8);
    IsraeliQueueEnqueue(q, &a9);
    IsraeliQueueEnqueue(q, &a10);


    for (int i = 0; i < 10; i++){

        printf("%d\n", (*(int*)IsraeliQueueDequeue(q)));
    }
	
	IsraeliQueueEnqueue(q, &a1);
    IsraeliQueueEnqueue(q, &a2);
    IsraeliQueueEnqueue(q, &a3);
    IsraeliQueueEnqueue(q, &a4);
    IsraeliQueueEnqueue(q, &a5);
    IsraeliQueueEnqueue(q, &a6);
    IsraeliQueueEnqueue(q, &a7);
    IsraeliQueueEnqueue(q, &a8);
    IsraeliQueueEnqueue(q, &a9);
    IsraeliQueueEnqueue(q, &a10);
	
	IsraeliQueueImprovePositions(q);

	for (int i = 0; i < 10; i++){

        printf("%d\n", (*(int*)IsraeliQueueDequeue(q)));
    }

    IsraeliQueueDestroy(q);
    free(FunctionArr);
    return 0;
}


int f1(void *a, void *b){// a and b are int pointers

	printf("f1\n");
	
	
	if (a == NULL || b == NULL){

        return -1;

    }else{

        int A = *(int*)a;
        int B = *(int*)b;
		
		if (B == 0){return 0;}
		
		return (A+B);
		
	}


}

int f2(void *a, void *b){// a and b are int pointers

	printf("f2\n");
	
    
	if (a == NULL || b == NULL){

        return -1;

    }else{

        int A = *(int*)a;
        int B = *(int*)b;
		
		if (B == 0){return 0;}
		
		return (A-B);
		
	}


}

int f3(void *a, void *b){// a and b are int pointers

	printf("f3\n");
	
	
	
	if (a == NULL || b == NULL){

        return -1;

    }else{

        int A = *(int*)a;
        int B = *(int*)b;
		
		if (B == 0){return 0;}
		
		return (A*B);
		
	}


}

int f4(void *a, void *b){// a and b are int pointers

	printf("f4\n");

	if (a == NULL || b == NULL){

        return -1;

    }else{

        int A = *(int*)a;
        int B = *(int*)b;
		
		if (B == 0){return 0;}
		
		return (A/B);
		
	}

}

int comparision(void *a, void *b){

    if (a == NULL || b == NULL){

        return -1;

    }else{

        int A = *(int*)a;
        int B = *(int*)b;

        if (A > B){

            return A;

        }else{
            
            return B;
        }

    }

}