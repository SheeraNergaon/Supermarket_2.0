#pragma once
#include "ShoppingItem.h"
#include "list.h"

typedef struct ShoppingCart
{
	LIST items;
}ShoppingCart;


void initCart(ShoppingCart* pCart);
float getTotalPrice(const ShoppingCart* pCart);
//float getTotalPriceClubMember(const ShoppingCart* pCart);
ShoppingItem* getItemByBarocde(ShoppingCart* pCart, const char* barcode);
int	addItemToCart(ShoppingCart* pCart, const char* barcode, float price, int count);
float printShoppingCart(const ShoppingCart* pCart);
void freeShoppingCart(ShoppingCart* pCart);