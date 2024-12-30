#include <stdio.h>
#include <string.h>

struct Mark
{
    char subject[80];
    float written;
    float practical;
};

// Hàm nhập thông tin
void inputMark(struct Mark *mark)
{
    printf("Nhap ten mon hoc: ");
    fflush(stdin);                                      // Làm sạch bộ đệm
    fgets(mark->subject, sizeof(mark->subject), stdin); // Sử dụng fgets để nhập chuỗi
    mark->subject[strcspn(mark->subject, "\n")] = 0;    // Loại bỏ ký tự '\n' nếu có

    printf("Nhap diem ly thuyet: ");
    scanf("%f", &mark->written);
    printf("Nhap diem thuc hanh: ");
    scanf("%f", &mark->practical);
}

// Hàm hiển thị thông tin
void displayMark(struct Mark mark)
{
    float avg = (mark.written + mark.practical) / 2.0;
    printf("THONG TIN VE DIEM\n");
    printf("------------------\n");
    printf("Mon hoc: %s\n", mark.subject);
    printf("Diem LT: %.2f\n", mark.written);
    printf("Diem TH: %.2f\n", mark.practical);
    printf("Diem TB: %.2f\n", avg);
}

int main()
{
    struct Mark mark;
    inputMark(&mark);
    displayMark(mark);
    return 0;
}
