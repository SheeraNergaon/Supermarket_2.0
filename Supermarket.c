#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Supermarket.h"
#include "Product.h"
#include "Customer.h"
#include "General.h"
#include "ShoppingCart.h"
#include "ClubMember.h"
#include "SuperMarketBin.h"
#include "CustomerText.h"

#define TEXT_FILE "Customers.txt"
#define BIN_FILE "SuperMarket.bin"
#define TEXT_TO_BIN

int		initSuperMarket(SuperMarket* pMarket)
{
	if (readCustomerArrFromTextFile(TEXT_FILE, pMarket) && readSuperMarketFromBinaryFile(pMarket, BIN_FILE)) { 
		printf("Supermarket succesfully loaded from files:\n");
		return 1;
	}

	pMarket->customerCount = 0;
	pMarket->customerArr = NULL;
	pMarket->productCount = 0;
	pMarket->productArr = NULL;
	pMarket->name = getStrExactLength("Enter market name");
	pMarket->sortCategory = UNSORTED; // Start as unsorted

	if (!pMarket->name)
		return 0;

	return 1;
}

void	printSuperMarket(const SuperMarket* pMarket)
{

	printf("Super Market Name: %s\t", pMarket->name);
	printf("\n");
	printAllProducts(pMarket);
	printf("\n");
	printAllCustomers(pMarket);
}

int		addProduct(SuperMarket* pMarket)
{
	char answer;
	char barcode[BARCODE_LENGTH + 1];
	Product* pProd = NULL;

	printf("\n");
	printf("Adding new product? y/Y: ");
	scanf("%c", &answer);
	getchar();

	if (toupper(answer) == 'Y')
	{
		if (!addNewProduct(pMarket))
		{
			free(pProd);
			return 0;
		}
		return 1;
	}
	else if (pMarket->productCount > 0)
	{
		printf("Do you want to increase the amount of an existing product? y/Y: ");
		scanf("%c", &answer);
		getchar();
		if (toupper(answer) == 'Y')
		{
			printAllProducts(pMarket);
			pProd = getProductFromUser(pMarket, barcode);
			if (pProd != NULL) //This barcode exist in stock
				updateProductCount(pProd);
		}
	}
	else
		return 0;
	pMarket->sortCategory = UNSORTED;
	return 1;
}

int		addNewProduct(SuperMarket* pMarket)
{

	Product** pTempArr = (Product**)realloc(pMarket->productArr, (pMarket->productCount + 1) * sizeof(Product*));
	if (!pTempArr)
	{
		freeProducts(pMarket);
		return 0;
	}
	pMarket->productArr = pTempArr;
	
	Product* pProd = (Product*)calloc(1, sizeof(Product));
	if (!pProd)
	{
		free(pMarket->productArr);
		return 0;
	}

	initProductNoBarcode(pProd);

	do
	{
		generateBarcode(pProd);
	} while (!isBarcodeUnique(pMarket, pProd->barcode)); //generate new barcode until it is unique
	
	pMarket->productArr[pMarket->productCount] = pProd;
	pMarket->productCount++;
	pMarket->sortCategory = UNSORTED;
	return 1;
}

int		isBarcodeUnique(const SuperMarket* pMarket, const char* barcode)
{
	for (int i = 0; i < pMarket->productCount; i++)
	{
		if (strcmp(pMarket->productArr[i]->barcode, barcode) == 0)
			return 0; // Barcode is not unique
	}
	return 1; // Barcode is unique
}


int isCustomerIdUnique(const SuperMarket* pMarket, const char* id)
{

	for (int i = 0; i < pMarket->customerCount; i++)
	{

		const char* currentId;
		currentId = pMarket->customerArr[i].id;

		if (strcmp(currentId, id) == 0) {
			printf("ID %s is not unique\n", id);
			return 0;
		}
	}
	return 1;
}



