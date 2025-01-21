#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Customer.h"
#include "General.h"

int	initCustomer(Customer* pCustomer)
{
	char firstName[MAX_STR_LEN];
	char lastName[MAX_STR_LEN];

	char* parts[2] = { firstName,lastName };
	char* msgParts[2] = { "Enter customer first name\n" ,"Enter customer last name\n" };
	for(int i = 0; i < 2; i++)
	{ 
		getNamePart(parts[i], msgParts[i]);
		upperLowerCustomerName(parts[i]);
	}

	char* combineName = combineFirstLast(parts);
	if (!combineName)
		return 0;

	pCustomer->name = combineName;
	pCustomer->pCart = NULL;

	getCustomerID(pCustomer);
	pCustomer->pDerivedObject = NULL;
	initCustomerVTable(pCustomer);

	return 1;
	
}

//void setCustomerType(Customer* pCustomer)
//{
// 	 char choice;
//     do {
//       	printf("Are you a club member? enter Y,y if yes, and N,n if no:\n");
//     	choice = getchar();
//     	while ( getchar() != '\n' );
//     	switch (choice) {
//       	case 'N':
//        case 'n':
//         	pCustomer->type = REGULAR;
//            break;
//       	case 'Y':
//        case 'y':
//         	pCustomer->type = MEMBER;
//     		ClubMember* pMember = (ClubMember*)pCustomer;
//         	break;
//       	default:
//         	printf("Invalid input, try again!\n");
//		}
//     } while (choice != 'N' && choice != 'n' && choice != 'Y' && choice != 'y');
//}
//
//void setTotalMonths(ClubMember* pMember) {
//	char temp[10];
//	char* newlinePos;
//	do {
//		printf("Enter total months of membership: ");
//		fgets(temp, sizeof(temp), stdin);
//
//		// Clear buffer if input was too long
//		if (!strchr(temp, '\n'))
//			while (getchar() != '\n');
//
//		// Remove newline
//		newlinePos = strchr(temp, '\n');
//		if (newlinePos)
//			*newlinePos = '\0';
//
//	} while (!isValidMonths(temp, strlen(temp)));
//
//	pMember->totalMonths = atoi(temp);
//}
//
//
//int isValidMonths(char months[], int size){
//	for (int i = 0; i < size; i++){
//    	if (months[i] < '0' || months[i] > '9'){
//            return 0;
//          }
//    }
//   	return 1;
//}
//
//void setDiscount(ClubMember* pMember) {
//  int seniority = pMember->totalMonths;
//
//  if (seniority > 0 && seniority < 24) {
//    pMember->discount = seniority * 0.1;
//  }
//  if (seniority < 60) {
//    pMember->discount = 2.5 + 0.5 * seniority / 12;
//  }
//  else {
//  	pMember->discount = 7.5;
//  }
//
//}

void getCustomerID(Customer* pCustomer)
{
	char msg[MAX_STR_LEN];
	sprintf(msg, "ID should be %d digits\n"
		"For example: 123456789\n", CUSTOMER_ID_LENGTH);
	char tempId[MAX_STR_LEN];

	do {
		getsStrFixSize(tempId, MAX_STR_LEN, msg);
	} while (!isCustomerIdValid(tempId));

	pCustomer->id = myStrDup(tempId);
}


void	getNamePart(char* part, const char* msg)
{
	int ok = 0;
	do {
		getsStrFixSize(part, MAX_STR_LEN, msg);
		if (checkEmptyString(part))
			printf("Name can not be empty\n");
		else if (!checkAlphaSpaceStr(part))
			printf("Name should contain only letters\n");
		else
			ok = 1;
	} while (!ok);
}

void	upperLowerCustomerName(char* name)
{
	toLowerStr(name);
	while (isspace(*name))
		name++;

	*name = toupper(*name);
}

char* combineFirstLast(char** parts)
{
	char temp[MAX_STR_LEN * 2] = { 0 };
	char** pNameParts = NULL;
	int count = 0;
	size_t totalLength = 0;

	for (int i = 0; i < 2; i++)
	{
		pNameParts = splitCharsToWords(parts[i], &count, NAMES_SEP, &totalLength);
		if (!pNameParts)
			return NULL;

		for (int j = 0; j < count; j++)
		{
			strcat(temp, pNameParts[j]);
			strcat(temp, NAMES_SEP);
			free(pNameParts[j]); //do not need
		}
		free(pNameParts);
		pNameParts = NULL;

		if (i == 0)
			strcat(temp, NAME_PARTS_SEP);
	}
	temp[strlen(temp) - 1] = '\0'; //remove last space
	
	char* combineName = myStrDup(temp);
	return combineName;
}

void printCustomer(const Customer* pCustomer)
{
	if (!pCustomer) return;
	printf("Name: %s\n", pCustomer->name);
	printf("ID: %s\n", pCustomer->id);
	if (pCustomer->pCart == NULL)
		printf("Shopping cart is empty!\n");
	else
	{
		printf("Doing shopping now!!!\n");
	}
}

int isCustomerIdValid(const char* id)
{
	if (strlen(id) != CUSTOMER_ID_LENGTH)
		return 0;
	for (int i = 0; i < CUSTOMER_ID_LENGTH; i++)
	{
		if (!isdigit(id[i]))
			return 0;
	}
	return 1;
}


void pay(Customer* pCustomer)
{
	if (!pCustomer->pCart)
		return;
	pCustomer->table.printPrice(pCustomer);
	//float price = getTotalPrice(pCustomer->pCart);
	//printf("---------- Cart info and bill for %s is: %f ----------\n", pCustomer->name, price);
	//printShoppingCart(pCustomer->pCart);
	printf("!!! --- Payment was recived!!!! --- \n");
	freeShoppingCart(pCustomer->pCart);
	free(pCustomer->pCart);
	pCustomer->pCart = NULL;
}

void cancelShopping(Customer* pCustomer)
{
	if (!pCustomer->pCart)
		return;
	printf("!!! --- Purchase was canceled!!!! --- \n");
	freeShoppingCart(pCustomer->pCart);
	free(pCustomer->pCart);
	pCustomer->pCart = NULL;
}


int	isCustomerById(const Customer* pCust, const char* id)
{
	if (strcmp(pCust->id, id) == 0)
		return 1;
	return 0;
}

void freeCustomer(Customer* pCust)
{
	if (pCust->pCart)
		pay(pCust); //will free every thing
	free(pCust->name);
	free(pCust->id);
	pCust->id = NULL;
	pCust->name = NULL;
	pCust->pCart = NULL;
}


//enum CustomerType getCustomerType(void) {
//	char choice;
//	do {
//		printf("Are you a club member? enter Y,y if yes, and N,n if no:\n");
//		choice = getchar();
//		while (getchar() != '\n');
//
//		switch (choice) {
//			case 'N':
//			case 'n':
//				return REGULAR;
//			case 'Y':
//			case 'y':
//				return MEMBER;
//			default:
//				printf("Invalid input, try again!\n");
//		}
//	} while (1);
//}

void initCustomerVTable(Customer* pCustomer) {
	pCustomer->table.init = initCustomer;
	pCustomer->table.print = printCustomer;
	//pCustomer->table.payment = pay;
	pCustomer->table.printPrice = printTotalPrice;
	pCustomer->table.delete = freeCustomer;


}

void printTotalPrice(const Customer* pCustomer)
{
	float price = 0;
	price = getTotalPrice(pCustomer->pCart);
	printf("Total price for %s is %.2f\n", pCustomer->name, price);
}