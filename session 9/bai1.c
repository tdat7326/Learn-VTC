#include <stdio.h>

int power(int a, int n);

int main()
{
    // Khai báo hàm power
    int a, n;
    // Nhập vào hai số nguyên a và n
    printf("Nhap so a: ");
    scanf("%d", &a);
    printf("Nhap so n: ");
    scanf("%d", &n);
    // Gọi hàm power và in kết quả
    printf("Ket qua %d^%d = %d\n", a, n, power(a, n));

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