#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include "Library/currency.h"

int main()
{
    char accountNumber[15];
    int choice;
    ATMCard currentCard;
    int loggedIn = 0;
    char createATM;
    int valid = 0;

    while (1)
    {
        if (!loggedIn)
        {
            printf("===========================\n");
            printf("VTC Academy Bank\n");
            printf("===========================\n");
            printf("1. Tao tai khoan moi\n");
            printf("2. Dang nhap\n");
            printf("3. Thoat\n");
            printf("Nhap lua chon cua ban: ");
            scanf("%d", &choice);

            switch (choice)
            {
            case 1:
                printf("\n=========================================\n");
                printf("          VTC Academy Bank\n");
                printf("=========================================\n");
                printf("\n          Create ATM Cards\n");
                printf("\n-----------------------------------------\n");

                printf("Nhap ho va ten: ");
                scanf(" %[^\n]", currentCard.username);
                printf("\nNhap so tai khoan(14 so): ");
                getAccountNumber(accountNumber);
                valid = isIDAccount(accountNumber);
                if (valid)
                {
                    printf("\nSo tai khoan hop le.\n");
                }

                printf("\nNhap so pin(6 so): ");
                scanf("%s", currentCard.pin);
                printf("\nNhap so tien ban dau: ");
                scanf("%lf", &currentCard.balance);
                printf("-----------------------------------------\n");

                if (isIDAccount(currentCard.accountNumber) && isValidPIN(currentCard.pin) && isAccountBalance(currentCard.balance))
                {
                    printf("Ban co muon tao The ATM khong? (Y/N): ");
                    scanf(" %c", &createATM);
                    if (createATM == 'Y' || createATM == 'y')
                    {
                        saveToFile(currentCard);
                        printf("Tao tai khoan thanh cong!\n");
                    }
                    else
                    {
                        printf("Khong tao tai khoan.\n");
                    }
                }
                else
                {
                    printf("Thong tin khong hop le.\n");
                }
                break;

            case 2:
                loggedIn = login(&currentCard);
                break;

            case 3:
                printf("Thoat chuong trinh. Hen gap lai!\n");
                exit(0);

            default:
                printf("Lua chon khong hop le. Vui long nhap lai!\n");
            }
        }
        else
        {
            displayMenu(&currentCard);
            scanf("%d", &choice);

            switch (choice)
            {
            case 1:
                withdrawMoney(&currentCard);
                break;
            case 2:
                transferMoney(&currentCard);
                break;
            case 3:
                changePIN(&currentCard);
                break;
            case 4:
                printf("===========================\n");
                printf("VTC Academy Bank\n");
                printf("===========================\n");
                printf("Nhap so tien muon gui: ");
                double depositAmount;
                scanf("%lf", &depositAmount);

                if (depositAmount > 0)
                {
                    currentCard.balance += depositAmount;
                    printf("Gui tien thanh cong! So du hien tai: %.2f VND\n", currentCard.balance);
                    overwriteFile(&currentCard);
                }
                else
                {
                    printf("So tien gui khong hop le.\n");
                }
                break;
            case 5:
                displayTransactionHistory(&currentCard);
                break;
            case 6:
                loggedIn = 0;
                printf("Dang xuat thanh cong!\n");
                break;
            default:
                printf("Lua chon khong hop le. Vui long nhap lai!\n");
            }
        }
    }
    return 0;
}