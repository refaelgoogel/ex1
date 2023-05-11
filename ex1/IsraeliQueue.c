#include "Item.h"
#include <math.h>
#include "IsraeliQueue.h"
#include "Student.h"


#define FRIENDS_PASS_LIMIT 5
#define RIVAL_BLOCK_PASS_LIMIT 3


int IfRival(IsraeliQueue q, Item item1, Item item2); // maybe should be removed
int IfExsistFurtherFriend(IsraeliQueue q, int itemIndex, int StartingPosition);
int IfEnemyBlocked(IsraeliQueue q, int friendIndex, int itemIndex);
bool ifAllItemsMerged(IsraeliQueue *qs);
void MarkAsUnmerged(IsraeliQueue *qs);
IsraeliQueueError ReallocateMoreSpace(IsraeliQueue q);
int FriendshipFunctionNumberInQueueArray(IsraeliQueue *qs);
FriendshipFunction* FriendshipFunctionFiller(IsraeliQueue *qs);
int FindIndex(IsraeliQueue q, Item item);
IsraeliQueue IsraeliQueueDequeueByIndex(IsraeliQueue q,int index);

struct IsraeliQueue_t{

    FriendshipFunction *friendship_functions;
    int size_of_friendship_functions;
    ComparisonFunction comparison_function; 
    int friendship_threshold;
    int rivalry_threshold;
    Item *array; 
    int size_of_allocation ;
    int expend_base;
    int size;

};
    
/**Creates a new IsraeliQueue_t object with the provided friendship functions, a NULL-terminated array,
 * comparison function, friendship threshold and rivalry threshold. Returns a pointer
 * to the new object. In case of failure, return NULL.*/

void PrintIsraeliQueue(IsraeliQueue q){

    printf("\n\n--------IsraeliQueue:--------\n\n");


    printf("students number: %d",q->size);

    printf("\nstudents in queue: \n");

    for (int i = 0; i < q->size; i++){
        
        printf("student number %d.", i+1);
        Student student = (Student)(q->array[i]->data);
        PrintStudent(student);
    }

    printf("%d friendship_functions\n", q->size_of_friendship_functions);
    printf("friendship threshold is %d and rivalry threshold is %d \n", q->friendship_threshold, q->rivalry_threshold);
    
}

IsraeliQueue IsraeliQueueCreate(FriendshipFunction *friendshipFunctions, ComparisonFunction comparisonFunction, int friendship_th, int rivalry_th){

    IsraeliQueue new_queue = (IsraeliQueue)malloc(sizeof(*new_queue));

    if (new_queue == NULL){

        return NULL;
    }


    int i = 0;

    if (friendshipFunctions != NULL){

        while (friendshipFunctions[i] != NULL){

            i++;
        }

    }

    	
    new_queue->friendship_functions = (FriendshipFunction*)malloc(sizeof(FriendshipFunction) * (i+1));

    if (new_queue->friendship_functions == NULL){

        free(new_queue); 
        return NULL;
    }


    for (int j = 0; j<i; j++){

        new_queue-> friendship_functions[j] = friendshipFunctions[j];
    }

    new_queue->friendship_functions[i] = NULL;
    new_queue->size_of_friendship_functions = i;


    if (comparisonFunction == NULL){

        free(new_queue->friendship_functions);
        free(new_queue);
        return NULL;
    }

    new_queue->comparison_function = comparisonFunction;

    new_queue->friendship_threshold = friendship_th;
    new_queue->rivalry_threshold = rivalry_th;

    new_queue->array = (Item*)malloc(sizeof(Item) * 10);

    if (new_queue->array == NULL){

        free(new_queue->friendship_functions);
        free(new_queue);
        return NULL;
    }


    new_queue->size_of_allocation = 10;

    new_queue->expend_base = 2;

    new_queue->size = 0;


    return new_queue;
}

/**Returns a new queue with the same elements as the parameter. If the parameter is NULL,
 * NULL is returned.*/
