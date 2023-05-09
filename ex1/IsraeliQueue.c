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
Item IsraeliQueueDequeueByindex(IsraeliQueue q,int index);

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

    printf("\n--------IsraeliQueue print---------\n");


    printf("number of students inside IsraeliQueue is %d \n",q->size);

    printf("these are the students inside IsraeliQueue: \n");

    for (int i = 0; i < q->size; i++){
        
        Student student = (Student)(q->array[i]->data);
        PrintStudent(student);
    }

    printf("size_of_friendship_functions is: %d\n", q->size_of_friendship_functions);
    printf("friendship threshold is %d\n", q->friendship_threshold);
    printf("rivalry threshold is %d\n", q->rivalry_threshold);

    
    printf("\n--------End of IsraeliQUeue Print---------\n");

}


IsraeliQueue IsraeliQueueCreate(FriendshipFunction *friendshipFunctions, ComparisonFunction comparisonFunction, int friendship_th, int rivalry_th){

    IsraeliQueue new_queue = (IsraeliQueue)malloc(sizeof(*new_queue));

    if (new_queue == NULL){

        return NULL;
    }

    //printf("IsraeliQueue malloced!\n");


    int i = 0;

    if (friendshipFunctions != NULL){

        while (friendshipFunctions[i] != NULL){

            i++;
        }

    }

    //printf("number of friendship functions is %d\n", i);
    	
    new_queue->friendship_functions = (FriendshipFunction*)malloc(sizeof(FriendshipFunction) * (i+1));

    if (new_queue->friendship_functions == NULL){

        free(new_queue); 
        return NULL;
    }

    //printf("friendship functions malloced!\n");

    for (int j = 0; j<i; j++){

        new_queue-> friendship_functions[j] = friendshipFunctions[j];
        //printf("function number %d has assigned to the new queue!\n", j+1);
    }

    new_queue->friendship_functions[i] = NULL;
    new_queue->size_of_friendship_functions = i;

    //printf("struct -> size_of_friendship_functions is %d\n", new_queue->size_of_friendship_functions);

    if (comparisonFunction == NULL){

        free(new_queue->friendship_functions);
        free(new_queue);
        return NULL;
    }

    new_queue->comparison_function = comparisonFunction;
    //printf("struct -> comparison_function assigned!\n", new_queue->comparison_function);

    new_queue->friendship_threshold = friendship_th;
    //printf("struct -> friendship_threshold assigned and it's %d!\n", new_queue->friendship_threshold);
    new_queue->rivalry_threshold = rivalry_th;
    //printf("struct -> rivalry_threshold assigned and it's %d!\n", new_queue->rivalry_threshold);

    new_queue->array = (Item*)malloc(sizeof(Item) * 10);

    if (new_queue->array == NULL){

        free(new_queue->friendship_functions);
        free(new_queue);
        return NULL;
    }

    //printf("struct -> array malloced!\n");

    new_queue->size_of_allocation = 10;
    //printf("struct -> size_of_allocation assigned and it's %d!\n", new_queue->size_of_allocation);

    new_queue->expend_base = 2;
    //printf("struct -> expend_base assigned and it's %d!\n", new_queue->expend_base);

    new_queue->size = 0;
    //printf("struct -> size assigned and it's %d!\n", new_queue->size);


    //printf("IsraeliQueue created!\n");
    return new_queue;
}

/**Returns a new queue with the same elements as the parameter. If the parameter is NULL,
 * NULL is returned.*/
