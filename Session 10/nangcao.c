#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 100

void chuanHoa(char str[]);
void nhapHocVien();
void timHocVien();
void chuanHoaDanhSach();
void hienThiDanhSach();

char students[MAX][100];
int studentCount = 0;

// Chuẩn hóa tên
void chuanHoa(char str[])
{
    int len = strlen(str), i, j = 0;
    char temp[100];
    int capitalize = 1;

    // Xóa khoảng trắng thừa và chuẩn hóa
    for (i = 0; i < len; i++)
    {
        if (isspace(str[i]))
        {
            if (j > 0 && !isspace(temp[j - 1]))
            {
                temp[j++] = ' ';
            }
        }
        else
        {
            temp[j++] = capitalize ? toupper(str[i]) : tolower(str[i]);
            capitalize = isspace(str[i]);
        }
    }

    if (j > 0 && temp[j - 1] == ' ')
        j--; // Xóa khoảng trắng cuối chuỗi

    temp[j] = '\0';
    strcpy(str, temp);
}

// Nhập học viên mới
void nhapHocVien()
{
    if (studentCount >= MAX)
    {
        printf("Danh sach day!\n");
        return;
    }
    printf("Nhap ten hoc vien: ");
    fgets(students[studentCount], sizeof(students[studentCount]), stdin);
    students[studentCount][strcspn(students[studentCount], "\n")] = 0; // Xóa xuống dòng
    studentCount++;
    printf("Da them hoc vien moi.\n");
}

// Tìm học viên
void timHocVien()
{
    char name[100];
    printf("Nhap ten hoc vien can tim: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0; // Xóa xuống dòng

    for (int i = 0; i < studentCount; i++)
    {
        if (strcasecmp(students[i], name) == 0)
        {
            printf("Tim thay hoc vien: %s\n", students[i]);
            return;
        }
    }
    printf("Khong tim thay hoc vien!\n");
}

// Chuẩn hóa tên học viên
void chuanHoaDanhSach()
{
    for (int i = 0; i < studentCount; i++)
    {
        chuanHoa(students[i]);
    }
    printf("Da chuan hoa ten hoc vien.\n");
}

// Hiển thị danh sách học viên
void hienThiDanhSach()
{
    printf("STT\tTen hoc vien\n");
    for (int i = 0; i < studentCount; i++)
    {
        printf("%02d\t%s\n", i + 1, students[i]);
    }
}

int main()
{
    int choice;

    do
    {
        printf("\n=== MENU ===\n");
        printf("1. Nhap them mot hoc vien moi\n");
        printf("2. Tim kiem hoc vien\n");
        printf("3. Chuan hoa ten hoc vien\n");
        printf("4. Hien thi danh sach hoc vien\n");
        printf("0. Thoat chuong trinh\n");
        printf("Moi ban chon: ");
        scanf("%d", &choice);
        getchar(); // Xóa ký tự xuống dòng

        switch (choice)
        {
        case 1:
            nhapHocVien();
            break;
        case 2:
            timHocVien();
            break;
        case 3:
            chuanHoaDanhSach();
            break;
        case 4:
            hienThiDanhSach();
            break;
        case 0:
            printf("Thoat chuong trinh.\n");
            break;
        default:
            printf("Lua chon khong hop le!\n");
        }
    } while (choice != 0);

    return 0;
}