IsraeliQueue IsraeliQueueClone(IsraeliQueue q){

    if (q == NULL){

        return NULL;
    }

    IsraeliQueue cloned_queue = IsraeliQueueCreate(q->friendship_functions, q->comparison_function, q->friendship_threshold, q->rivalry_threshold);

    if (cloned_queue == NULL){

        return NULL;
    }


    // adding to the new queue all the items from the old queue in the same order
    // with all the same properties


    for (int i = 0; i < q->size; i++){

        IsraeliQueueError error = ReallocateMoreSpace(cloned_queue); // reallocating more space *if needed*

        if (error != ISRAELIQUEUE_SUCCESS){

            IsraeliQueueDestroy(cloned_queue);
            return NULL;
        }

        Item item = ItemCreate(q->array[i]->data); // creating a new item with the same data as the old item

        if (item == NULL){

            IsraeliQueueDestroy(cloned_queue);
            return NULL;
        }


        cloned_queue->array[i] = item;
        cloned_queue->size++;

        // copying all the properties of the old item to the new item
        cloned_queue->array[i]->passedFriends = q->array[i]->passedFriends;
        cloned_queue->array[i]->blockedRivals = q->array[i]->blockedRivals;
        cloned_queue->array[i]->improvedPosition = q->array[i]->improvedPosition;
        cloned_queue->array[i]->merged = q->array[i]->merged;


        // not freeing the item because it's being used in the new queue
        // when the new queue will be destroyed, the items will be destroyed as well

    }

    return cloned_queue;    
}

/**@param IsraeliQueue: an IsraeliQueue created by IsraeliQueueCreate
 *
 * Deallocates all memory allocated by IsraeliQueueCreate for the object pointed to by
 * the parameter.*/
void IsraeliQueueDestroy(IsraeliQueue q){

    if (q == NULL){
        return;
    }

    free(q->friendship_functions);

    for (int i = 0; i<q->size; i++){

        ItemDestroy(q->array[i]);
    }

    free(q->array);

    free(q);

}


// maybe should be static due to not defined in header file
// returns false if item1 is friend of item2 and true otherwise (enemies)
int IfRival(IsraeliQueue q, Item item1, Item item2){

	if (item1 == NULL || item2 == NULL || q == NULL){

		return 0;
	}
	
	if (q->size_of_friendship_functions == 0) {return 0;}

    int rivalitySum = 0;

    for (int i = 0; i < (q->size_of_friendship_functions); i++){


        if ((q->friendship_functions[i](item1->data, item2->data)) > (q->friendship_threshold)){

            return 1; // 1 for friend
			
        }else{
			
			rivalitySum += q->friendship_functions[i](item1->data, item2->data);
        }

    }

    // if we got here, the items are not freidns by all friendship functions


    rivalitySum = (rivalitySum / q->size_of_friendship_functions);


    if (rivalitySum < q->rivalry_threshold){

        return -1; // negative mean enemy
    }

    return 0; // not friends and not enemies: nutrals

}



/**@param IsraeliQueue: an IsraeliQueue in which to insert the item.
 * @param item: an item to enqueue
 *
 * Places the item in the foremost position accessible to it.*/
