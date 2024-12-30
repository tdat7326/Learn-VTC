#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <direct.h>
#include <time.h>
#include <sys/stat.h>

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
void recordTransaction(const char *type, double amount, ATMCard *card, const char *targetAccount);
void displayTransactionHistory(ATMCard *card);
int loadFromFile(ATMCard *card, const char *accountNumber);
void overwriteFile(ATMCard *card);
int confirmPrintReceipt();
char *formatCurrency(double amount);
void printTransactionReceipt(const char *atmID, const char *cardNumber, const char *sourceAccount,
                             const char *targetAccount, double amount, double remainingBalance, const char *transactionID, const char *vatAccountNumber, double vatAmount, const char *type);
int isIDAccount(char *accountNumber);
void getAccountNumber(char *accountNumber, int maxLength);
int isValidUsername(char *username);
int isAccountBalance(double balance);
void saveToFile(ATMCard card);
void withdrawMoney(ATMCard *card);
void transferMoney(ATMCard *card);
void changePIN(ATMCard *card);
void depositMoney(ATMCard *card);
int login(ATMCard *currentCard);
void displayMenu(ATMCard *card);
void displayTransactionHistory(ATMCard *card);
void displayAccountInfo(ATMCard *card);
void waitForKeyPress();
void waitForKeyPress()
{
    printf("\nBam phim bat ky de quay lai...\n");
    getch(); // Chờ người dùng bấm phím bất kỳ
}

void recordTransaction(const char *type, double amount, ATMCard *card, const char *targetAccount)
{
    // Tạo thư mục history nếu chưa tồn tại
    mkdir("history"); // Tạo thư mục với quyền truy cập đầy đủ

    // Tạo tên file dựa trên số tài khoản trong thư mục history
    char filename[100];
    snprintf(filename, sizeof(filename), "history/%s_transactions.dat", card->accountNumber);

    // Mở file với chế độ thêm
    FILE *file = fopen(filename, "a");
    if (file == NULL)
    {
        printf("Khong the mo tep de luu lich su giao dich.\n");
        return;
    }

    // Ghi lại giao dịch vào file
    time_t now;
    time(&now);
    if (targetAccount != NULL)
    {
        fprintf(file, "Giao dich: %s, So tien: %.2f VND, Tai khoan: %s, Thoi gian: %s",
                type, amount, targetAccount, ctime(&now)); // Thêm thời gian giao dịch và tài khoản đích
    }
    else
    {
        fprintf(file, "Giao dich: %s, So tien: %.2f VND, Tai khoan: %s, Thoi gian: %s",
                type, amount, card->accountNumber, ctime(&now)); // Thêm thời gian giao dịch
    }
    fclose(file);

    // Cập nhật lịch sử giao dịch trong bộ nhớ
    strcpy(transactions[transactionCount].type, type);
    transactions[transactionCount].amount = amount;
    transactions[transactionCount].timestamp = now;
    transactionCount++; // Tăng số lượng giao dịch
}

// Hàm xác nhận in biên lai
int confirmPrintReceipt()
{
    char choice;
    printf("Ban co muon in bien lai? (Y/N): ");
    scanf(" %c", &choice); // Dấu cách trước %c để bỏ qua ký tự trắng

    if (choice == 'Y' || choice == 'y')
    {
        return 1; // Trả về 1 nếu chọn in
    }
    else
    {
        return 0; // Trả về 0 nếu không in
    }
}

char *formatCurrency(double amount)
{
    static char buffer[50];                            // Đảm bảo đủ không gian cho chuỗi
    int intPart = (int)amount;                         // Phần nguyên
    int decimalPart = (int)((amount - intPart) * 100); // Phần thập phân (2 chữ số)

    // Định dạng phần nguyên với dấu phân cách
    char temp[50];
    snprintf(temp, sizeof(temp), "%d", intPart);
    int len = strlen(temp);
    int j = 0;
    int k = 0;

    // Thêm dấu phân cách hàng nghìn
    for (int i = len - 1; i >= 0; i--)
    {
        if (k > 0 && k % 3 == 0)
        {
            buffer[j++] = '.'; // Thêm dấu phân cách
        }
        buffer[j++] = temp[i];
        k++;
    }
    buffer[j] = '\0';

    // Đảo ngược chuỗi để có định dạng đúng
    for (int i = 0; i < j / 2; i++)
    {
        char swap = buffer[i];
        buffer[i] = buffer[j - i - 1];
        buffer[j - i - 1] = swap;
    }

    // Thêm phần thập phân nếu có
    if (decimalPart > 0)
    {
        snprintf(temp, sizeof(temp), ",%02d", decimalPart); // Sử dụng dấu phẩy cho phần thập phân
        strcat(buffer, temp);
    }

    // Thêm đơn vị VNĐ
    strcat(buffer, " VND");
    return buffer;
}