IsraeliQueue IsraeliQueueClone(IsraeliQueue q){

    if (q == NULL){

        //printf("ERROR: IsraeliQueueClone got NULL as a parameter!\n");
        return NULL;
    }

    IsraeliQueue cloned_queue = IsraeliQueueCreate(q->friendship_functions, q->comparison_function, q->friendship_threshold, q->rivalry_threshold);

    if (cloned_queue == NULL){

        return NULL;
    }

    //printf("cloned_queue created!\n");

    // adding to the new queue all the items from the old queue in the same order
    // with all the same properties

    //printf("cloning the queue...\n");
    //printf("size of the queue is %d\n", q->size);

    for (int i = 0; i < q->size; i++){

        IsraeliQueueError error = ReallocateMoreSpace(cloned_queue); // reallocating more space *if needed*

        if (error != ISRAELIQUEUE_SUCCESS){

            //printf("ERROR: ReallocateMoreSpace in clone failed!\n");
            IsraeliQueueDestroy(cloned_queue);
            return NULL;
        }

        Item item = ItemCreate(q->array[i]->data); // creating a new item with the same data as the old item

        if (item == NULL){

            IsraeliQueueDestroy(cloned_queue);
            return NULL;
        }

        //printf("item number %d cloned!\n", i+1);

        cloned_queue->array[i] = item;
        //printf("item number %d assigned to the cloned queue!\n", i+1);
        cloned_queue->size++;
        //printf("size of the cloned queue is %d now \n", cloned_queue->size);

        // copying all the properties of the old item to the new item
        cloned_queue->array[i]->passedFriends = q->array[i]->passedFriends;
        cloned_queue->array[i]->blockedRivals = q->array[i]->blockedRivals;
        cloned_queue->array[i]->improvedPosition = q->array[i]->improvedPosition;
        cloned_queue->array[i]->merged = q->array[i]->merged;

        //printf("item number %d properties copied!\n", i+1);

        // not freeing the item because it's being used in the new queue
        // when the new queue will be destroyed, the items will be destroyed as well

    }

    //printf("queue cloned!\n");
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
    //printf("friendship_functions freed!\n");

    for (int i = 0; i<q->size; i++){

        ItemDestroy(q->array[i]);
        //printf("item number %d destroyed!\n", i+1);
    }

    free(q->array);
    //printf("array freed!\n");

    free(q);
    //printf("queue destroyed!\n");

}


// maybe should be static due to not defined in header file
// returns false if item1 is friend of item2 and true otherwise (enemies)
int IfRival(IsraeliQueue q, Item item1, Item item2){

	if (item1 == NULL || item2 == NULL || q == NULL){

        //printf("ERROR: IfRival got NULL as a parameter!\n");
		return 0;
	}
	
	if (q->size_of_friendship_functions == 0) {return 0;}

    int rivalitySum = 0;

    for (int i = 0; i < (q->size_of_friendship_functions); i++){

        //printf("checking friendship by function number %d\n", i+1);

        if ((q->friendship_functions[i](item1->data, item2->data)) > (q->friendship_threshold)){

            //printf("by function number %d the items (%d,%d) are friends because the function return %d and the friendship treshold is %d \n", i+1, *(int*)(item1->data), *(int*)(item2->data), q->friendship_functions[i](item1->data, item2->data), q->friendship_threshold);
            return 1; // 1 for friend
			
        }else{
			
			rivalitySum += q->friendship_functions[i](item1->data, item2->data);
            //printf("by function number %d the items (%d,%d) are NOT FRIENDS! because the function return %d and the friendship treshold is %d \n", i+1, *(int*)(item1->data), *(int*)(item2->data), q->friendship_functions[i](item1->data, item2->data), q->friendship_threshold);
		    //printf("rivalitySum is %d\n", rivalitySum);
        }

    }

    // if we got here, the items are not freidns by all friendship functions

    //printf("the items are not friends by all friendship functions!\n");

    rivalitySum = (rivalitySum / q->size_of_friendship_functions);

    //printf("rivalitySum is %d\n", rivalitySum);

    if (rivalitySum < q->rivalry_threshold){

        //printf("the items ARE ENEMIES by the rivalry threshold!\n");
        return -1; // negative mean enemy
    }

    //printf("the items are natural!\n");
    return 0; // not friends and not enemies: nutrals

}



/**@param IsraeliQueue: an IsraeliQueue in which to insert the item.
 * @param item: an item to enqueue
 *
 * Places the item in the foremost position accessible to it.*/
