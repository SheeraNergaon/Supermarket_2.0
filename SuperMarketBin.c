#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include"SuperMarketBin.h"


int readSuperMarketFromBinaryFile(SuperMarket* pMarket, const char* fileName) {
    FILE* fp = fopen(fileName, "rb");
    if (!fp) {
        return 0;
    }
    int nameLength; //read and store the length of the name
    if (fread(&nameLength, sizeof(int), 1, fp) != 1) {
        fclose(fp);
        return 0;
    }
    pMarket->name = (char*)malloc(nameLength); //allocate memory for the name with the lengthwe found
    if (!pMarket->name) {
        fclose(fp);
        return 0;
    }
    if (fread(pMarket->name, sizeof(char), nameLength, fp) != nameLength) { // finally read and store the name in the variable pmarket->name
        free(pMarket->name);
        fclose(fp);
        return 0;
    }
    if (fread(&pMarket->productCount, sizeof(int), 1, fp) != 1) {
        free(pMarket->name);
        fclose(fp);
        return 0;
    }

    pMarket->productArr = (Product**)malloc(pMarket->productCount * sizeof(Product)); // allocate memory for the product array with the size that we read
    if (!pMarket->productArr) {
        free(pMarket->name);
        fclose(fp);
        return 0;
    }

    for (int i = 0; i < pMarket->productCount; i++) {

        pMarket->productArr[i] = (Product*)malloc(sizeof(Product));
        if (!pMarket->productArr[i] || !readProductFromBinaryFile(fp, pMarket->productArr[i]))
        {
            fclose(fp);
            return 0;
        }
    }
    fclose(fp);
    return 1;
}

int readProductFromBinaryFile(FILE* fp, Product* pProduct) {

    if (!fp || !pProduct)
        return 0;

    if (fread(pProduct, sizeof(Product), 1, fp) != 1)
    {
        return 0;
    }

    return 1;

}



int writeSuperMarketToBin(const SuperMarket* pMarket, const char* filename)
{
    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        printf("Error: Cannot open file %s for writing\n", filename);
        return 0;
    }


    int nameLen = (int)strlen(pMarket->name) + 1;
    fwrite(&nameLen, sizeof(int), 1, fp);
    fwrite(pMarket->name, sizeof(char), nameLen, fp);


    fwrite(&pMarket->productCount, sizeof(int), 1, fp);


    for (int i = 0; i < pMarket->productCount; i++) {
        fwrite(pMarket->productArr[i], sizeof(Product), 1, fp);
    }

    fclose(fp);
    printf("Supermarket data saved successfully to %s\n", filename);
    return 1;
}

