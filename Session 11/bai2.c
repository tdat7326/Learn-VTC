#include <stdio.h>
#include <string.h>

struct Product
{
    char name[50];
    int quantity;
    float price;
};

// Hàm nhập thông tin sản phẩm
void inputProducts(struct Product products[], int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("Nhap thong tin san pham thu %d:\n", i + 1);
        printf("Ten san pham: ");
        fflush(stdin);
        fgets(products[i].name, sizeof(products[i].name), stdin);
        products[i].name[strcspn(products[i].name, "\n")] = 0; // Xóa ký tự xuống dòng
        printf("So luong: ");
        scanf("%d", &products[i].quantity);
        printf("Don gia: ");
        scanf("%f", &products[i].price);
    }
}

// Hàm hiển thị danh sách sản phẩm
void displayProducts(struct Product products[], int n)
{
    printf("DANH MUC SAN PHAM\n");
    printf("------------------\n");
    printf("STT | Ten san pham       | So luong | Don gia  | Tong tien\n");
    for (int i = 0; i < n; i++)
    {
        float total = products[i].quantity * products[i].price;
        printf("%-4d| %-18s| %-9d| %-9.2f| %-10.2f\n",
               i + 1, products[i].name, products[i].quantity, products[i].price, total);
    }
}

int main()
{
    struct Product products[5];
    inputProducts(products, 5);
    displayProducts(products, 5);
    return 0;
}
