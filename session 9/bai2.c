#include <stdio.h>

// Khai báo hàm power
int power(int a, int n);
int inputNumber();
int checkNumber(int num);

int main()
{
    int a, n;

    // Nhập vào hai số nguyên a và n
    printf("Nhap so a: ");
    scanf("%d", &a);
    printf("Nhap so n: ");
    scanf("%d", &n);

    // Gọi hàm power và in kết quả
    printf("Ket qua %d^%d = %d\n", a, n, power(a, n));

    // Sử dụng các hàm mới
    int number = inputNumber();
    if (checkNumber(number))
    {
        printf("%d la so nguyen to.\n", number);
    }
    else
    {
        printf("%d khong phai la so nguyen to.\n", number);
    }

    // Nhập dữ liệu cho mảng và kiểm tra số nguyên tố
    int arr[7];
    printf("Nhap 7 so nguyen:\n");
    for (int i = 0; i < 7; i++)
    {
        arr[i] = inputNumber();
    }

    printf("Cac so nguyen to trong mang la: ");
    for (int i = 0; i < 7; i++)
    {
        if (checkNumber(arr[i]))
        {
            printf("%d ", arr[i]);
        }
    }
    printf("\n");

    return 0;
}

// Định nghĩa hàm power
int power(int a, int n)
{
    int result = 1;
    for (int i = 0; i < n; i++)
    {
        result *= a;
    }
    return result;
}

// Định nghĩa hàm inputNumber
int inputNumber()
{
    int num;
    printf("Nhap mot so nguyen: ");
    scanf("%d", &num);
    return num;
}

// Định nghĩa hàm checkNumber
int checkNumber(int num)
{
    if (num <= 1)
        return 0;
    for (int i = 2; i * i <= num; i++)
    {
        if (num % i == 0)
            return 0;
    }
    return 1;
}