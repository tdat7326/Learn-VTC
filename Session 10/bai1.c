#include <stdio.h>
#include <string.h>

int main()
{
    char str[100], ch;
    int count = 0, i;

    printf("Nhap chuoi: ");
    fflush(stdin);
    fgets(str, sizeof(str), stdin);

    printf("Nhap ky tu can dem: ");
    scanf(" %c", &ch);

    for (i = 0; str[i] != '\0'; i++)
    {
        if (str[i] == ch)
        {
            count++;
        }
    }

    printf("So lan xuat hien cua ky tu '%c' trong chuoi la: %d\n", ch, count);

    return 0;
}