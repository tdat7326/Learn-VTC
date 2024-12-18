#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct
{
    char username[100];
    char accountNumber[15];
    char pin[7];
    double balance;
} ATMCard;

int isIDAccount(char *accountNumber)
{
    if (strlen(accountNumber) != 14)
    {
        return 0;
    }

    for (int i = 0; i < 14; i++)
    {
        if (accountNumber[i] < '0' || accountNumber[i] > '9')
        {
            return 0;
        }
    }
    return 1;
}

int isValidPIN(char *pin)
{
    if (strlen(pin) != 6)
    {
        return 0;
    }

    for (int i = 0; i < 6; i++)
    {
        if (pin[i] < '0' || pin[i] > '9')
        {
            return 0;
        }
    }
    return 1;
}

int isAccountBalance(double balance)
{
    return balance >= 50000;
}

void saveToFile(ATMCard card)
{
    char filename[30];
    snprintf(filename, sizeof(filename), "%s.dat", card.accountNumber);

    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Khong the mo tep de luu thong tin.\n");
        return;
    }
    fprintf(file, "%s %s %s %.2f\n", card.username, card.accountNumber, card.pin, card.balance);
    fclose(file);
    printf("Da tao file cho tai khoan: %s\n", filename);
}

int loadFromFile(ATMCard *card, const char *accountNumber)
{
    char filename[30];
    snprintf(filename, sizeof(filename), "%s.dat", accountNumber);

    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Khong the mo tep de doc thong tin.\n");
        return 0;
    }

    // Đọc thông tin từ file
    if (fscanf(file, "%99s %14s %6s %lf", card->username, card->accountNumber, card->pin, &card->balance) != 4)
    {
        printf("Khong the doc du lieu tu tep.\n");
        fclose(file);
        return 0;
    }
    fclose(file);
    return 1;
}

void overwriteFile(ATMCard *card)
{
    saveToFile(*card);
}

void withdrawMoney(ATMCard *card)
{
    double amount;
    printf("Nhap so tien muon rut: ");
    scanf("%lf", &amount);

    if (amount > 0 && amount <= card->balance)
    {
        card->balance -= amount;
        printf("Rut tien thanh cong! So du con lai: %.2f VND\n", card->balance);
        overwriteFile(card);
    }
    else
    {
        printf("So tien khong hop le hoac vuot qua so du.\n");
    }
}

void transferMoney(ATMCard *card)
{
    char targetAccount[15];
    double amount;
    ATMCard targetCard;

    printf("Nhap so tai khoan nhan tien: ");
    scanf("%s", targetAccount);

    if (!loadFromFile(&targetCard, targetAccount))
    {
        printf("So tai khoan khong ton tai.\n");
        return;
    }

    printf("Nhap so tien muon chuyen: ");
    scanf("%lf", &amount);

    if (amount > 0 && amount <= card->balance)
    {
        card->balance -= amount;
        targetCard.balance += amount;
        printf("Chuyen tien thanh cong!\n");

        // Cập nhật file cho cả hai tài khoản
        overwriteFile(card);
        overwriteFile(&targetCard);
    }
    else
    {
        printf("So tien khong hop le hoac vuot qua so du.\n");
    }
}

void changePIN(ATMCard *card)
{
    char newPIN[7];
    printf("Nhap ma PIN moi (6 chu so): ");
    scanf("%6s", newPIN);

    if (isValidPIN(newPIN))
    {
        strcpy(card->pin, newPIN);
        printf("Doi ma PIN thanh cong!\n");
        overwriteFile(card);
    }
    else
    {
        printf("Ma PIN khong hop le.\n");
    }
}

int login(ATMCard *currentCard)
{
    char accountNumber[15];
    char pin[7];
    int found = 0;

    printf("===========================\n");
    printf("    VTC Academy Bank\n");
    printf("===========================\n");
    printf("        DANG NHAP      \n");
    printf("===========================\n");
    printf("Nhap so tai khoan: ");
    scanf("%s", accountNumber);
    printf("Nhap ma PIN: ");
    scanf("%s", pin);

    // Tải thông tin tài khoản từ file
    if (!loadFromFile(currentCard, accountNumber))
    {
        printf("Khong the tai du lieu tu tep.\n");
        return 0;
    }

    // Kiểm tra thông tin đăng nhập
    if (strcmp(currentCard->accountNumber, accountNumber) == 0 && strcmp(currentCard->pin, pin) == 0)
    {
        found = 1; // Đăng nhập thành công
    }

    if (!found)
    {
        printf("Dang nhap that bai. Vui long kiem tra lai thong tin.\n");
    }
    return found;
}

int main()
{
    int choice;
    ATMCard currentCard;
    int loggedIn = 0;

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
                printf("===========================\n");
                printf("VTC Academy Bank\n");
                printf("===========================\n");
                printf("Nhap ho va ten: ");
                scanf(" %[^\n]", currentCard.username);
                printf("Nhap so tai khoan (14 chu so): ");
                scanf("%s", currentCard.accountNumber);
                printf("Nhap ma PIN (6 chu so): ");
                scanf("%s", currentCard.pin);
                printf("Nhap so tien ban dau: ");
                scanf("%lf", &currentCard.balance);

                if (isIDAccount(currentCard.accountNumber) && isValidPIN(currentCard.pin) && isAccountBalance(currentCard.balance))
                {
                    saveToFile(currentCard);
                    printf("Tao tai khoan thanh cong!\n");
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
            printf("===========================\n");
            printf("VTC Academy Bank\n");
            printf("===========================\n");
            printf("1. Rut tien\n");
            printf("2. Chuyen tien\n");
            printf("3. Doi ma PIN\n");
            printf("4. Gui tien\n");
            printf("5. Dang xuat\n");
            printf("Nhap lua chon cua ban: ");
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