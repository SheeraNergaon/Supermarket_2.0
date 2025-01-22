#pragma once
#include "Customer.h"
#include "Product.h"
#include "ShoppingCart.h"
#define eNofSortOptions 3


typedef enum {
	UNSORTED,
	SORT_BY_NAME,
	SORT_BY_COUNT,
	SORT_BY_PRICE
} ProductSortCategory;

typedef struct
{
	char*		name;
	Customer*	customerArr;
	int			customerCount;
	Product**	productArr;
	int			productCount;
	ProductSortCategory sortCategory;
}SuperMarket;




int			initSuperMarket(SuperMarket* pMarket);
void		printSuperMarket(const SuperMarket* pMarket);
int			addProduct(SuperMarket* pMarket);
int			addNewProduct(SuperMarket* pMarket);
int			addCustomer(SuperMarket* pMarket);
int			isCustomerInMarket(SuperMarket* pMarket, Customer* pCust);
int			doShopping(SuperMarket* pMarket);
Customer*	doPrintCart(SuperMarket* pMarket);
int			manageShoppingCart(SuperMarket* pMarket);
Customer*	getCustomerShopPay(SuperMarket* pMarket);
void		fillCart(SuperMarket* pMarket, ShoppingCart* pCart);
void		clearCart(SuperMarket* pMarket, Customer* pCustomer);

void		printProductByType(SuperMarket* pMarket);
void		printAllProducts(const SuperMarket* pMarket);
void		printAllCustomers(const SuperMarket* pMarket);
int			isBarcodeUnique(const SuperMarket* pMarket, const char* barcode);
int 		isCustomerIdUnique(const SuperMarket* pMarket, const char* id);


int			getProductIndexByBarcode(SuperMarket* pMarket, const char* barcode);
Product*	getProductByBarcode(SuperMarket* pMarket, const char* barcode);
Product*	getProductFromUser(SuperMarket* pMarket, char* barcode);
void		getUniquBarcode(char* barcode, SuperMarket* pMarket);
Customer*	FindCustomerById(SuperMarket* pMarket, const char* id);

Customer*	getCustomerWhoShop(SuperMarket* pMarket);
Product*	getProductAndCount(SuperMarket* pMarket, int* pCount);

void		freeMarket(SuperMarket* pMarket);
void		freeProducts(SuperMarket* pMarket);
void		freeCustomers(SuperMarket* pMarket);




int compareByName(const void* a, const void* b);
int compareByCount(const void* a, const void* b);
int compareByPrice(const void* a, const void* b);
void sortByCategory(SuperMarket* pMarket);
int sortMenu();

int searchBySortedType(SuperMarket* pMarket);
