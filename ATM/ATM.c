#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct
{
    char username[100];
    char accountNumber[11];
    char pin[5];
    double balance;
} ATMCard;

int isIDAccount(char *accountNumber)
{
    if (strlen(accountNumber) != 10)
    {
        return 0;
    }

    for (int i = 0; i < 10; i++)
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
    if (strlen(pin) != 4)
    {
        return 0;
    }

    for (int i = 0; i < 4; i++)
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
    FILE *file = fopen("account-number.dat", "a");
    if (file == NULL)
    {
        printf("Khong the mo tep luu thong tin.\n");
        return;
    }
    fprintf(file, "%s %s %s %.2f\n", card.username, card.accountNumber, card.pin, card.balance);
    fclose(file);
}

int loadFromFile(ATMCard *cards, int *count)
{
    FILE *file = fopen("account-number.dat", "r");
    if (file == NULL)
    {
        printf("Khong the mo tep de doc thong tin.\n");
        return 0;
    }
    *count = 0;
    while (fscanf(file, "%s %s %s %lf", cards[*count].username, cards[*count].accountNumber, cards[*count].pin, &cards[*count].balance) != EOF)
    {
        (*count)++;
    }
    fclose(file);
    return 1;
}

void overwriteFile(ATMCard *cards, int count)
{
    FILE *file = fopen("account-number.dat", "w"); // Ghi đè toàn bộ tệp
    if (file == NULL)
    {
        printf("Khong the mo tep de ghi thong tin.\n");
        return;
    }

    // Ghi lại từng tài khoản vào tệp
    for (int i = 0; i < count; i++)
    {
        fprintf(file, "%s %s %s %.2f\n", cards[i].username, cards[i].accountNumber, cards[i].pin, cards[i].balance);
    }
    fclose(file);
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
    }
    else
    {
        printf("So tien khong hop le hoac vuot qua so du.\n");
    }
}

void transferMoney(ATMCard *card)
{
    char targetAccount[11];
    double amount;
    int found = 0;

    ATMCard cards[100];
    int count = 0;

    if (!loadFromFile(cards, &count))
    {
        printf("Khong the tai du lieu tu tep.\n");
        return;
    }

    printf("Nhap so tai khoan nhan tien: ");
    scanf("%s", targetAccount);

    // Tìm tài khoản đích trong danh sách
    for (int i = 0; i < count; i++)
    {
        if (strcmp(cards[i].accountNumber, targetAccount) == 0)
        {
            printf("Nhap so tien muon chuyen: ");
            scanf("%lf", &amount);

            if (amount > 0 && amount <= card->balance)
            {
                // Thực hiện chuyển tiền
                card->balance -= amount;    // Trừ tiền tài khoản của người gửi
                cards[i].balance += amount; // Cộng tiền cho tài khoản đích

                printf("Chuyen tien thanh cong!\n");

                // Cập nhật danh sách tài khoản của người gửi trong mảng
                for (int j = 0; j < count; j++)
                {
                    if (strcmp(cards[j].accountNumber, card->accountNumber) == 0)
                    {
                        cards[j].balance = card->balance;
                        break;
                    }
                }

                // Ghi lại tất cả dữ liệu vào tệp
                overwriteFile(cards, count);
                return;
            }
            else
            {
                printf("So tien khong hop le hoac vuot qua so du.\n");
                return;
            }
        }
    }

    printf("So tai khoan khong ton tai.\n");
}

void changePIN(ATMCard *card)
{
    char newPIN[5];
    printf("Nhap ma PIN moi (4 chu so): ");
    scanf("%4s", newPIN);

    if (isValidPIN(newPIN))
    {
        strcpy(card->pin, newPIN);
        printf("Doi ma PIN thanh cong!\n");
    }
    else
    {
        printf("Ma PIN khong hop le.\n");
    }
}

int login(ATMCard *currentCard)
{
    char accountNumber[11];
    char pin[5];
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

    ATMCard cards[100];
    int count = 0;
    loadFromFile(cards, &count);

    for (int i = 0; i < count; i++)
    {
        if (strcmp(cards[i].accountNumber, accountNumber) == 0 && strcmp(cards[i].pin, pin) == 0)
        {
            *currentCard = cards[i];
            found = 1;
            break;
        }
    }

    if (!found)
    {
        printf("Dang nhap that bai. Vui long kiem tra lai thong tin.\n");
    }
    return found;
}

int main()
{
    int choice, count = 0;
    ATMCard cards[100];
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
                printf("Nhap so tai khoan (10 chu so): ");
                scanf("%s", currentCard.accountNumber);
                printf("Nhap ma PIN (4 chu so): ");
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
                overwriteFile(cards, count);
                break;
            case 2:
                transferMoney(&currentCard);
                overwriteFile(cards, count);
                break;
            case 3:
                changePIN(&currentCard);
                overwriteFile(cards, count);
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
                    overwriteFile(cards, count);
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