#pragma once
#include <stdio.h>
#include "Supermarket.h"
#include "Customer.h"

int readCustomerFromTextFile(FILE* fileName, Customer* pCustomer);
Customer* readCustomerArrFromTextFile(const char* fileName, SuperMarket* market);
int writeToTextFile(const SuperMarket* pMarket, const char* filename);