IsraeliQueueError IsraeliQueueEnqueue(IsraeliQueue q, void *item){


    if (q == NULL){
        return ISRAELIQUEUE_BAD_PARAM;
    }

    IsraeliQueueError error = ReallocateMoreSpace(q);

    if (error != ISRAELIQUEUE_SUCCESS){

        return ISRAELIQUEUE_ALLOC_FAILED;
    }
    
    Item new_item = ItemCreate(item);

    if (new_item == NULL){

        return ISRAELIQUEUE_ALLOC_FAILED;
    }

    // let's divided into 2 cases: 

    // case 1: there is no friendship functions

    if (q->size_of_friendship_functions == 0){ // enqueue like regular queue without trying to improve position

        q->array[q->size] = new_item;
        q->size++;
        return ISRAELIQUEUE_SUCCESS;
    }
	
    // if we got here, there are friendship functions (because there is return in the previous if statement)
    // case 2: there are friendship functions
	
    if (q->size == 0){


        q->array[0] = new_item;
        q->size++;
        return ISRAELIQUEUE_SUCCESS;
    }

	q->array[q->size] = new_item;

    int swapIndex = q->size;

	
    for (int j = 0; j < q->size; j++){


        int FurtherFriend = IfExsistFurtherFriend(q, q->size, j);

        if (FurtherFriend == -1){ // no friends have been found

            swapIndex = q->size;
            break;

        }else{ // friend found

            int EnemyBlock = IfEnemyBlocked(q, FurtherFriend, q->size);

            if (EnemyBlock == -1){ // no enemies have been found

                swapIndex = FurtherFriend;
                break;

            }else{// enemy found

                q->array[EnemyBlock]->blockedRivals++;
                j = EnemyBlock;
                continue; // continue searching for other friend
            }
        }

    }

    if (swapIndex == q->size){ // if we get in, the item is not friend of anyone or friend of the last item

        q->array[swapIndex] = new_item;
        q->size++;

        return ISRAELIQUEUE_SUCCESS;

    } else { // if we get in, the item is friend of someone 

        for (int i = q->size ; i > (swapIndex+1) ; i--){ // shift all items to the right

            q->array[i] = q->array[i-1];
        } 

        q->array[swapIndex+1] = new_item;
        q->size++;
        q->array[swapIndex]->passedFriends++;


        return ISRAELIQUEUE_SUCCESS;
        
    }

}


/**@param IsraeliQueue: an IsraeliQueue to which the function is to be added
 * @param FriendshipFunction: a FriendshipFunction to be recognized by the IsraeliQueue
 * going forward.
 *
 * Makes the IsraeliQueue provided recognize the FriendshipFunction provided.*/
IsraeliQueueError IsraeliQueueAddFriendshipMeasure(IsraeliQueue q, FriendshipFunction f){

    FriendshipFunction* temp = (FriendshipFunction*)realloc(q->friendship_functions, sizeof(FriendshipFunction) * (q->size_of_friendship_functions + 2));

    if (temp == NULL){

        return ISRAELIQUEUE_ALLOC_FAILED;
    }

    q->friendship_functions = temp;
    q->friendship_functions[q->size_of_friendship_functions] = f;
    q->friendship_functions[q->size_of_friendship_functions+1] = NULL;
    q->size_of_friendship_functions++;


    return ISRAELIQUEUE_SUCCESS;

}

/**@param IsraeliQueue: an IsraeliQueue whose friendship threshold is to be modified
 * @param friendship_threshold: a new friendship threshold for the IsraeliQueue*/
IsraeliQueueError IsraeliQueueUpdateFriendshipThreshold(IsraeliQueue q, int newFriendshipThreshold){

        q->friendship_threshold = newFriendshipThreshold;
        return ISRAELIQUEUE_SUCCESS;

}

/**@param IsraeliQueue: an IsraeliQueue whose rivalry threshold is to be modified
 * @param friendship_threshold: a new rivalry threshold for the IsraeliQueue*/
IsraeliQueueError IsraeliQueueUpdateRivalryThreshold(IsraeliQueue q, int newRivalryThreshold){

        q->rivalry_threshold = newRivalryThreshold;
        return ISRAELIQUEUE_SUCCESS;

}

/**Returns the number of elements of the given queue. If the parameter is NULL, 0
 * is returned.*/
int IsraeliQueueSize(IsraeliQueue q){

    if (q == NULL){

        return 0;
    }

    return q->size;

}

/**Removes and returns the foremost element of the provided queue. If the parameter
 * is NULL or a pointer to an empty queue, NULL is returned.*/
void* IsraeliQueueDequeue(IsraeliQueue q){

    if (q == NULL || q->size == 0){

        return NULL;
    }

    void* temp = q->array[0]->data;

    for (int i = 0; i < q->size-1; i++){

        q->array[i] = q->array[i+1];

    }

    q->array[q->size-1] = NULL;
    q->size--;
    
    return temp;

}


/**@param item: an object comparable to the objects in the IsraeliQueue
 *
 * Returns whether the queue contains an element equal to item. If either
 * parameter is NULL, false is returned.*/
bool IsraeliQueueContains(IsraeliQueue q, void* comparableItem){

    if (q == NULL || comparableItem == NULL || q->size == 0){

        return false;
    }

    for (int i = 0; i < q->size; i++){

        if (q->comparison_function(q->array[i]->data, comparableItem) == 1){ // if the item is found

            return true;
        }
    }

    return false;

}

