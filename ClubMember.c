#include "ClubMember.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void setTotalMonths(ClubMember* pMember) {
    int totalMonths;
    printf("Enter total months of membership (positive number): ");
    while (1) {
        // Read an integer directly
        scanf("%d", &totalMonths);
        // Validate the input is positive
        if (totalMonths < 0) {
            printf("Total months cannot be negative. Try again.\n");
            continue;
        }

        // Valid input; assign and exit
        pMember->totalMonths = totalMonths;
        break;
    }

    // Clear any extra input from the buffer
    while (getchar() != '\n');
}




void printClubMember(const Customer* pCustomer) {
    printCustomer(pCustomer);
    ClubMember* pMember = (ClubMember*)pCustomer->pDerivedObject;
    printf("Total month of membership: %d\n", pMember->totalMonths);
}

int initClubMember(Customer* pCustomer) {
    if (!initCustomer(pCustomer)) return 0;
    initClubMemberVTable(pCustomer);

    pCustomer->pDerivedObject = (ClubMember*)malloc(sizeof(ClubMember));
    if (!pCustomer->pDerivedObject)
    {
        freeCustomer(pCustomer);
        return 0;
    }

    ClubMember* theClubMember = (ClubMember*)pCustomer->pDerivedObject;
    setTotalMonths(theClubMember);
    initClubMemberVTable(pCustomer);
    theClubMember->customer = pCustomer;
    return 1;

}
void initClubMemberVTable(Customer* pCustomer) {
    pCustomer->table.init = initClubMember;
    pCustomer->table.print = printClubMember;
    pCustomer->table.delete = freeClubMember;
    pCustomer->table.printPrice = printTotalPriceMember;
}
      
double calculateDiscount(int totalMonth)
{
    double discount = 0.0;
    int years = 0;

    if (totalMonth <= 24)
    {
        discount = 0.001 * totalMonth;
    }
    else if (totalMonth <= 60)
    {
        years = totalMonth / 12;
        discount = (years * 0.005) + 0.025;
        if (discount > 0.045)
            discount = 0.045;
    }
    else
    {
        discount = 0.075;
    }

    return discount;
}
void	freeClubMember(Customer* pCustomer)
{
    if (pCustomer->pDerivedObject)
        free(pCustomer->pDerivedObject);
    freeCustomer(pCustomer);
}
void printTotalPriceMember(const Customer* pCustomer)
{
    ClubMember* pMember = (ClubMember*)pCustomer->pDerivedObject;
    float price = 0.0;
    price = getTotalPrice(pCustomer->pCart) * (float)(1 - calculateDiscount(pMember->totalMonths));
    printf("Total price for %s is %.2f, after discount of %.2f%%\n", pCustomer->name, price, 100 * calculateDiscount(pMember->totalMonths));
}