IsraeliQueueError IsraeliQueueEnqueue(IsraeliQueue q, void *item){


    if (q == NULL){
        //printf("ERROR: IsraeliQueueEnqueue got NULL as a parameter!\n");
        return ISRAELIQUEUE_BAD_PARAM;
    }


    IsraeliQueueError error = ReallocateMoreSpace(q);

    if (error != ISRAELIQUEUE_SUCCESS){

        //printf("ERROR: ReallocateMoreSpace in enqueue failed!\n");
        return ISRAELIQUEUE_ALLOC_FAILED;
    }
    
    Item new_item = ItemCreate(item);

    if (new_item == NULL){

        //printf("ERROR: ItemCreate in enqueue failed!\n");
        return ISRAELIQUEUE_ALLOC_FAILED;
    }

    // let's divided into 2 cases: 

    // case 1: there is no friendship functions

    if (q->size_of_friendship_functions == 0){ // enqueue like regular queue without trying to improve position

        q->array[q->size] = new_item;
        q->size++;
        //printf("item number %d enqueued without friendships functions!\n", q->size);
        return ISRAELIQUEUE_SUCCESS;
    }
	
    // if we got here, there are friendship functions (because there is return in the previous if statement)
    // case 2: there are friendship functions
	
    if (q->size == 0){


        q->array[0] = new_item;
        q->size++;
        //printf("item number 1 enqueued!\n");
        return ISRAELIQUEUE_SUCCESS;
    }

	q->array[q->size] = new_item;
	//printf("q->array[%d] = new_item;\n", q->size);


    int swapIndex = q->size;

    //printf("checking if the item is friend of anyone...\n");
	
    for (int j = 0; j < q->size; j++){

        //printf("checking if the item is friend of item[%d]\n", j);

        int FurtherFriend = IfExsistFurtherFriend(q, q->size, j);
        //printf("index of FurtherFriend is %d\n", FurtherFriend);

        if (FurtherFriend == -1){ // no friends have been found

            swapIndex = q->size;
            //printf("no friends have been found from item[%d] to item[%d]!\n",q->size, j);
            break;

        }else{ // friend found

            //printf("item[%d] is a friend of the enqueued item!, checking if it is blocked by enemies...\n", FurtherFriend);
            int EnemyBlock = IfEnemyBlocked(q, FurtherFriend, q->size);

            if (EnemyBlock == -1){ // no enemies have been found

                swapIndex = FurtherFriend;
                //printf("not blocked by enemies, item[%d] is the new swapIndex!\n", FurtherFriend+1);
                break;

            }else{// enemy found

                q->array[EnemyBlock]->blockedRivals++;
                printf("item is blocked by enemies, continue searching for a friend in smaller distance!\n");
                continue; // continue searching for other friend
            }
        }

    }

    if (swapIndex == q->size){ // if we get in, the item is not friend of anyone or friend of the last item

        q->array[swapIndex] = new_item;
        q->size++;
        //printf("item enqueue to the end of the queue, has no friends that can help!\n");
        return ISRAELIQUEUE_SUCCESS;

    } else { // if we get in, the item is friend of someone 

        //printf("item enqueue in the end of the queue, has friends that can help!\n");
        for (int i = q->size ; i > (swapIndex+1) ; i--){ // shift all items to the right

            //printf("shifting item[%d] to item[%d]\n", i-1, i);
            q->array[i] = q->array[i-1];
        } 

        printf("improve position in enqueue was successful!\n");
        //printf("item[%d] = new_item;\n", swapIndex+1);
        q->array[swapIndex+1] = new_item;
        q->size++;
        q->array[swapIndex]->passedFriends++;

        //printf("size now is %d\n", q->size);

        return ISRAELIQUEUE_SUCCESS;
        
    }
	
	

}