// this function return -1 if no rival with blocking ability has been found between the two items,
// and the index of the rival with blocking ability, if found
// assumption: friendIndex < itemIndex (the friend is closer to the head of the queue)
int IfEnemyBlocked(IsraeliQueue q, int friendIndex, int itemIndex){

    if (itemIndex > q->size){

        return -1;
    }


    for (int i = friendIndex; i < itemIndex; i++){


        if (IfRival(q, q->array[itemIndex], q->array[i]) == -1 && q->array[i]->blockedRivals < RIVAL_BLOCK_PASS_LIMIT){

            return i;
        }
    }
    
    return -1; // no rival has been found

}


// this function search for friends with passing ability between the startingPosition and the item. 
// return -1 if no friend with passing ability has been found, andindex of the further friend with passing ability, if found
// assumptions:
// itemIndex > friendIndex 
// itemIndex > StartingPosition
// itemIndex < q->size
int IfExsistFurtherFriend(IsraeliQueue q , int itemIndex, int StartingPosition){

    if (StartingPosition > itemIndex || itemIndex > q->size){

        return -1;
    }


    for (int i = StartingPosition; i < itemIndex; i++){


        if (IfRival(q, q->array[itemIndex], q->array[i]) == 1 && (q->array[i]->passedFriends) < FRIENDS_PASS_LIMIT){

            return i;
        }
    }

    return -1; // no friend has been found

}

/**Advances each item in the queue to the foremost position accessible to it,
 * from the back of the queue frontwards.*/

IsraeliQueue IsraeliQueueDequeueByIndex(IsraeliQueue q,int index){

    if (q == NULL || q->size == 0){

        return NULL;
    }

    for (int i = index; i < q->size-1; i++){

        q->array[i] = q->array[i+1];

    }

    q->array[q->size-1] = NULL;
    q->size--;

    return q;
}


int FindIndex(IsraeliQueue q, Item item){


    for (int i = 0; i < q->size; i++){


        if (q->array[i]->data == item->data){

            return i;

        }

    }

    return -1;

}

IsraeliQueueError IsraeliQueueImprovePositions(IsraeliQueue q){

    int indexCurrentItem = 0;

    IsraeliQueue qCopy = IsraeliQueueClone(q);

    for (int i = ((q->size)-1); i >= 0; i--){

        indexCurrentItem = FindIndex(q, qCopy->array[i]);
        q = IsraeliQueueDequeueByIndex(q, indexCurrentItem);

        IsraeliQueueError error = IsraeliQueueEnqueue(q, qCopy->array[i]->data);
        indexCurrentItem = FindIndex(q, qCopy->array[i]);

        q->array[indexCurrentItem]->passedFriends = qCopy->array[i]->passedFriends;
        q->array[indexCurrentItem]->blockedRivals = qCopy->array[i]->blockedRivals;

        for (int j = 0; j<q->size; j++){

            int index = FindIndex(q, qCopy->array[j]);
            qCopy->array[j]->passedFriends = q->array[index]->passedFriends;
            qCopy->array[j]->blockedRivals = q->array[index]->blockedRivals;

        }

        if (error != ISRAELIQUEUE_SUCCESS){
            return ISRAELI_QUEUE_ERROR;
        }
    }

    IsraeliQueueDestroy(qCopy);
    return ISRAELIQUEUE_SUCCESS;

}


/**@param q_arr: a NULL-terminated array of IsraeliQueues
 * @param ComparisonFunction: a comparison function for the merged queue
 *
 * Merges all queues in q_arr into a single new queue, with parameters the parameters described
 * in the exercise. Each queue in q_arr enqueues its head in the merged queue, then lets the next
 * one enqueue an item, in the order defined by q_arr. In the event of any error during execution, return NULL.*/
