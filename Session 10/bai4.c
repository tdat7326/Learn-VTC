#include <stdio.h>
#include <string.h>
#include <ctype.h>

int isPalindrome(char str[])
{
    int left = 0, right = strlen(str) - 1;

    while (left < right)
    {
        // Bỏ qua khoảng trắng và ký tự không phải chữ cái
        while (left < right && !isalnum(str[left]))
            left++;
        while (left < right && !isalnum(str[right]))
            right--;

        // So sánh ký tự (không phân biệt hoa thường)
        if (tolower(str[left]) != tolower(str[right]))
            return 0;

        left++;
        right--;
    }

    return 1;
}

int main()
{
    char str[1000];

    printf("Nhap mot chuoi: ");
    fgets(str, sizeof(str), stdin);
    str[strcspn(str, "\n")] = 0; // Xóa ký tự xuống dòng

    if (isPalindrome(str))
        printf("Chuoi la Palindrome.\n");
    else
        printf("Chuoi khong la Palindrome.\n");

    return 0;
}
