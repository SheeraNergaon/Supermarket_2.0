#pragma once
#include "Supermarket.h"
#include "Product.h"

int readSuperMarketFromBinaryFile(SuperMarket* pMarket, const char* fileName);
int readProductFromBinaryFile(FILE* fp, Product* pProduct);
int writeSuperMarketToBin(const SuperMarket* pMarket, const char* filename);