#include <stdio.h>
#include <string.h>

struct Product
{
    char name[50];
    int quantity;
    float price;
};

// Hàm xóa bộ đệm nhập liệu
void clearInputBuffer()
{
    while (getchar() != '\n')
        ;
}

// Hàm sắp xếp theo tên A-Z
void sortByName(struct Product products[], int n)
{
    struct Product temp;
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if (strcmp(products[i].name, products[j].name) > 0)
            {
                temp = products[i];
                products[i] = products[j];
                products[j] = temp;
            }
        }
    }
}

// Hàm sắp xếp theo giá tăng dần
void sortByPrice(struct Product products[], int n)
{
    struct Product temp;
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if (products[i].price > products[j].price)
            {
                temp = products[i];
                products[i] = products[j];
                products[j] = temp;
            }
        }
    }
}

// Hàm hiển thị danh sách
void displayProducts(struct Product products[], int n)
{
    printf("\nDANH MUC SAN PHAM\n");
    printf("------------------\n");
    printf("STT | Ten san pham      | So luong | Don gia  | Tong tien\n");
    for (int i = 0; i < n; i++)
    {
        float total = products[i].quantity * products[i].price;
        printf("%-4d| %-18s| %-9d| %-9.2f| %-10.2f\n",
               i + 1, products[i].name, products[i].quantity, products[i].price, total);
    }
}

// Hàm nhập thông tin sản phẩm
void inputProducts(struct Product products[], int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("Nhap thong tin san pham thu %d:\n", i + 1);
        printf("Ten san pham: ");
        clearInputBuffer(); // Xóa bộ đệm trước khi nhập
        fgets(products[i].name, sizeof(products[i].name), stdin);
        products[i].name[strcspn(products[i].name, "\n")] = 0; // Xóa ký tự xuống dòng

        printf("So luong: ");
        scanf("%d", &products[i].quantity);

        printf("Don gia: ");
        scanf("%f", &products[i].price);
    }
}

int main()
{
    struct Product products[5];
    int choice;

    do
    {
        printf("\nMENU\n");
        printf("1. Nhap san pham\n");
        printf("2. Hien thi danh sach\n");
        printf("3. Sap xep theo ten A-Z\n");
        printf("4. Sap xep theo gia tang dan\n");
        printf("5. Thoat\n");
        printf("Lua chon: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            inputProducts(products, 5);
            break;
        case 2:
            displayProducts(products, 5);
            break;
        case 3:
            sortByName(products, 5);
            printf("\nDanh sach sap xep theo ten A-Z:\n");
            displayProducts(products, 5);
            break;
        case 4:
            sortByPrice(products, 5);
            printf("\nDanh sach sap xep theo gia tang dan:\n");
            displayProducts(products, 5);
            break;
        case 5:
            printf("Thoat chuong trinh.\n");
            break;
        default:
            printf("Lua chon khong hop le!\n");
        }
    } while (choice != 5);

    return 0;
}