void printTransactionReceipt(const char *atmID, const char *cardNumber, const char *sourceAccount,
                             const char *targetAccount, double amount, double remainingBalance, const char *transactionID, const char *vatAccountNumber, double vatAmount, const char *type)
{
    // Lấy thời gian hiện tại
    time_t t;
    time(&t);
    struct tm *timeinfo = localtime(&t);

    // Xác nhận in biên lai
    if (!confirmPrintReceipt())
    {
        printf("Khong in bien lai.\n");
        return; // Nếu không in, thoát khỏi hàm
    }

    if (strcmp(type, "Chuyen tien") == 0) // Kiểm tra loại giao dịch
    {
        // In biên lai giao dịch
        printf("==========================================\n");
        printf("            BIEN LAI GIAO DICH ATM        \n");
        printf("==========================================\n");
        printf("NGAY G/D: %02d/%02d/%d %02d:%02d:%02d\n",
               timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900,
               timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
        printf("SO THE: %s\n", cardNumber);
        printf("------------------------------------------\n");
        printf("      GIAO DICH CHUYEN KHOAN\n");
        printf("Tai khoan chuyen tien: %s\n", sourceAccount);
        printf("Tai khoan nhan tien: %s\n", targetAccount);
        printf("So tien: %s\n", formatCurrency(amount));                   // Sử dụng hàm formatCurrency
        printf("So du tai khoan: %s\n", formatCurrency(remainingBalance)); // Sử dụng hàm formatCurrency
        printf("ID GIAO DICH: %s\n", transactionID);
        printf("------------------------------------------\n");
        printf("               *** THANK YOU ***          \n");
    }
    else if (strcmp(type, "Rut tien") == 0) // Kiểm tra loại giao dịch
    {
        // In biên lai RUT TIEN
        printf("==========================================\n");
        printf("            BIEN LAI GIAO DICH ATM        \n");
        printf("==========================================\n");
        printf("NGAY G/D: %02d/%02d/%d %02d:%02d:%02d\n",
               timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900,
               timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
        printf("SO THE: %s\n", cardNumber);
        printf("------------------------------------------\n");
        printf("      GIAO DICH RUT TIEN\n");
        printf("So tien: %s\n", formatCurrency(amount));                   // Sử dụng hàm formatCurrency
        printf("So du tai khoan: %s\n", formatCurrency(remainingBalance)); // Sử dụng hàm formatCurrency
        printf("ID GIAO DICH: %s\n", transactionID);
        printf("------------------------------------------\n");
        printf("               *** THANK YOU ***          \n");
    }
    else if (strcmp(type, "Gui tien") == 0) // Kiểm tra loại giao dịch
    {
        // In biên lai gui tien
        printf("==========================================\n");
        printf("            BIEN LAI GIAO DICH ATM        \n");
        printf("==========================================\n");
        printf("NGAY G/D: %02d/%02d/%d %02d:%02d:%02d\n",
               timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900,
               timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
        printf("SO THE: %s\n", cardNumber);
        printf("------------------------------------------\n");
        printf("      GIAO DICH GUI TIEN\n");
        printf("So tien gui: %s\n", formatCurrency(amount));               // Sử dụng hàm formatCurrency
        printf("So du tai khoan: %s\n", formatCurrency(remainingBalance)); // Sử dụng hàm formatCurrency
        printf("ID GIAO DICH: %s\n", transactionID);
        printf("------------------------------------------\n");
        printf("               *** THANK YOU ***          \n");
    }
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

// Hàm kiểm tra ký tự có phải là số không
int isNumber(char c)
{
    return (c >= '0' && c <= '9');
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
            return 0; // Chỉ cho phép chữ cái
        }
    }
    return 1;
}

// Hàm nhập số tài khoản với độ dài giới hạn
void getAccountNumber(char *accountNumber, int maxLength)
{
    int i = 0;
    char ch;

    while (1)
    {
        i = 0;
        printf("Nhap so tai khoan (14 chu so): ");
        while (i < maxLength)
        {
            ch = getch();   // Đọc từng ký tự từ bàn phím mà không hiển thị
            if (ch == '\r') // Dừng nếu nhấn Enter
                break;

            if (isNumber(ch)) // Kiểm tra nếu là số
            {
                accountNumber[i++] = ch; // Ghi ký tự vào accountNumber
                putchar(ch);             // In ký tự ra màn hình
            }
        }
        accountNumber[i] = '\0'; // Đảm bảo chuỗi kết thúc đúng
        printf("\n");

        // Kiểm tra độ dài của số tài khoản
        if (i == 14)
        {
            break; // Thoát khỏi vòng lặp nếu nhập đúng 14 chữ số
        }
        else
        {
            printf("So tai khoan phai co dung 14 chu so. Vui long nhap lai.\n");
        }
    }
    accountNumber[i] = '\0'; // Đảm bảo chuỗi kết thúc đúng
    printf("\n");
}

// Hàm nhập mã PIN với độ dài giới hạn
void getPIN(char *pin, int maxLength)
{
    int i = 0;
    char ch;

    while (1)
    {
        i = 0;
        printf("Nhap ma PIN (6 chu so): ");
        while (i < maxLength)
        {
            ch = getch();   // Đọc từng ký tự từ bàn phím mà không hiển thị
            if (ch == '\r') // Dừng nếu nhấn Enter
                break;

            if (isNumber(ch)) // Kiểm tra nếu là số
            {
                pin[i++] = ch; // Ghi ký tự vào pin
                putchar('*');  // Hiển thị dấu *
            }
        }
        pin[i] = '\0'; // Đảm bảo chuỗi kết thúc đúng
        printf("\n");

        // Kiểm tra độ dài của mã PIN
        if (i == 6)
        {
            break; // Thoát khỏi vòng lặp nếu nhập đúng 6 chữ số
        }
        else
        {
            printf("Ma PIN phai co dung 6 chu so. Vui long nhap lai.\n");
        }
    }
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

// Hàm kiểm tra tên tài khoản hợp lệ
int isValidUsername(char *username)
{
    if (strlen(username) <= 10)
    {
        return 0;
    }

    int hasSpace = 0;
    for (int i = 0; i < strlen(username); i++)
    {
        if (username[i] == ' ')
        {
            hasSpace = 1;
            break;
        }
    }

    return hasSpace;
}

int isAccountBalance(double balance)
{
    return balance >= 50000;
}

double calculateVAT(double amount)
{
    const double VAT_RATE = 0.005; // Tỷ lệ VAT 1%
    return amount * VAT_RATE;
}

// Hàm lưu thông tin tài khoản vào file
void saveToFile(ATMCard card)
{
    char filename[30];
    snprintf(filename, sizeof(filename), "accountATM/%s.dat", card.accountNumber);

    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Khong the mo tep de luu thong tin.\n");
        return;
    }
    fprintf(file, "%s\n%s\n%s\n%.2f\n", card.username, card.accountNumber, card.pin, card.balance);
    fclose(file);
}

// Hàm đọc thông tin tài khoản từ file
int loadFromFile(ATMCard *card, const char *accountNumber)
{
    char filename[30];
    snprintf(filename, sizeof(filename), "accountATM/%s.dat", accountNumber);

    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Khong the mo tep de doc thong tin.\n");
        return 0;
    }

    // Đọc thông tin từ file
    if (fscanf(file, "%99[^\n]\n%14s\n%6s\n%lf", card->username, card->accountNumber, card->pin, &card->balance) != 4)
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
    int choice;
    double amount = 0;
    char choiceContinue;

    do
    {
        // Hiển thị menu rút tiền
        printf("=========================================\n");
        printf("          VTC Academy Bank\n");
        printf("ATM Machine - Withdraw\n");
        printf("=========================================\n");
        printf("1. 100.000 VND\n");
        printf("2. 200.000 VND\n");
        printf("3. 500.000 VND\n");
        printf("4. 1.000.000 VND\n");
        printf("5. 2.000.000 VND\n");
        printf("6. So tien khac\n");
        printf("-----------------------------------------\n");
        printf("Nhap lua chon: ");
        scanf("%d", &choice);

        // Xác định số tiền dựa trên lựa chọn
        switch (choice)
        {
        case 1:
            amount = 100000;
            break;
        case 2:
            amount = 200000;
            break;
        case 3:
            amount = 500000;
            break;
        case 4:
            amount = 1000000;
            break;
        case 5:
            amount = 2000000;
            break;
        case 6:
            printf("Nhap so tien muon rut: ");
            scanf("%lf", &amount);
            break;
        default:
            printf("Lua chon khong hop le. Vui long chon lai.\n");
            continue; // Quay lại vòng lặp nếu lựa chọn không hợp lệ
        }

        // Kiểm tra số tiền hợp lệ
        if (amount <= 0)
        {
            printf("So tien phai lon hon 0.\n");
            continue;
        }

        if (amount > card->balance)
        {
            printf("So tien rut vuot qua so du hien tai.\n");
            continue;
        }

        if (amount < 50000)
        {
            printf("So tien rut toi thieu la 50.000 VND.\n");
            continue;
        }

        // Trừ tiền trong tài khoản
        card->balance -= amount;

        // Ghi lại giao dịch
        recordTransaction("Rut tien", amount, card, NULL);
        overwriteFile(card);

        // Hỏi người dùng có muốn in biên lai không
        if (confirmPrintReceipt())
        {
            // In biên lai giao dịch
            printTransactionReceipt("ATM_ID", card->accountNumber, card->accountNumber, NULL, amount, card->balance, "TX_ID", "VAT_ACCOUNT", 0, "Rut tien");
        }
        else
        {
            printf("Khong in bien lai.\n");
            // Thông báo giao dịch thành công
            printf("Rut tien thanh cong!\n So du con lai: %s\n", formatCurrency(card->balance));
        }

        // Hỏi người dùng có muốn giao dịch tiếp không
        printf("Ban co muon giao dich tiep khong? (Y/N): ");
    } while (choiceContinue != 'N' && choiceContinue != 'n'); // Nếu không chọn thoát, tiếp tục vòng lặp
}

void transferMoney(ATMCard *card)
{
    char targetAccount[15];
    double amount;
    ATMCard targetCard;
    char choice;
    char choiceContinue;

    do
    {
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
            recordTransaction("Chuyen tien", amount, card, targetAccount);            // Ghi lại giao dịch chuyển tiền
            recordTransaction("Nhan tien", amount, &targetCard, card->accountNumber); // Ghi lại giao dịch nhận tiền
            overwriteFile(card);
            overwriteFile(&targetCard);

            // Tính phí VAT
            double vatAmount = calculateVAT(amount);

            // In biên lai giao dịch
            const char *vatAccountNumber = "123456789"; // Số tài khoản nhận phí VAT
            if (confirmPrintReceipt())
            {
                printTransactionReceipt("12700005", card->accountNumber, card->accountNumber, targetAccount, amount, card->balance, "TX123456789", vatAccountNumber, vatAmount, "Chuyen tien");
            }
            else
            {
                printf("Khong in bien lai.\n");
                // Thông báo giao dịch thành công
                printf("Chuyen tien thanh cong! So du con lai: %s\n", formatCurrency(card->balance));
            }
        }
        else
        {
            printf("So tien khong hop le hoac vuot qua so du.\n");
        }
        // Hỏi người dùng có muốn thoát không
        printf("Ban co muon giao dich them khong? (Y/N): ");
        while ((choiceContinue = getchar()) == '\n' || choiceContinue == ' ')
            ; // Bỏ qua các ký tự trắng và newline
    } while (choiceContinue != 'N' && choiceContinue != 'n'); // Nếu không chọn thoát, tiếp tục vòng lặp
}

