#include <stdio.h>

// Hàm hiển thị menu
void displayMenu();

// Hàm xử lý nhập lựa chọn
int getMenuChoice();

int main()
{
    int choice;

    do
    {
        displayMenu();
        choice = getMenuChoice();

        // Xử lý lựa chọn
        switch (choice)
        {
        case 1:
            printf("You chose Menu 1\n");
            break;
        case 2:
            printf("You chose Menu 2\n");
            break;
        case 3:
            printf("You chose Menu 3\n");
            break;
        case 4:
            printf("You chose Menu 4\n");
            break;
        case 5:
            printf("Exiting the program. Goodbye!\n");
            break;
        default:
            printf("Invalid choice, please try again!\n");
        }
    } while (choice != 5);

    return 0;
}

// Hàm hiển thị menu
void displayMenu()
{
    printf("-------------- MENU --------------\n");
    printf("1. Menu 1\n");
    printf("2. Menu 2\n");
    printf("3. Menu 3\n");
    printf("4. Menu 4\n");
    printf("5. Exit\n");
    printf("----------------------------------\n");
    printf("Please choose: ");
}

// Hàm xử lý nhập lựa chọn
int getMenuChoice()
{
    int choice;
    while (1)
    {
        if (scanf("%d", &choice) != 1 || choice < 1 || choice > 5)
        {
            while (getchar() != '\n')
                ; // Xóa bộ đệm nhập sai
            printf("Entered incorrectly, re-enter: ");
        }
        else
        {
            return choice;
        }
    }
}