#include <math.h>
#include "IsraeliQueue.h"
#include "Item.h"

#define FRIENDS_PASS_LIMIT 5
#define RIVAL_BLOCK_PASS_LIMIT 3


int IfRival(IsraeliQueue q, Item item1, Item item2); // maybe should be removed
int IfExsistFurtherFriend(IsraeliQueue q, int itemIndex, int StartingPosition);
int IfEnemyBlocked(IsraeliQueue q, int friendIndex, int itemIndex);
bool ifAllItemsMerged(IsraeliQueue *qs);
void MarkAsUnmerged(IsraeliQueue *qs);
IsraeliQueueError ReallocateMoreSpace(IsraeliQueue q);


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
IsraeliQueue IsraeliQueueCreate(FriendshipFunction *friendshipFunctions, ComparisonFunction comparisonFunction, int friendship_th, int rivalry_th){

    IsraeliQueue new_queue = malloc(sizeof(*new_queue));

    if (new_queue == NULL){

        return NULL;
    }

    int i = 0;

    while (friendshipFunctions[i] != NULL){

        i++;
    }
    
	printf("sarting!!!!! size of Friendship Function is %d\n",i);
	
    new_queue->friendship_functions = malloc(sizeof(FriendshipFunction) * (i+1));

    if (new_queue->friendship_functions == NULL){

        free(new_queue); 
        return NULL;
    }

    for (int j = 0; j<i; j++){

        new_queue-> friendship_functions[j] = friendshipFunctions[j];
    }

    new_queue->friendship_functions[i] = NULL;
    new_queue->size_of_friendship_functions = i;
    
    new_queue->comparison_function = comparisonFunction;

    new_queue->friendship_threshold = friendship_th;
    new_queue->rivalry_threshold = rivalry_th;

    new_queue->array = malloc(sizeof(Item) * 10);
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

        ReallocateMoreSpace(q); // reallocating more space *if needed*

        Item item = ItemCreate(q->array[i]->data); // creating a new item with the same data as the old item

        if (item == NULL){

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

        free(q);
}


// maybe should be static due to not defined in header file
// returns false if item1 is friend of item2 and true otherwise (enemies)
int IfRival(IsraeliQueue q, Item item1, Item item2){

	if (item1 == NULL || item2 == NULL || q == NULL){
	
		return 0;
	}
	
	if (q->size_of_friendship_functions == 0) {return 0;}
	
	printf("checking rivality...\n");
	
	printf("size of friendship is: %d\n",q->size_of_friendship_functions);

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

		printf("error");
        return ISRAELIQUEUE_ALLOC_FAILED;
    }
		
	printf("the size of q is: %d\n", q->size);
	
    if (q->size == 0){
		
		printf("the size of q is: %d\n", q->size);
        q->array[0] = new_item;
        q->size++;
        return ISRAELIQUEUE_SUCCESS;
    }
    

	q->array[q->size] = new_item;
	
	
    int swapIndex = q->size;
	
	printf("out of the loop: current SwapIndex is %d\n", swapIndex);

    for (int j = 0; j< q->size; j++){

        int FurtherFriend = IfExsistFurtherFriend(q, q->size, j);
		
		printf("Index of FurtherFriend is %d\n", FurtherFriend);


        if (FurtherFriend == -1){ // no friends have been found

			printf("no friend has been found!\n");
            break;

        }else{ // friend found
		
		
			printf("friend has been found!\n");

            int EnemyBlock = IfEnemyBlocked(q, FurtherFriend, q->size);
			
			printf("Index of EnemyBlock is %d\n", EnemyBlock);


            if (EnemyBlock == -1){ // no enemies have been found

				printf("no Blocking enemies has been found!\n");
                swapIndex = FurtherFriend;
				printf("current SwapIndex is %d\n", swapIndex);
                break;

            }else{// enemy found

				printf("Blocking enemies has been found!\n");
                q->array[EnemyBlock]->blockedRivals++;
                continue; // continue searching for other friend
            }


        }

    }

	printf("out of the loop!\n");
	printf("current SwapIndex is %d\n", swapIndex);

    if (swapIndex == q->size || swapIndex == (q->size-1)){ // if we get in, the item is not friend of anyone or friend of the last item

		printf(" the exp swapIndex == q->size || swapIndex == (q->size-1) is true. friends is not everything!\n");
        q->array[q->size] = new_item;
        q->size++;
        return ISRAELIQUEUE_SUCCESS;

    } else { // if we get in, the item is friend of someone 

	
		printf("item is friend of someone and there is no block!\n");
		printf("current SwapIndex is %d\n", swapIndex);

		printf("shifting all item...\n");
		printf("...\n");
		printf("...\n");
		printf("...\n");
		
        for (int i = q->size ; i > (swapIndex+1) ; i--){ // shift all items to the right

            q->array[i] = q->array[i-1];
        } 
		
		printf("done!\n");
		
		printf("current (SwapIndex+1) is %d\n", (swapIndex+1));

		
        q->array[swapIndex+1] = new_item;
        q->size++;
		
		printf("finished!\n");


        return ISRAELIQUEUE_SUCCESS;
        
    }
	
	

}

/**@param IsraeliQueue: an IsraeliQueue to which the function is to be added
 * @param FriendshipFunction: a FriendshipFunction to be recognized by the IsraeliQueue
 * going forward.
 *
 * Makes the IsraeliQueue provided recognize the FriendshipFunction provided.*/
