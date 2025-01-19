#pragma once
#include "Product.h"

typedef struct
{
	char		barcode[BARCODE_LENGTH + 1];
	float		price;
	int			count;
}ShoppingItem;

void			printItem(void* data);
ShoppingItem*	createItem(const char* barcode, float price, int count);