int	addCustomer(SuperMarket* pMarket)
{
	Customer customer;
	int clubMember = 0;

	printf("Is the customer a club memeber? 1 for yes, 0 for no: ");
	scanf(" %d", &clubMember);

	do
	{
		if (clubMember == 1)
		{
			if (!initClubMember(&customer))
			{
				return 0;
			}
		}
		else
			if (!initCustomer(&customer))
			{
				return 0;
			}
	} while (!isCustomerIdUnique(pMarket, customer.id));

	if (isCustomerInMarket(pMarket, &customer))
	{
		printf("This customer is already exist in market\n");
		freeCustomer(&customer);
		return 0;
	}

	pMarket->customerArr = (Customer*)safeRealloc(pMarket->customerArr,
		(pMarket->customerCount + 1) * sizeof(Customer));

	if (!pMarket->customerArr)
	{
		freeCustomer(&customer);
		return 0;
	}

	pMarket->customerArr[pMarket->customerCount] = customer;
	pMarket->customerCount++;

	return 1;
}

int		isCustomerInMarket(SuperMarket* pMarket, Customer* pCust)
{
	for (int i = 0; i < pMarket->customerCount; i++)
	{
		if (strcmp(pMarket->customerArr[i].name, pCust->name) == 0)
			return 1;
	}
	return 0;
}

int	doShopping(SuperMarket* pMarket)
{
	Customer* pCustomer = getCustomerShopPay(pMarket);
	if (!pCustomer)
		return 0;

	if (pCustomer->pCart == NULL)
	{
		pCustomer->pCart = (ShoppingCart*)malloc(sizeof(ShoppingCart));
		if (!pCustomer->pCart)
			return 0;
		initCart(pCustomer->pCart);
	}
	fillCart(pMarket, pCustomer->pCart);
	if (!pCustomer->pCart) //did not buy any thing
	{
		free(pCustomer->pCart);
		pCustomer->pCart = NULL;
	}
	printf("---------- Shopping ended ----------\n");
	return 1;
}

Customer*	doPrintCart(SuperMarket* pMarket)
{
	float price;
	Customer* pCustomer = getCustomerShopPay(pMarket);
	if (!pCustomer)
		return NULL;
	if (pCustomer->pCart == NULL)
	{
		printf("Customer cart is empty\n");
		return NULL;
	} 
	price = printShoppingCart(pCustomer->pCart);
	pCustomer->table.printPrice(pCustomer);
	return pCustomer;
}

int	manageShoppingCart(SuperMarket* pMarket)
{
	Customer* pCustomer = doPrintCart(pMarket);
	char answer;

	if(!pCustomer)
		return 0;

	printf("Do you want to pay for the cart? y/Y, anything else to cancel shopping!\t");
	do {
		scanf("%c", &answer);
	} while (isspace(answer));

	getchar(); //clean the enter

	if (answer == 'y' || answer == 'Y')
		pay(pCustomer);
	else {
		clearCart(pMarket, pCustomer);
		cancelShopping(pCustomer);
	}
	return 1;

}

Customer*	getCustomerShopPay(SuperMarket* pMarket)
{
	if (pMarket->customerCount == 0)
	{
		printf("No customer listed to market\n");
		return NULL;
	}

	if (pMarket->productCount == 0)
	{
		printf("No products in market - cannot shop\n");
		return NULL;
	}

	Customer* pCustomer = getCustomerWhoShop(pMarket);
	if (!pCustomer)
	{
		printf("this customer is not listed\n");
		return NULL;
	}

	return pCustomer;
}

void	printAllProducts(const SuperMarket* pMarket)
{
	printf("There are %d products\n", pMarket->productCount);
	printf("%-20s %-10s\t", "Name", "Barcode");
	printf("%-20s %-10s %-20s %-15s\n", "Type", "Price", "Count In Stoke", "Expiry Date");
	printf("-------------------------------------------------------------------------------------------------\n");

	generalArrayFunction(pMarket->productArr, pMarket->productCount, sizeof(Product*), printProduct);
}

void	printAllCustomers(const SuperMarket* pMarket)
{
	printf("There are %d listed customers\n", pMarket->customerCount);
	for (int i = 0; i < pMarket->customerCount; i++)
		printCustomer(&pMarket->customerArr[i]);
	
}


Customer* getCustomerWhoShop(SuperMarket* pMarket)
{
	printAllCustomers(pMarket);
	char searchTerm[MAX_STR_LEN];
	getsStrFixSize(searchTerm, sizeof(searchTerm), "Who is shopping? Enter customer id\n");

	Customer* pCustomer = FindCustomerById(pMarket, searchTerm);
	
	return pCustomer;
}