IsraeliQueue IsraeliQueueMerge(IsraeliQueue *qs ,ComparisonFunction f){

    
    if (qs == NULL || f == NULL){
        
        return NULL;
    }

    FriendshipFunction *friendshipFunctions = FriendshipFunctionFiller(qs);

    if (friendshipFunctions == NULL){

        return NULL;
    }

    // computing friendship treshold and rivalry treshold

    int new_friendship_th = 0;
    int new_rivalry_th = 1;

    int i = 0;

    while (qs[i] != NULL){

        new_friendship_th += qs[i]->friendship_threshold;
        new_rivalry_th *= qs[i]->rivalry_threshold;
        i++;
    }

    double rootOrder = (1.0 / ((double)i));

    new_friendship_th = (int)((new_friendship_th + i - 1) / i); // ceiling to the nearest integer
    new_rivalry_th = (int)ceil(pow(new_rivalry_th, rootOrder)); // ceiling to the nearest integer

    // creating new queue

    IsraeliQueue new_queue = IsraeliQueueCreate(friendshipFunctions,f,new_friendship_th,new_rivalry_th);

    free(friendshipFunctions); // friendshipFunctions is no longer needed because it has been copied to new_queue

    if (new_queue == NULL){ // if new_queue creation failed

        return NULL;
    }


    // transfering items from qs to new_queue

    while (!ifAllItemsMerged(qs)){ // while there is at least one item in one of the queues that has not been merged

        i = 0;

        while (qs[i] != NULL){


            int j = 0;

            for (j = 0; j<qs[i]->size; j++){

                if (qs[i]->array[j]->merged == false){ // if item has been merged, continue to next item

                    break;
                }

            }



            if (j == qs[i]->size){// all items in qs[i] have been merged, continue to next queue

                i++;
                continue;
            }


            qs[i]->array[j]->merged = true;


            IsraeliQueueError error = IsraeliQueueEnqueue(new_queue,qs[i]->array[j]->data);

            if (error == ISRAELIQUEUE_ALLOC_FAILED || error ==  ISRAELIQUEUE_BAD_PARAM){// if enqueue failed

                return NULL;
            }

            i++;
        }
        
    }


    MarkAsUnmerged(qs); // unmarking all items as merged

    // returning new_queue

    return new_queue;

}


int FriendshipFunctionNumberInQueueArray(IsraeliQueue *qs){

    int i = 0;
    int sum_of_functions = 0;

    while (qs[i] != NULL){

        sum_of_functions += qs[i]->size_of_friendship_functions;
        i++;
    }

    return sum_of_functions;
}

FriendshipFunction* FriendshipFunctionFiller(IsraeliQueue *qs){

    int sum_of_functions = FriendshipFunctionNumberInQueueArray(qs);

    FriendshipFunction *friendshipFunctions = (FriendshipFunction*)malloc(sizeof(FriendshipFunction) * (sum_of_functions+1));

    if (friendshipFunctions == NULL){

        return NULL;
    }

    int i = 0;
    int k = 0;

    while (qs[i] != NULL){


        for (int j = 0; j < qs[i]->size_of_friendship_functions; j++){

            friendshipFunctions[k] = qs[i]->friendship_functions[j];
            k++;
        }

        i++;
    }

    friendshipFunctions[k] = NULL;

    return friendshipFunctions;
}



bool ifAllItemsMerged(IsraeliQueue *qs){

    int i = 0;

    while (qs[i] != NULL){

        int j = 0;

        while (j < qs[i]->size){

            if (qs[i]->array[j]->merged == false){

                return false;
            }

            j++;
        }

        i++;
    }

    return true;
}


void MarkAsUnmerged(IsraeliQueue *qs){

    int i = 0;

    while (qs[i] != NULL){

        int j = 0;

        while (j < qs[i]->size){

            qs[i]->array[j]->merged = false;
            j++;
        }

        i++;
    }

}

IsraeliQueueError ReallocateMoreSpace(IsraeliQueue q){

    if (q->size == q->size_of_allocation){

        Item *temp = (Item*)realloc(q->array, sizeof(Item) * (q->size_of_allocation * q->expend_base));

        if (temp == NULL){

            return ISRAELIQUEUE_ALLOC_FAILED;
        }

        q->array = temp;

        q->size_of_allocation = q->size_of_allocation * q->expend_base;
    }

    return ISRAELIQUEUE_SUCCESS;
}
