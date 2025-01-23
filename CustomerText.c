#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CustomerText.h"
#include "Customer.h"
#include "ClubMember.h"
#include "General.h"

int writeToTextFile(const SuperMarket* pMarket, const char* filename) {
	FILE* fp = fopen(filename, "w");
	if (!fp) return 0;

	fprintf(fp, "%d\n", pMarket->customerCount);
	for (int i = 0; i < pMarket->customerCount; i++) {
		Customer* pCustomer = &pMarket->customerArr[i];
		fprintf(fp, "%s\n%s\n", pCustomer->name, pCustomer->id);
		if (pCustomer->pDerivedObject) {
			ClubMember* pMember = (ClubMember*)pCustomer->pDerivedObject;
			if (!pMember) return 0;
			fprintf(fp, "1 %d\n", pMember->totalMonths);
		}
		else {
			fprintf(fp, "0 0\n");
		}
	}
	fclose(fp);
	printf("Supermarket data saved successfully to %s\n", filename);
	return 1;
}

int readCustomerFromTextFile(FILE* fileName, Customer* pCustomer) {
	int choiceCustomerType, totalMonths;
	char tempName[MAX_STR_LEN] = { 0 }, tempId[MAX_STR_LEN] = { 0 };

	if (!fgets(tempName, MAX_STR_LEN, fileName)) return 0;
	tempName[strcspn(tempName, "\n")] = '\0';
	pCustomer->name = (char*)malloc(sizeof(char) * (strlen(tempName) + 1));
	if (!pCustomer->name) return 0;
	strcpy(pCustomer->name, tempName);

	if (fscanf(fileName, "%s", tempId) != 1) return 0;
	pCustomer->id = strdup(tempId);


	if (fscanf(fileName, "%d %d\n", &choiceCustomerType, &totalMonths) != 2) return 0;
	if (choiceCustomerType == 1) {
		pCustomer->pDerivedObject = (ClubMember*)malloc(sizeof(ClubMember));
		if (!pCustomer->pDerivedObject) return 0;
		ClubMember* pMember = (ClubMember*)pCustomer->pDerivedObject;
		pMember->totalMonths = totalMonths;
		pCustomer->pCart = NULL;
		pMember->customer = pCustomer->pDerivedObject;
		initClubMemberVTable(pCustomer);
	}
	else {
		pCustomer->pCart = NULL;
		pCustomer->pDerivedObject = NULL;
		initCustomerVTable(pCustomer);
	}
	return 1;


}
Customer* readCustomerArrFromTextFile(const char* fileName, SuperMarket* market) {
	FILE* fp = fopen(fileName, "r");
	if (!fp) {
		return NULL;
	}
	int customerCount;
	if (fscanf(fp, "%d\n", &customerCount) != 1) {
		fclose(fp);
		return NULL;
	}
	market->customerCount = customerCount;
	Customer* pCustomerArr = (Customer*)malloc(sizeof(Customer) * customerCount);
	if (!pCustomerArr) {
		fclose(fp);
		return NULL;
	}
	market->customerArr = pCustomerArr;

	for (int i = 0; i < market->customerCount; i++) {
		if (!readCustomerFromTextFile(fp, &pCustomerArr[i])) {
			fclose(fp);
			return NULL;
		}
	}
	fclose(fp);
	return pCustomerArr;
}