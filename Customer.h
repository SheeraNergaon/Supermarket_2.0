#pragma once
#include "ShoppingCart.h"

#define CUSTOMER_ID_LENGTH 9
#define NAMES_SEP " "
#define NAME_PARTS_SEP "- "

enum Discount {

};

enum CustomerType {
	REGULAR,
	MEMBER
};

typedef struct
{
	char*			id;
	char*			name;
	ShoppingCart*	pCart;
	enum CustomerType type;
}Customer;

typedef struct {
	Customer customer;
	int totalMonths;
	float discount;
}ClubMember;

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


void setCustomerType(Customer* pCustomer);
void setTotalMonths(ClubMember* pMember);
void setDiscount(ClubMember* pMember);
int isValidMonths(char months[], int size);
void setDiscount(ClubMember* pMember);



