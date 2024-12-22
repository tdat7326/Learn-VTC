#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

typedef struct
{
    char username[100];
    char accountNumber[15];
    char pin[7];
    double balance;
} ATMCard;

typedef struct Transaction
{
    char type[20]; // "Chuyen tien", "Gui tien", "Rut tien"
    double amount;
    time_t timestamp;
} Transaction;

#define MAX_TRANSACTIONS 100
Transaction transactions[MAX_TRANSACTIONS];
int transactionCount = 0;

void saveTransactionHistory(ATMCard *card);
void recordTransaction(const char *type, double amount, ATMCard *card);
void displayTransactionHistory(ATMCard *card);
int loadFromFile(ATMCard *card, const char *accountNumber);
void overwriteFile(ATMCard *card);
void printTransactionReceipt(const char *atmID, const char *cardNumber, const char *sourceAccount,
                             const char *targetAccount, double amount, double remainingBalance, const char *transactionID);

void printTransactionReceipt(const char *atmID, const char *cardNumber, const char *sourceAccount,
                             const char *targetAccount, double amount, double remainingBalance, const char *transactionID)
{
    // Lấy thời gian hiện tại
    time_t t;
    time(&t);
    struct tm *timeinfo = localtime(&t);

    // In biên lai giao dịch
    printf("==========================================\n");
    printf("            BIEN LAI GIAO DICH ATM        \n");
    printf("==========================================\n");
    printf("NGAY G/D: %02d/%02d/%d %02d:%02d:%02d\n",
           timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900,
           timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
    printf("ATM ID: %s\n", atmID);
    printf("SO THE: %s\n", cardNumber);
    printf("------------------------------------------\n");
    printf("GIAO DICH CHUYEN KHOAN\n");
    printf("TAI KHOAN CHUYEN TIEN: %s\n", sourceAccount);
    printf("TAI KHOAN NHAN TIEN: %s\n", targetAccount);
    printf("SO TIEN: %.2f VND\n", amount);
    printf("SO TAI KHOAN: %.2f VND\n", remainingBalance);
    printf("PHI: 0.00 VND\n");
    printf("ID GIAO DICH: %s\n", transactionID);
    printf("------------------------------------------\n");
    printf("                *** THANK YOU ***          \n");
}

// Định nghĩa hàm lưu lịch sử giao dịch
void saveTransactionHistory(ATMCard *card)
{
    char filename[30];
    snprintf(filename, sizeof(filename), "%s.dat", card->accountNumber); // Sử dụng file chính để lưu lịch sử

    FILE *file = fopen(filename, "a"); // Mở file với chế độ thêm
    if (file == NULL)
    {
        printf("Khong the mo tep de luu lich su giao dich.\n");
        return;
    }

    // Ghi lại giao dịch vào file chính
    for (int i = 0; i < transactionCount; i++)
    {
        fprintf(file, "Giao dich: %s, So tien: %.2f VND, Thoi gian: %s",
                transactions[i].type,
                transactions[i].amount,
                ctime(&transactions[i].timestamp)); // Thêm thời gian giao dịch
    }
    fclose(file);
}

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

// Định nghĩa hàm ghi lại giao dịch
void recordTransaction(const char *type, double amount, ATMCard *card)
{
    if (transactionCount < MAX_TRANSACTIONS)
    {
        strcpy(transactions[transactionCount].type, type);
        transactions[transactionCount].amount = amount;
        transactions[transactionCount].timestamp = time(NULL);
        transactionCount++;
    }

    // Lưu lịch sử giao dịch vào file khi có giao dịch mới
    saveTransactionHistory(card);
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

void printReceipt(const char *transactionType, double amount, double balance, const char *accountNumber)
{
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char dateTime[100];
    strftime(dateTime, sizeof(dateTime), "%d-%m-%Y %H:%M:%S", t);

    printf("\n=========================================\n");
    printf("             BIEN LAI GIAO DICH\n");
    printf("=========================================\n");
    printf("Thoi gian: %s\n", dateTime);
    printf("So tai khoan: %s\n", accountNumber);
    printf("Loai giao dich: %s\n", transactionType);
    printf("So tien: %.2f VND\n", amount);
    printf("So du hien tai: %.2f VND\n", balance);
    printf("=========================================\n");
    printf("        Cam on quy khach da su dung\n");
    printf("        dich vu cua chung toi!\n");
    printf("=========================================\n");
}

void withdrawMoney(ATMCard *card)
{
    double amount;
    printf("Nhap so tien muon rut: ");
    scanf("%lf", &amount);

    // Kiểm tra số tiền hợp lệ
    if (amount <= 0)
    {
        printf("So tien phai lon hon 0.\n");
        return;
    }

    if (amount > card->balance)
    {
        printf("So tien rut vuot qua so du hien tai.\n");
        return;
    }

    if (amount < 50000)
    { // Điều kiện giả định số tiền rút tối thiểu là 50.000 VND
        printf("So tien rut toi thieu la 50.000 VND.\n");
        return;
    }

    // Trừ tiền trong tài khoản
    card->balance -= amount;

    // Ghi lại giao dịch
    printf("Rut tien thanh cong! So du con lai: %.2f VND\n", card->balance);
    recordTransaction("Rut tien", amount, card);
    overwriteFile(card);

    // In biên lai giao dịch
    printReceipt("Rut tien", amount, card->balance, card->accountNumber);
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
        recordTransaction("Chuyen tien", amount, card);
        recordTransaction("Nhan tien", amount, &targetCard);
        overwriteFile(card);
        overwriteFile(&targetCard);
        printf("So du con lai: %.2f VND\n", card->balance);
        printReceipt("Chuyen tien", amount, card->balance, card->accountNumber); // In biên lai
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

void depositMoney(ATMCard *card)
{
    double depositAmount;
    printf("Nhap so tien muon gui: ");
    scanf("%lf", &depositAmount);

    if (depositAmount > 0)
    {
        card->balance += depositAmount;
        printf("Gui tien thanh cong! So du hien tai: %.2f VND\n", card->balance);
        recordTransaction("Gui tien", depositAmount, card);
        overwriteFile(card);
        printReceipt("Gui tien", depositAmount, card->balance, card->accountNumber); // In biên lai
    }
    else
    {
        printf("So tien gui khong hop le.\n");
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
    // Nhập PIN và hiển thị dấu *
    for (int i = 0; i < 6; i++)
    {
        pin[i] = getch();   // Đọc ký tự mà không hiển thị
        if (pin[i] == '\r') // Nếu nhấn Enter, thoát vòng lặp
            break;
        printf("*"); // Hiển thị dấu *
    }
    pin[6] = '\0'; // Kết thúc chuỗi
    printf("\n");

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

void displayTransactionHistory(ATMCard *card)
{
    char filename[30];
    snprintf(filename, sizeof(filename), "%s_transactions.dat", card->accountNumber);

    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Khong co lich su giao dich nao.\n");
        return;
    }

    printf("\nLich su giao dich:\n");
    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        // Chỉ in ra giao dịch "Nhan tien" cho tài khoản nhận tiền
        if (strstr(line, "Nhan tien") != NULL)
        {
            printf("%s", line);
        }
    }
    fclose(file);
}

void displayAccountInfo(ATMCard *card)
{
    printf("\n=========================================\n");
    printf("          VTC Academy Bank\n");
    printf("=========================================\n");
    printf("            ATM Machine\n");
    printf("-----------------------------------------\n");
    printf("Account No:   %s\n", card->accountNumber);
    printf("Pin Code:     ******\n");
    printf("-----------------------------------------\n");
    printf("Account Name: %s\n", card->username);
    printf("-----------------------------------------\n");
}

int main()
{
    int choice;
    ATMCard currentCard;
    int loggedIn = 0;
    char createATM;
    const char *atmID = "12700005";
    const char *cardNumber = "1234XXXXXXXX5678"; // Chỉ hiển thị một phần số thẻ để bảo mật
    const char *sourceAccount = "12345678901234";
    const char *targetAccount = "100001945349";
    double amount = 340000.00;
    double remainingBalance = 126078900.00;
    const char *transactionID = "TX123456789";
    printTransactionReceipt(atmID, cardNumber, sourceAccount, targetAccount, amount, remainingBalance, transactionID);

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
                printf("=========================================\n");
                printf("          VTC Academy Bank\n");
                printf("=========================================\n");
                printf("\n          Create ATM Cards\n");
                printf("\n-----------------------------------------\n");

                printf("Nhap ho va ten: ");
                scanf(" %[^\n]", currentCard.username);
                printf("Nhap so tai khoan(14 so): ");
                scanf("%s", currentCard.accountNumber);
                printf("Nhap so pin(6 so): ");
                scanf("%s", currentCard.pin);
                printf("Nhap so tien ban dau: ");
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
                if (loggedIn)
                {
                    displayAccountInfo(&currentCard); // Hiển thị thông tin tài khoản sau khi đăng nhập thành công
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
            printf("1. Rut tien\n");
            printf("2. Chuyen tien\n");
            printf("3. Doi ma PIN\n");
            printf("4. Gui tien\n");
            printf("5. Kiem tra so du\n");
            printf("6. Lich su giao dich\n");
            printf("7. Dang xuat\n");
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
                printf("\n=========================================\n");
                printf("          VTC Academy Bank\n");
                printf("=========================================\n");
                printf("So du hien tai: %.2f VND\n", currentCard.balance);
                printf("-----------------------------------------\n");
                break;
            case 6:
                displayTransactionHistory(&currentCard);
                break;
            case 7:
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