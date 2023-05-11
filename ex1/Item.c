#include "Item.h"


// creting a new item
Item ItemCreate(void *data){

    Item new_item = (Item)malloc(sizeof(*new_item));

    if (new_item == NULL){

        return NULL;
    }

    new_item->data = data;
    new_item->passedFriends = 0;
    new_item->blockedRivals = 0;

    // for the improve position function
    new_item->improvedPosition = false;

    // for the merge function
    new_item->merged = false;

    return new_item;

}

// destroying an item
void ItemDestroy(Item item){
    
    free(item);

}
