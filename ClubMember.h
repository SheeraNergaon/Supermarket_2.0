#pragma once
#include "Customer.h"


typedef struct {
	Customer* customer;
	int totalMonths;
}ClubMember;


void setTotalMonths(ClubMember* pMember);


void printClubMember(const Customer* pCustomer);
int initClubMember(Customer* pCustomer);
void initClubMemberVTable(Customer* pCustomer);
double calculateDiscount(int totalMonths);
void printTotalPriceMember(const Customer* pCustomer);
void freeClubMember(Customer* pCustomer);