IsraeliQueueError IsraeliQueueAddFriendshipMeasure(IsraeliQueue q, FriendshipFunction f){

    FriendshipFunction* temp = realloc(q->friendship_functions, sizeof(FriendshipFunction) * (q->size_of_friendship_functions + 1));

    if (temp == NULL){

        return ISRAELIQUEUE_ALLOC_FAILED;
    }

    q->friendship_functions = temp;
    q->friendship_functions[q->size_of_friendship_functions] = f;
    q->friendship_functions[q->size_of_friendship_functions + 1] = NULL;
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
int IfExsistFurtherFriend(IsraeliQueue q, int itemIndex, int StartingPosition){

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
IsraeliQueueError IsraeliQueueImprovePositions(IsraeliQueue q){

	
    if (q == NULL){

        return ISRAELIQUEUE_BAD_PARAM;
    }else if (q->size == 0 || q->size == 1){

        return ISRAELIQUEUE_SUCCESS;
    }


    for (int j = 0; j < q->size; j++){

        
        int ImproveCurrentItemIndex = (q->size - 1); 

        while (q->array[ImproveCurrentItemIndex]->improvedPosition == true){
			
            ImproveCurrentItemIndex--;
        }
		
		printf("current improving item index = %d\n",ImproveCurrentItemIndex);
            
        // ImproveCurrentItemIndex is the index of item that hasn't improved position yet

        // imroving position of item ImproveCurrentItemIndex


        if (ImproveCurrentItemIndex == 0){

            q->array[ImproveCurrentItemIndex]->improvedPosition = true;
            continue;

        }else{

            
            int index_of_improvment = ImproveCurrentItemIndex;


            // searching for friends
            for (int k = 0; k < ImproveCurrentItemIndex; k++){ 
                

                // searching for friends with passing ability from k to ImproveCurrentItemIndex
                int friendIndex = IfExsistFurtherFriend(q, ImproveCurrentItemIndex, k);

                if (friendIndex == -1){// no friend has been found

                    break;

                }else{//has friend in index friendIndex


                    // checking if there is a rival between the friend and the item
                    int rivalIndex = IfEnemyBlocked(q, friendIndex, ImproveCurrentItemIndex);

                    if (rivalIndex == -1){// no rival has been found, we can improve position

                        index_of_improvment = friendIndex;
                        break;

                    }else{ // enemy blocked

                        q->array[rivalIndex]->blockedRivals++;
                        // continue searching for another friend
                        continue; 

                    }


                }

                
            }

            if (index_of_improvment >= (ImproveCurrentItemIndex-1)){ // no friend has been found or friend is too close to item

                q->array[ImproveCurrentItemIndex]->improvedPosition = true;
                

            }else{

                Item temp = q->array[ImproveCurrentItemIndex];

                for (int c = ImproveCurrentItemIndex; c > (index_of_improvment+1); c--){

                    q->array[c] = q->array[c-1];
                }
                

                q->array[index_of_improvment+1] = temp;
                q->array[ImproveCurrentItemIndex]->improvedPosition = true;
                q->array[index_of_improvment]->passedFriends++;
                continue; // continue improving position of another items

            }

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


    int sum_of_functions = 0;
    int i = 0;

    while (qs[i] != NULL){

        
        int j = 0;

        while (qs[i]->friendship_functions[j] != NULL){

            sum_of_functions++;
            j++;
        }

    }

    FriendshipFunction *friendshipFunctions = malloc(sizeof(FriendshipFunction) * (sum_of_functions+1));

    if (friendshipFunctions == NULL){

        return NULL;
    }

    i = 0;
    int k = 0;

    while (qs[i] != NULL){

        int j = 0;

        while (qs[i]->friendship_functions[j] != NULL){

            friendshipFunctions[k] = qs[i]->friendship_functions[j];
            k++;
            j++;
        }

    }


    // computing friendship treshold and rivalry treshold

    int new_friendship_th = 0;
    int new_rivalry_th = 1;

    i = 0;

    while (qs[i] != NULL){

        new_friendship_th += qs[i]->friendship_threshold;
        new_rivalry_th *= qs[i]->rivalry_threshold;
        i++;
    }

    new_friendship_th = (int)((new_friendship_th + i - 1) / i); // ceiling to the nearest integer
    new_rivalry_th = (int)ceil(pow(new_rivalry_th, i)); // ceiling to the nearest integer


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

            while (qs[i]->array[j]->merged == true && j < qs[i]->size){ // searching for an item that has not been merged in qs[i]

                j++;
            }

            if (j == qs[i]->size){// all items in qs[i] have been merged, continue to next queue
                
                i++;
                continue;
            }

            Item item = ItemCreate(qs[i]->array[j]->data); // if we got here, we found an item that has not been merged

            if (item == NULL){

                return NULL;
            }

            qs[i]->array[j]->merged = true;

            IsraeliQueueError error = IsraeliQueueEnqueue(new_queue,item); 

            if (error == ISRAELIQUEUE_ALLOC_FAILED || error ==  ISRAELIQUEUE_BAD_PARAM){// if enqueue failed
                        
                return NULL;
            }

            free(item); 

            i++;
        }
        
    }

    MarkAsUnmerged(qs); // unmarking all items as merged

    // returning new_queue

    return new_queue;

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

        Item *temp = realloc(q->array, sizeof(Item) * (q->size_of_allocation * q->expend_base));

        if (temp == NULL){

            return ISRAELIQUEUE_ALLOC_FAILED;
        }

        q->array = temp;

        q->size_of_allocation = q->size_of_allocation * q->expend_base;
    }

    return ISRAELIQUEUE_SUCCESS;
}

void* getData(IsraeliQueue q, int index){
	
	return (q->array[index]->data);
	
}