void changePIN(ATMCard *card)
{
    char newPIN[7];
    char confirmPIN[7];
    int i = 0;
    char ch;

    // Function to get PIN input
    void getPINInput(char *pin)
    {
        int i = 0;
        char ch;
        while (i < 6)
        {
            ch = getch();   // Đọc từng ký tự từ bàn phím mà không hiển thị
            if (ch == '\r') // Dừng nếu nhấn Enter
                break;

            if (isNumber(ch)) // Kiểm tra nếu là số
            {
                pin[i++] = ch; // Ghi ký tự vào pin
                putchar('*');  // Hiển thị dấu *
            }
        }
        pin[i] = '\0'; // Đảm bảo chuỗi kết thúc đúng
        printf("\n");
    }

    printf("Nhap ma PIN moi (6 chu so): ");
    getPINInput(newPIN);

    printf("Xac nhan ma PIN moi: ");
    getPINInput(confirmPIN);

    if (strcmp(newPIN, confirmPIN) == 0)
    {
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
    else
    {
        printf("Ma PIN khong khop. Vui long thu lai.\n");
    }
}

void depositMoney(ATMCard *card)
{
    double depositAmount;
    char choice;
    char choiceContinue;

    do
    {
        printf("Nhap so tien muon gui: ");
        scanf("%lf", &depositAmount);

        if (depositAmount > 0)
        {
            card->balance += depositAmount;
            recordTransaction("Gui tien", depositAmount, card, NULL); // Ghi lại giao dịch gửi tiền
            overwriteFile(card);

            // Tính phí VAT
            double vatAmount = calculateVAT(depositAmount);

            // In biên lai giao dịch
            const char *vatAccountNumber = "123456789"; // Số tài khoản nhận phí VAT
            if (confirmPrintReceipt())
            {
                printTransactionReceipt("12700005", card->accountNumber, card->accountNumber, card->accountNumber, depositAmount, card->balance, "TX123456789", vatAccountNumber, vatAmount, "Gui tien");
            }
            else
            {
                printf("Khong in bien lai.\n");
                // Thông báo giao dịch thành công
                printf("Gui tien thanh cong!\n So du hien tai: %s\n", formatCurrency(card->balance));
                waitForKeyPress();
            }
        }
        else
        {
            printf("So tien gui khong hop le.\n");
        }
        printf("Ban co muon giao dich them khong? (Y/N): ");
    } while (choiceContinue != 'N' && choiceContinue != 'n'); // Nếu không chọn thoát, tiếp tục vòng lặp
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

    // Nhập số tài khoản và kiểm tra
    getAccountNumber(accountNumber, 14);
    int valid = isIDAccount(accountNumber);
    if (!valid)
    {
        printf("So tai khoan khong hop le.\n");
        return 0;
    }
    getPIN(pin, 6);

    // Tải thông tin tài khoản từ file
    if (!loadFromFile(currentCard, accountNumber))
    {
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

void displayMenu(ATMCard *card)
{
    printf("\n=========================================\n");
    printf("          VTC Academy Bank\n");
    printf("=========================================\n");
    printf("            ATM Machine\n");
    printf("-----------------------------------------\n");
    printf("So tai khoan: %s\n", card->accountNumber);
    printf("Ma Pin: ******\n");
    printf("So du: %s\n", formatCurrency(card->balance));
    printf("-----------------------------------------\n");
    printf("Ten tai khoan: %s\n", card->username);
    printf("-----------------------------------------\n");
    printf("1. Rut tien\n");
    printf("2. Chuyen tien\n");
    printf("3. Doi ma PIN\n");
    printf("4. Gui tien\n");
    printf("5. Lich su giao dich\n");
    printf("6. Dang xuat\n");
    printf("Nhap lua chon cua ban: ");
}

void displayTransactionHistory(ATMCard *card)
{
    char choice;
    // Tạo tên file dựa trên số tài khoản trong thư mục history
    char filename[100];
    snprintf(filename, sizeof(filename), "history/%s_transactions.dat", card->accountNumber); // Tạo tên file

    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Khong co lich su giao dich nao.\n");
        waitForKeyPress();
        return;
    }

    printf("\nLich su giao dich:\n");
    printf("========================================================================================\n");
    printf("| %-20s | %-15s | %-25s | %-15s |\n", "Loai giao dich", "So tien", "Thoi gian", "Tai khoan dich");
    printf("========================================================================================\n");

    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        // Phân tích dòng để lấy thông tin
        char type[20];
        double amount;
        char targetAccount[15];
        char timestamp[30];

        sscanf(line, "Giao dich: %[^,], So tien: %lf VND, Tai khoan: %[^,], Thoi gian: %[^\n]", type, &amount, targetAccount, timestamp);

        // In ra theo định dạng chuẩn
        printf("| %-20s | %-15s | %-25s | %-15s |\n", type, formatCurrency(amount), timestamp, targetAccount);
    }
    printf("========================================================================================\n");
    waitForKeyPress();
    fclose(file);
}

void displayAccountInfo(ATMCard *card)
{
    printf("\n=========================================\n");
    printf("          VTC Academy Bank\n");
    printf("=========================================\n");
    printf("            ATM Machine\n");
    printf("-----------------------------------------\n");
    printf("So tai khoan:   %s\n", card->accountNumber);
    printf("Ma Pin:     ******\n");
    printf("-----------------------------------------\n");
    printf("Ten tai khoan: %s\n", card->username);
    printf("-----------------------------------------\n");
}

#ifndef CURRENCY_H
#define CURRENCY_H

char *
formatCurrency(double amount);

#endif // CURRENCY_H