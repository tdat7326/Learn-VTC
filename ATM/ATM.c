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
    char pin[7];
    ATMCard currentCard;
    int loggedIn = 0;
    char createATM;
    int valid = 0;
    int found = 0;

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
                if (!isValidUsername(currentCard.username))
                {
                    printf("Ten tai khoan khong hop le. Phai co dau cach va tren 10 ky tu.\n");
                    break;
                }

                getAccountNumber(accountNumber, 14);
                strcpy(currentCard.accountNumber, accountNumber); // Sao chép số tài khoản vào currentCard
                getPIN(currentCard.pin, 6);
                printf("\nNhap so tien ban dau: ");
                scanf("%lf", &currentCard.balance);
                printf("-----------------------------------------\n");

                if (!isIDAccount(currentCard.accountNumber))
                {
                    printf("So tai khoan khong hop le.\n");
                }
                else if (!isValidPIN(currentCard.pin))
                {
                    printf("Ma PIN khong hop le.\n");
                }
                else if (!isAccountBalance(currentCard.balance))
                {
                    printf("So du nap vao tai khoan phai > 50.000 VND.\n");
                }

                else
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
                break;

            case 2:
                if (login(&currentCard))
                {
                    printf("Dang nhap thanh cong!\n");
                    loggedIn = 1; // Đặt loggedIn thành 1 khi đăng nhập thành công
                }
                else
                {
                    printf("Dang nhap that bai.\n");
                }
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