IsraeliQueueError IsraeliQueueImproveItemByIndex(IsraeliQueue q, int index){


    if (q == NULL || q->array[index]->improvedPosition == true){
        //printf("ERROR: IsraeliQueueEnqueue got NULL as a parameter!\n");
        return ISRAELIQUEUE_BAD_PARAM;
    }

    IsraeliQueueError error = ReallocateMoreSpace(q);

    if (error != ISRAELIQUEUE_SUCCESS){

        //printf("ERROR: ReallocateMoreSpace in enqueue failed!\n");
        return ISRAELIQUEUE_ALLOC_FAILED;
    }

    Item new_item = IsraeliQueueDequeueByindex(q,index);

    if (new_item == NULL){

        //printf("ERROR: ItemCreate in enqueue failed!\n");
        return ISRAELIQUEUE_ALLOC_FAILED;
    }

    // let's divided into 2 cases:

    // case 1: there is no friendship functions

    if (q->size_of_friendship_functions == 0){ // enqueue like regular queue without trying to improve position

        q->array[q->size] = new_item;
        q->size++;
        //printf("item number %d enqueued without friendships functions!\n", q->size);
        q->array[q->size]->improvedPosition = true;
        return ISRAELIQUEUE_SUCCESS;
    }

    // if we got here, there are friendship functions (because there is return in the previous if statement)
    // case 2: there are friendship functions

    if (q->size == 0){


        q->array[0] = new_item;
        q->size++;
        q->array[0]->improvedPosition = true;
        //printf("item number 1 enqueued!\n");
        return ISRAELIQUEUE_SUCCESS;
    }

    q->array[q->size] = new_item;
    //printf("q->array[%d] = new_item;\n", q->size);


    int swapIndex = q->size;

    //printf("checking if the item is friend of anyone...\n");

    for (int j = 0; j < q->size; j++){

        //printf("checking if the item is friend of item[%d]\n", j);

        int FurtherFriend = IfExsistFurtherFriend(q, q->size, j);
        //printf("index of FurtherFriend is %d\n", FurtherFriend);

        if (FurtherFriend == -1){ // no friends have been found

            swapIndex = q->size;
            //printf("no friends have been found from item[%d] to item[%d]!\n",q->size, j);
            break;

        }else{ // friend found

            //printf("item[%d] is a friend of the enqueued item!, checking if it is blocked by enemies...\n", FurtherFriend);
            int EnemyBlock = IfEnemyBlocked(q, FurtherFriend, q->size);

            if (EnemyBlock == -1){ // no enemies have been found

                swapIndex = FurtherFriend;
                //printf("not blocked by enemies, item[%d] is the new swapIndex!\n", FurtherFriend+1);
                break;

            }else{// enemy found

                q->array[EnemyBlock]->blockedRivals++;
                printf("item is blocked by enemies, continue searching for a friend in smaller distance!\n");
                continue; // continue searching for other friend
            }
        }

    }

    if (swapIndex == q->size){ // if we get in, the item is not friend of anyone or friend of the last item

        q->array[swapIndex] = new_item;
        q->size++;
        q->array[swapIndex]->improvedPosition = true;
        //printf("item enqueue to the end of the queue, has no friends that can help!\n");
        return ISRAELIQUEUE_SUCCESS;

    } else { // if we get in, the item is friend of someone

        //printf("item enqueue in the end of the queue, has friends that can help!\n");
        for (int i = q->size ; i > (swapIndex+1) ; i--){ // shift all items to the right

            //printf("shifting item[%d] to item[%d]\n", i-1, i);
            q->array[i] = q->array[i-1];
        }

        printf("improve position in enqueue was successful!\n");
        //printf("item[%d] = new_item;\n", swapIndex+1);
        q->array[swapIndex+1] = new_item;
        q->size++;
        q->array[swapIndex+1]->improvedPosition = true;
        q->array[swapIndex]->passedFriends++;

        //printf("size now is %d\n", q->size);

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

    //printf("friendship function added successfully!\n");

    return ISRAELIQUEUE_SUCCESS;

}

/**@param IsraeliQueue: an IsraeliQueue whose friendship threshold is to be modified
 * @param friendship_threshold: a new friendship threshold for the IsraeliQueue*/
IsraeliQueueError IsraeliQueueUpdateFriendshipThreshold(IsraeliQueue q, int newFriendshipThreshold){

        q->friendship_threshold = newFriendshipThreshold;
        //printf("friendship threshold updated successfully!\n");
        return ISRAELIQUEUE_SUCCESS;

}

/**@param IsraeliQueue: an IsraeliQueue whose rivalry threshold is to be modified
 * @param friendship_threshold: a new rivalry threshold for the IsraeliQueue*/
IsraeliQueueError IsraeliQueueUpdateRivalryThreshold(IsraeliQueue q, int newRivalryThreshold){

        q->rivalry_threshold = newRivalryThreshold;
        //printf("rivalry threshold updated successfully!\n");
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

Item IsraeliQueueDequeueByindex(IsraeliQueue q,int index){

    if (q == NULL || q->size == 0){

        return NULL;
    }

    Item temp = q->array[index];

    for (int i = index; i < q->size-1; i++){

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

    //printf("checking if there is enemies between the friend, item[%d], and item[%d]\n", friendIndex, itemIndex);

    for (int i = friendIndex; i < itemIndex; i++){

        //printf("checking if item[%d] is a rival of item[%d]\n", i, itemIndex);

        if (IfRival(q, q->array[itemIndex], q->array[i]) == -1 && q->array[i]->blockedRivals < RIVAL_BLOCK_PASS_LIMIT){

            //printf("item has blocking enemy! THE ENEMY IS[%d]!\n", i);
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

    //printf("searching freind for item[%d] from item[%d]\n", itemIndex, StartingPosition);

    for (int i = StartingPosition; i < itemIndex; i++){

        //printf("searching if friend of item[%d]\n",i);

        if (IfRival(q, q->array[itemIndex], q->array[i]) == 1 && (q->array[i]->passedFriends) < FRIENDS_PASS_LIMIT){

            //printf("item[%d] is a friend of item[%d] that has more passing!\n", itemIndex,i);
            return i;
        }
    }

    //printf("no FurtherFriend has been found!\n");
    return -1; // no friend has been found

}

/**Advances each item in the queue to the foremost position accessible to it,
 * from the back of the queue frontwards.*/
IsraeliQueueError IsraeliQueueImprovePositions(IsraeliQueue q){

    if (q == NULL){

        return ISRAELIQUEUE_BAD_PARAM;
    }


    else if (q->size == 0 || q->size == 1){ return ISRAELIQUEUE_SUCCESS;}


    for (int j = 0; j < q->size; j++){

        int p = 0;

        for (p = (q->size - 1); p>=0; p--){

            if (q->array[p]->improvedPosition == false){

                break;

            }
        }

        int ImproveCurrentItemIndex = p;

        if (IsraeliQueueImproveItemByIndex(q,ImproveCurrentItemIndex) != ISRAELIQUEUE_SUCCESS){

            return ISRAELI_QUEUE_ERROR;

        }

    }

    for (int i = 0; i < q->size; i++){

        q->array[i]->improvedPosition = false;
    }

    return ISRAELIQUEUE_SUCCESS;
}


/**@param q_arr: a NULL-terminated array of IsraeliQueues
 * @param ComparisonFunction: a comparison function for the merged queue
 *
 * Merges all queues in q_arr into a single new queue, with parameters the parameters described
 * in the exercise. Each queue in q_arr enqueues its head in the merged queue, then lets the next
 * one enqueue an item, in the order defined by q_arr. In the event of any error during execution, return NULL.*/
IsraeliQueue IsraeliQueueMerge(IsraeliQueue *qs ,ComparisonFunction f){

    //(FriendshipFunction *friendshipFunctions, ComparisonFunction comparisonFunction, int friendship_th, int rivalry_th){
    
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

        PrintIsraeliQueue(qs[i]);
        new_friendship_th += qs[i]->friendship_threshold;
        new_rivalry_th *= qs[i]->rivalry_threshold;
        i++;
    }

    double rootOrder = (1.0 / ((double)i));

    new_friendship_th = (int)((new_friendship_th + i - 1) / i); // ceiling to the nearest integer
    new_rivalry_th = (int)ceil(pow(new_rivalry_th, rootOrder)); // ceiling to the nearest integer

    // creating new queue

    //ptintf("creating new_queue\n");
    IsraeliQueue new_queue = IsraeliQueueCreate(friendshipFunctions,f,new_friendship_th,new_rivalry_th);

    free(friendshipFunctions); // friendshipFunctions is no longer needed because it has been copied to new_queue

    if (new_queue == NULL){ // if new_queue creation failed

        return NULL;
    }

    PrintIsraeliQueue(new_queue);

    // transfering items from qs to new_queue

    while (!ifAllItemsMerged(qs)){ // while there is at least one item in one of the queues that has not been merged

        i = 0;

        while (qs[i] != NULL){

            //printf("queue number[%d] is not null\n", i);

            int j = 0;

            for (j = 0; j<qs[i]->size; j++){

                if (qs[i]->array[j]->merged == false){ // if item has been merged, continue to next item

                    break;
                }

            }

            //printf("item[%d] in queue[%d] is the next to merge\n", j, i);

            //printf("line 748\n");

            if (j == qs[i]->size){// all items in qs[i] have been merged, continue to next queue

                //printf("oh oh - j=%d == qs[%d]->size == %d \n", j,i, qs[i]->size);
                i++;
                continue;
            }

            //printf("line 757\n");

            qs[i]->array[j]->merged = true;
            //printf("item[%d] in queue[%d] has been marked as merged\n", j, i);

            //printf("the data is %d\n", *(int*)qs[i]->array[j]->data);

            IsraeliQueueError error = IsraeliQueueEnqueue(new_queue,qs[i]->array[j]->data);
            //printf("enqueu to the new queue has been done\n");

            if (error == ISRAELIQUEUE_ALLOC_FAILED || error ==  ISRAELIQUEUE_BAD_PARAM){// if enqueue failed

                //printf("Enqueue failed in IsraeliQueueMerge!\n");
                return NULL;
            }

            i++;
        }
        
    }

    //printf("GOT HERE!!!\n");

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
