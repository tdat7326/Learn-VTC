#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
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
int isValidPIN(char *pin);
int isAccountBalance(double balance);
double calculateVAT(double amount);
void saveToFile(ATMCard card);
void withdrawMoney(ATMCard *card);
void transferMoney(ATMCard *card);
void changePIN(ATMCard *card);
void depositMoney(ATMCard *card);
int login(ATMCard *currentCard);
void checkBalance(ATMCard *card);
void displayMenu(ATMCard *card);
void displayTransactionHistory(ATMCard *card);
void displayAccountInfo(ATMCard *card);

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
    fprintf(file, "Giao dich: %s, So tien: %.2f VND, Tai khoan: %s, Thoi gian: %s",
            type, amount, card->accountNumber, ctime(&now)); // Thêm thời gian giao dịch
    fclose(file);

    // Cập nhật lịch sử giao dịch trong bộ nhớ
    strcpy(transactions[transactionCount].type, type);
    transactions[transactionCount].amount = amount;
    transactions[transactionCount].timestamp = now;
    transactionCount++; // Tăng số lượng giao dịch
}

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
    char choice;

    // Xác nhận in biên lai
    if (!confirmPrintReceipt())
    {
        printf("Khong in bien lai.\n");
        return; // Nếu không in, thoát khỏi hàm
    }
    do
    {
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
        // Hỏi người dùng có muốn thoát không
        printf("Ban co muon giao dich tiep khong? (Y/N): ");
        scanf(" %c", &choice);
    } while (choice != 'n' && choice != 'n'); // Nếu không chọn thoát, tiếp tục vòng lặp
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

int isNumber(char c)
{
    return (c >= '0' && c <= '9'); // Kiểm tra xem có phải là chữ số không
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

void getAccountNumber(char *accountNumber)
{
    int i = 0;
    char ch;

    while (i < 14) // Chỉ cho nhập 14 ký tự
    {
        ch = getchar(); // Đọc từng ký tự từ bàn phím
        if (ch == '\n') // Dừng nếu nhấn Enter
            continue;

        if (isNumber(ch)) // Kiểm tra nếu là số
        {
            accountNumber[i++] = ch; // Ghi ký tự vào accountNumber
            putchar(ch);             // In ký tự ra màn hình
        }
        else
        {
            printf("\nChi duoc nhap so, vui long nhap lai: ");
            while (getchar() != '\n')
                ; // Xóa buffer nếu có ký tự không hợp lệ
        }
    }
    accountNumber[i] = '\0'; // Đảm bảo chuỗi kết thúc đúng
    printf("\n");
}

void getPIN(char *pin)
{
    int i = 0;
    char ch;

    while (i < 6) // Chỉ cho nhập 6 ký tự
    {
        ch = getchar(); // Đọc từng ký tự từ bàn phím
        if (ch == '\n') // Dừng nếu nhấn Enter
            continue;

        if (isNumber(ch)) // Kiểm tra nếu là số
        {
            pin[i++] = ch; // Ghi ký tự vào pin
            putchar(ch);   // In ký tự ra màn hình
        }
        else
        {
            printf("\nChi duoc nhap so, vui long nhap lai: ");
            while (getchar() != '\n')
                ; // Xóa buffer nếu có ký tự không hợp lệ
        }
    }
    pin[i] = '\0'; // Đảm bảo chuỗi kết thúc đúng
    printf("\n");
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

double calculateVAT(double amount)
{
    const double VAT_RATE = 0.005; // Tỷ lệ VAT 1%
    return amount * VAT_RATE;
}

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
    fprintf(file, "%s %s %s %.2f\n", card.username, card.accountNumber, card.pin, card.balance);
    fclose(file);
}

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
        printf("6. Other number\n");
        printf("-----------------------------------------\n");
        printf("Your choice: ");
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
        scanf(" %c", &choiceContinue);
    } while (choiceContinue != 'N' && choiceContinue != 'n'); // Nếu không chọn thoát, tiếp tục vòng lặp
}

void transferMoney(ATMCard *card)
{
    char targetAccount[15];
    double amount;
    ATMCard targetCard;
    char choice;

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
        scanf(" %c", &choice);
    } while (choice != 'N' && choice != 'n'); // Nếu không chọn thoát, tiếp tục vòng lặp
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
    char choice;

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
                printf("Gui tien thanh cong! So du hien tai: %s\n", formatCurrency(card->balance));
            }
        }
        else
        {
            printf("So tien gui khong hop le.\n");
        }
        printf("Ban co muon giao dich them khong? (Y/N): ");
        scanf(" %c", &choice);
    } while (choice != 'N' && choice != 'n'); // Nếu không chọn thoát, tiếp tục vòng lặp
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
    while (1)
    {
        printf("Nhap so tai khoan (14 chu so): ");
        scanf("%s", accountNumber);

        // Kiểm tra xem số tài khoản có phải là số không
        int isValid = 1; // Biến để kiểm tra tính hợp lệ
        for (int i = 0; i < strlen(accountNumber); i++)
        {
            if (accountNumber[i] < '0' || accountNumber[i] > '9')
            {
                isValid = 0; // Nếu có ký tự không phải số, đánh dấu không hợp lệ
                break;
            }
        }

        // Kiểm tra độ dài
        if (isValid && strlen(accountNumber) == 14)
        {
            break; // Nếu hợp lệ, thoát khỏi vòng lặp
        }
        else
        {
            printf("So tai khoan khong hop le. Vui long nhap lai.\n");
        }
    }

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

void checkBalance(ATMCard *card)
{
    printf("\n=========================================\n");
    printf("          VTC Academy Bank\n");
    printf("=========================================\n");
    printf("So du hien tai: %s\n", formatCurrency(card->balance)); // Sử dụng hàm formatCurrency
    printf("-----------------------------------------\n");
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
    do
    {
        char filename[100];
        snprintf(filename, sizeof(filename), "history/%s_transactions.dat", card->accountNumber); // Tạo tên file

        FILE *file = fopen(filename, "r");
        if (file == NULL)
        {
            printf("Khong co lich su giao dich nao.\n");
            return;
        }

        printf("\nLich su giao dich:\n");
        printf("=========================================\n");
        printf("| %-20s | %-15s | %-25s |\n", "Loai giao dich", "So tien", "Thoi gian");
        printf("=========================================\n");

        char line[256];
        while (fgets(line, sizeof(line), file))
        {
            // Phân tích dòng để lấy thông tin
            char type[20];
            double amount;
            char accountNumber[15];
            char timestamp[30];

            sscanf(line, "Giao dich: %[^,], So tien: %lf VND, Tai khoan: %[^,], Thoi gian: %[^\n]", type, &amount, accountNumber, timestamp);

            // In ra theo định dạng chuẩn
            printf("| %-20s | %-15s | %-25s |\n", type, formatCurrency(amount), timestamp);
        }
        printf("=========================================\n");
        fclose(file);

        // Hỏi người dùng có muốn thoát không
        printf("Ban co muon thoat? (Y/N): ");
        scanf(" %c", &choice);
    } while (choice != 'Y' && choice != 'y'); // Nếu không chọn thoát, tiếp tục vòng lặp
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