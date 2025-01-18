#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ShoppingCart.h"
#include "General.h"
#include "list.h"


void	initCart(ShoppingCart* pCart)
{
	L_init(&(pCart->items));
}

float	getTotalPrice(const ShoppingCart* pCart)
{
	NODE* current = &(pCart->items.head);
	current = current->next;
	float price = 0;
	while (current) {
		price += ((ShoppingItem*)current->key)->price * ((ShoppingItem*)current->key)->count;
		current = current->next;
	}
	return price;
}

int	 addItemToCart(ShoppingCart* pCart, const char* barcode, float price, int count)
{
	NODE* prev = &(pCart->items.head);
	NODE* current = prev->next;
	ShoppingItem* newItem;

	while (current != NULL && strcmp(((ShoppingItem*)current->key)->barcode, barcode) < 0)
	{
		prev = current;
		current = current->next;
	}
	if (current != NULL && strcmp(((ShoppingItem*)current->key)->barcode, barcode) == 0)
	{
		((ShoppingItem*)current->key)->count += count;
		return 1;
	}

	newItem = createItem(barcode, price, count);
	if (!newItem)
		return 0;

	if (!L_insert(prev, newItem))
	{
		free(newItem);
		return 0;
	}
	return 1;
}

float	printShoppingCart(const ShoppingCart* pCart){ // we will need to change anyway according to club member

	float price = 0;
	L_print(&(pCart->items), printItem);
	price = getTotalPrice(pCart);
	printf("Total bill to pay: %.2f\n", price);
	return price;
}



ShoppingItem*	getItemByBarocde(ShoppingCart* pCart, const char* barcode)
{
	NODE* current = &(pCart->items.head);
	current = current->next;
	while (current)
	{
		if (strcmp(((ShoppingItem*)current->key)->barcode, barcode) == 0)
			return (ShoppingItem*)current->key;
		current = current->next;
	}
	return NULL;
}

void freeShoppingCart(ShoppingCart* pCart)
{
	if (!pCart) return;
	L_free(&(pCart->items), NULL);  // Pass NULL since ShoppingItem does not need extra freeing

}