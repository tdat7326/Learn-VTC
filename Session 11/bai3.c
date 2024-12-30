#include <stdio.h>
#include <string.h>

struct Mark
{
    char subject[80];
    float written;
    float practical;
};

// Hàm xóa bộ đệm nhập liệu
void clearInputBuffer()
{
    while (getchar() != '\n')
        ;
}

// Hàm nhập điểm
void inputMarks(struct Mark marks[], int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("Nhap thong tin mon hoc thu %d:\n", i + 1);
        printf("Ten mon hoc: ");
        clearInputBuffer(); // Xóa bộ đệm trước khi dùng fgets
        fgets(marks[i].subject, sizeof(marks[i].subject), stdin);
        marks[i].subject[strcspn(marks[i].subject, "\n")] = 0; // Xóa ký tự xuống dòng

        printf("Diem ly thuyet: ");
        scanf("%f", &marks[i].written);

        printf("Diem thuc hanh: ");
        scanf("%f", &marks[i].practical);
    }
}

// Hàm hiển thị danh sách
void displayMarks(struct Mark marks[], int n)
{
    printf("DANH SACH MON HOC\n");
    printf("------------------\n");
    for (int i = 0; i < n; i++)
    {
        float avg = (marks[i].written + marks[i].practical) / 2.0;
        printf("%d. Mon hoc: %s, Diem TB: %.2f\n", i + 1, marks[i].subject, avg);
    }
}

// Hàm tìm môn học có điểm TB cao nhất
void highestMark(struct Mark marks[], int n)
{
    int maxIndex = 0;
    float maxAvg = (marks[0].written + marks[0].practical) / 2.0;
    for (int i = 1; i < n; i++)
    {
        float avg = (marks[i].written + marks[i].practical) / 2.0;
        if (avg > maxAvg)
        {
            maxAvg = avg;
            maxIndex = i;
        }
    }
    printf("Mon hoc co diem TB cao nhat:\n");
    printf("Mon hoc: %s, Diem TB: %.2f\n", marks[maxIndex].subject, maxAvg);
}

int main()
{
    struct Mark marks[6];
    int choice;

    do
    {
        printf("\nMENU\n");
        printf("1. Nhap diem\n");
        printf("2. Hien thi danh sach\n");
        printf("3. Mon hoc co diem TB cao nhat\n");
        printf("4. Thoat\n");
        printf("Lua chon: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            inputMarks(marks, 6);
            break;
        case 2:
            displayMarks(marks, 6);
            break;
        case 3:
            highestMark(marks, 6);
            break;
        case 4:
            printf("Thoat chuong trinh.\n");
            break;
        default:
            printf("Lua chon khong hop le!\n");
        }
    } while (choice != 4);

    return 0;
}
