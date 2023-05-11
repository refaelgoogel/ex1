#ifndef ITEM_H
#define ITEM_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <math.h>

// this is the item type represent cell in IsraeliQueue
typedef struct Item {

    void *data;
    bool improvedPosition;
    bool merged;
    int passedFriends;
    int blockedRivals;

    
} *Item;


// this function creates a new item, given the data
Item ItemCreate(void *data);
// this function destroys the item
void ItemDestroy(Item item);

#endif