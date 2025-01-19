#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include  "ShoppingItem.h"


void printItem(void* data) {
	const ShoppingItem* item = (ShoppingItem*)data;
	printf("Barcode: %-13s Price: %6.2f  Count: %3d\n",
		   item->barcode,
		   item->price,
		   item->count);
}

ShoppingItem*	createItem(const char* barcode, float price, int count)
{
	ShoppingItem* pItem = (ShoppingItem*)malloc(1 * sizeof(ShoppingItem));
	if (!pItem)
		return NULL;
	strcpy(pItem->barcode, barcode);
	pItem->price = price;
	pItem->count = count;
	return pItem;
}