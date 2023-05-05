#ifndef ITEM_H
#define ITEM_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <math.h>


typedef struct Item {

    void *data;
    bool improvedPosition;
    bool merged;
    int passedFriends;
    int blockedRivals;

    
} *Item;

Item ItemCreate(void *data);
void ItemDestroy(Item item);

#endif