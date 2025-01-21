#pragma once
#include "ShoppingCart.h"
#include "vtable.h"

#define CUSTOMER_ID_LENGTH 9
#define NAMES_SEP " "
#define NAME_PARTS_SEP "- "


enum CustomerType {
	REGULAR,
	MEMBER
};

typedef struct _Costumer
{
	char*			id;
	char*			name;
	ShoppingCart*	pCart;
	void* pDerivedObject;
	enum CustomerType type;
	CustomerVTable table;
}Customer;


int		initCustomer(Customer* pCustomer);
void	getCustomerID(Customer* pCustomer);
void	printCustomer(const Customer* pCustomer);

int		isCustomerIdValid(const char* id);

int		isCustomerById(const Customer* pCust, const char* id);
//int		isCustomersNameValid(const char* name);
void	getNamePart(char* part, const char* msg);
void	upperLowerCustomerName(char* name);
//char*	combineFirstLast(char*  firstName, char* lastName);
char*	combineFirstLast(char** parts);
void	pay(Customer* pCustomer);
void	cancelShopping(Customer* pCustomer);

void	freeCustomer(Customer* pCust);

void initCustomerVTable(Customer* pCustomer);
void printTotalPrice(const Customer* pCustomer);