void fillCart(SuperMarket* pMarket, ShoppingCart* pCart)
{
	printAllProducts(pMarket);
	char op;
	while (1)
	{
		printf("Do you want to shop for a product? y/Y, anything else to exit!!\t");
		do {
			scanf("%c", &op);
		} while (isspace(op));
		getchar(); //clean the enter
		if (op != 'y' && op != 'Y')
			break;
		int count;
		Product* pProd = getProductAndCount(pMarket, &count);
		if(pProd)
		{
			if (!addItemToCart(pCart, pProd->barcode, pProd->price, count))
			{
				printf("Error adding item\n");
				return;
			}
			pProd->count -= count; //item bought!!!
		}
	}
}

void clearCart(SuperMarket* pMarket, Customer* pCustomer)
{
	if (!pCustomer->pCart)
		return;

	NODE* current = pCustomer->pCart->items.head.next;
	while (current) {
		ShoppingItem* pItem = (ShoppingItem*)current->key;
		Product* pProd = getProductByBarcode(pMarket, pItem->barcode);

		if (pProd) {
			pProd->count += pItem->count; // Restore the count to the product
		}

		current = current->next; // Move to the next item in the cart
	}
}

Product* getProductAndCount(SuperMarket* pMarket, int* pCount)
{
	char barcode[BARCODE_LENGTH + 1];
	Product* pProd = getProductFromUser(pMarket, barcode);
	if (pProd == NULL)
	{
		printf("No such product\n");
		return NULL;
	} 
	
	if (pProd->count == 0)
	{
		printf("This product is out of stock\n");
		return NULL;
	}
	
	int count;
	do {
		printf("How many items do you want? max %d\n", pProd->count);
		scanf("%d", &count);
	} while (count <= 0 || count > pProd->count);
	*pCount = count;
	return pProd;
}

void	printProductByType(SuperMarket* pMarket)
{
	if (pMarket->productCount == 0)
	{
		printf("No products in market\n");
		return;
	}
	eProductType	type = getProductType();
	int count = 0;
	for (int i = 0; i < pMarket->productCount; i++)
	{
		if (pMarket->productArr[i]->type == type)
		{
			count++;
			printProduct(pMarket->productArr[i]);
		}
	}
	if (count == 0)
		printf("There are no product of type %s in market %s\n", getProductTypeStr(type), pMarket->name);
}

Product* getProductFromUser(SuperMarket* pMarket, char* barcode)
{
	getBarcodeCode(barcode);
	Product* pProd = getProductByBarcode(pMarket, barcode);
	if (!pProd)
	{
		printf("No such product barcode\n");
		return NULL;
	}
	return pProd;
}

void	freeMarket(SuperMarket* pMarket)
{
	free(pMarket->name);
	freeProducts(pMarket);
	freeCustomers(pMarket);
}

void freeProducts(SuperMarket* pMarket)
{
	for (int i = 0; i < pMarket->productCount; i++)
	{
		freeProduct(pMarket->productArr[i]);
		free(pMarket->productArr[i]);
	}
	free(pMarket->productArr);
}

void freeCustomers(SuperMarket* pMarket)
{
	for (int i = 0; i < pMarket->customerCount; i++)
		freeCustomer(&pMarket->customerArr[i]);
	free(pMarket->customerArr);
}

void	getUniquBarcode(char* barcode, SuperMarket* pMarket)
{
	int cont = 1;
	while (cont)
	{
		getBarcodeCode(barcode);
		int index = getProductIndexByBarcode(pMarket, barcode);
		if (index == -1)
			cont = 0;
		else
			printf("This product already in market\n");
	}
}

int getProductIndexByBarcode(SuperMarket* pMarket, const char* barcode)
{
	for (int i = 0; i < pMarket->productCount; i++)
	{
		if (isProduct(pMarket->productArr[i], barcode))
			return i;
	}
	return -1;
}

Product* getProductByBarcode(SuperMarket* pMarket, const char* barcode)
{
	for (int i = 0; i < pMarket->productCount; i++)
	{
		if (isProduct(pMarket->productArr[i], barcode))
			return pMarket->productArr[i];
	}
	return NULL;

}


Customer* FindCustomerById(SuperMarket* pMarket, const char* id)
{
	for (int i = 0; i < pMarket->customerCount; i++)
	{
		if (isCustomerById(&pMarket->customerArr[i], id))
			return &pMarket->customerArr[i];
	}
	return  NULL;
}


