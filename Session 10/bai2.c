#include <stdio.h>
#include <string.h>
#include <ctype.h>

void chuanHoaChuoi(char *str)
{
    int i, len = strlen(str);
    int inWord = 0; // Đánh dấu đang ở trong từ hay không

    // Xóa khoảng trắng đầu chuỗi
    while (len > 0 && isspace(str[0]))
    {
        for (i = 0; i < len - 1; i++)
        {
            str[i] = str[i + 1];
        }
        len--;
        str[len] = '\0';
    }

    // Xóa khoảng trắng cuối chuỗi
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
                str[i] = ' '; // Giữ lại một khoảng trắng
                inWord = 0;
            }
            else
            {
                // Xóa khoảng trắng thừa
                for (int j = i; j < len - 1; j++)
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
                str[i] = toupper(str[i]); // Viết hoa chữ cái đầu từ
            }
            else
            {
                str[i] = tolower(str[i]); // Các chữ cái khác viết thường
            }
            inWord = 1;
        }
    }
}

int main()
{
    char str[100];

    printf("Nhap chuoi: ");
    fflush(stdin);
    fgets(str, sizeof(str), stdin);

    printf("Chuoi truoc khi chuan hoa: %s\n", str);
    chuanHoaChuoi(str);
    printf("Chuoi sau khi chuan hoa: %s\n", str);

    return 0;
}