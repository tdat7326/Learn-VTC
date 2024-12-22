#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Hàm chuẩn hóa xâu giống bài 2
void chuanHoaXau(char *str)
{
    int i, j, len = strlen(str);
    int inWord = 0;

    // Xóa khoảng trắng đầu
    while (len > 0 && isspace(str[0]))
    {
        for (i = 0; i < len - 1; i++)
        {
            str[i] = str[i + 1];
        }
        len--;
        str[len] = '\0';
    }

    // Xóa khoảng trắng cuối
    while (len > 0 && isspace(str[len - 1]))
    {
        str[len - 1] = '\0';
        len--;
    }

    // Chuẩn hóa khoảng trắng giữa các từ và viết hoa chữ cái đầu
    for (i = 0; i < len; i++)
    {
        if (isspace(str[i]))
        {
            if (inWord)
            {
                str[i] = ' ';
                inWord = 0;
            }
            else
            {
                for (j = i; j < len - 1; j++)
                {
                    str[j] = str[j + 1];
                }
                len--;
                i--;
            }
        }
        else
        {
            if (!inWord && (i == 0 || isspace(str[i - 1])))
            {
                str[i] = toupper(str[i]);
            }
            else
            {
                str[i] = tolower(str[i]);
            }
            inWord = 1;
        }
    }
}

// Hàm đếm nguyên âm
int demNguyenAm(const char *str)
{
    int count = 0;
    for (int i = 0; str[i] != '\0'; i++)
    {
        char c = tolower(str[i]);
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u')
        {
            count++;
        }
    }
    return count;
}

// Hàm đếm số từ
int demTu(const char *str)
{
    int count = 0;
    int inWord = 0;

    for (int i = 0; str[i] != '\0'; i++)
    {
        if (isspace(str[i]))
        {
            if (inWord)
            {
                count++;
                inWord = 0;
            }
        }
        else if (!isspace(str[i]))
        {
            inWord = 1;
        }
    }

    if (inWord)
    {
        count++;
    }

    return count;
}

int main()
{
    char str[81]; // 80 ký tự + NULL
    printf("Nhap chuoi (toi da 80 ky tu): ");
    fflush(stdin);
    fgets(str, sizeof(str), stdin);

    // Kiểm tra độ dài
    if (strlen(str) > 80)
    {
        printf("Chuoi vuot qua 80 ky tu!\n");
        return 1;
    }

    // Chuẩn hóa xâu
    printf("\nChuoi truoc khi chuan hoa: %s\n", str);
    chuanHoaXau(str);
    printf("Chuoi sau khi chuan hoa: %s\n", str);

    // Thống kê
    int soKyTu = strlen(str);
    int soNguyenAm = demNguyenAm(str);
    float tiLeNguyenAm = (float)soNguyenAm / soKyTu * 100;
    int soTu = demTu(str);

    printf("\nThong ke chuoi:\n");
    printf("1. So ky tu trong cau: %d\n", soKyTu);
    printf("2. So nguyen am: %d\n", soNguyenAm);
    printf("3. Ty le nguyen am: %.2f%%\n", tiLeNguyenAm);
    printf("4. So tu trong cau: %d\n", soTu);

    return 0;
}