int compareByName(const void* a, const void* b) {
	Product* prodA = *(Product**)a;
	Product* prodB = *(Product**)b;

	// Temporary lowercase versions of the strings
	char* nameA = prodA->name;
	char* nameB = prodB->name;
	while (*nameA && *nameB) {
		char lowerA = tolower(*nameA);
		char lowerB = tolower(*nameB);

		if (lowerA != lowerB) {
			return (lowerA - lowerB);
		}

		nameA++;
		nameB++;
	}
	return (tolower(*nameA) - tolower(*nameB));
}



int compareByCount(const void* a, const void* b) {
	Product* prodA = *(Product**)a;
	Product* prodB = *(Product**)b;
	return prodA->count - prodB->count; 
}

int compareByPrice(const void* a, const void* b) {
	Product* prodA = *(Product**)a;
	Product* prodB = *(Product**)b;

	if (prodA->price < prodB->price)
		return -1; // Return -1 if prodA is cheaper than prodB
	else if (prodA->price > prodB->price)
		return 1;  // Return 1 if prodA is more expensive than prodB
	else
		return 0;  // Return 0 if both prices are equal
}

int sortMenu() {
	const char* sortMenuStrings[] = { "Sort by Name","Sort by Count","Sort by Price"};
	int option;
	printf("\n");
	printf("How would you like to sort the products?\n");
	for (int i = 0; i < eNofSortOptions; i++) {
		printf("%d - %s\n", i, sortMenuStrings[i]);
	}
	printf("Enter your choice (0-%d): ", eNofSortOptions - 1);
	scanf("%d", &option);

	// Clean buffer
	char tav;
	scanf("%c", &tav);

	return option;
}


void sortByCategory(SuperMarket* pMarket) {
	int option = sortMenu();

	switch (option) {
	case 0:
		pMarket->sortCategory = SORT_BY_NAME;
		qsort(pMarket->productArr, pMarket->productCount, sizeof(Product*), compareByName);
		break;

	case 1:
		pMarket->sortCategory = SORT_BY_COUNT;
		qsort(pMarket->productArr, pMarket->productCount, sizeof(Product*), compareByCount);
		break;

	case 2:
		pMarket->sortCategory = SORT_BY_PRICE;
		qsort(pMarket->productArr, pMarket->productCount, sizeof(Product*), compareByPrice);
		break;

	default:
		printf("Invalid choice. Products will remain unsorted.\n");
		pMarket->sortCategory = UNSORTED;
		break;
	}
}

int searchBySortedType(SuperMarket* pMarket) {
	if (!pMarket || !pMarket->productArr || pMarket->productCount <= 0) {
		return 0; 
	}

	Product tempProduct;         // Temporary product for search 
	Product* pTempProduct = &tempProduct; // Pointer to pass to bsearch
	Product** pFoundProduct = NULL; // Pointer to store the found product

	switch (pMarket->sortCategory) {
	case UNSORTED:
		printf("Products are not sorted.\n");
		return 1;

	case SORT_BY_NAME:
		printf("The array is sorted by name. Please enter the name of the product to search:\n");
		initProductName(&tempProduct); 
		pFoundProduct = (Product**)bsearch(&pTempProduct,pMarket->productArr,pMarket->productCount,sizeof(Product*),compareByName
		);
		break;

	case SORT_BY_COUNT:
		printf("The array is sorted by count. Please enter the count of the product to search:\n");
		tempProduct.count = getPositiveInt("Enter product count: ");
		pFoundProduct = (Product**)bsearch(&pTempProduct,pMarket->productArr,pMarket->productCount,sizeof(Product*),compareByCount
		);
		break;

	case SORT_BY_PRICE:
		printf("The array is sorted by price. Please enter the price of the product to search:\n");
		tempProduct.price = getPositiveFloat("Enter product price: ");
		pFoundProduct = (Product**)bsearch(&pTempProduct,pMarket->productArr,pMarket->productCount,sizeof(Product*),compareByPrice
		);
		break;

	default:
		printf("Invalid sorting category.\n");
		return 0; 
	}

	if (pFoundProduct) {
		printf("Product found:\n");
		printProduct(*pFoundProduct); 
	}
	else {
		printf("Product not found.\n");
	}

	return 1; 
}
