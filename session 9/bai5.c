#include <stdio.h>

#define SIZE 100

// Hàm chức năng
void displayMenu();
void addElement(int arr[], int *n);
void updateElement(int arr[], int n);
void deleteElement(int arr[], int *n);
void linearSearch(int arr[], int n);
void secondSearch(int arr[], int n);
void sortDescending(int arr[], int n);
void insertSorted(int arr[], int *n);
void displayArray(int arr[], int n);

int main()
{
    int arr[SIZE];
    int n = 0; // Số phần tử của mảng
    int choice;

    do
    {
        displayMenu();
        printf("Moi ban chon: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            addElement(arr, &n);
            break;
        case 2:
            updateElement(arr, n);
            break;
        case 3:
            deleteElement(arr, &n);
            break;
        case 4:
            linearSearch(arr, n);
            break;
        case 5:
            secondSearch(arr, n);
            break;
        case 6:
            sortDescending(arr, n);
            break;
        case 7:
            insertSorted(arr, &n);
            break;
        case 8:
            displayArray(arr, n);
            break;
        case 0:
            printf("Thoat chuong trinh. Tam biet!\n");
            break;
        default:
            printf("Lua chon khong hop le. Moi nhap lai!\n");
        }
    } while (choice != 0);

    return 0;
}

// Hiển thị menu
void displayMenu()
{
    printf("\n------------- MENU -------------\n");
    printf("1. Nhap them mot phan tu cho mang\n");
    printf("2. Sua phan tu theo vi tri cua mang\n");
    printf("3. Xoa phan tu mang mang theo vi tri\n");
    printf("4. Tim kiem tuyen tinh mot phan tu trong mang\n");
    printf("5. Tim kiem nhi phan mot phan tu trong mang\n");
    printf("6. Sap xep theo chieu giam dan\n");
    printf("7. Chen them mot phan tu cho mang sao cho danh sach sap xep van duoc bao toan\n");
    printf("8. Hien thi mang\n");
    printf("0. Thoat chuong trinh\n");
    printf("--------------------------------\n");
}

// Thêm phần tử vào mảng
void addElement(int arr[], int *n)
{
    if (*n >= SIZE)
    {
        printf("Mang da day!\n");
        return;
    }
    printf("Nhap phan tu can them: ");
    scanf("%d", &arr[*n]);
    (*n)++;
}

// Sửa phần tử theo vị trí
void updateElement(int arr[], int n)
{
    int pos, value;
    printf("Nhap vi tri can sua (0-%d): ", n - 1);
    scanf("%d", &pos);

    if (pos >= 0 && pos < n)
    {
        printf("Nhap gia tri moi: ");
        scanf("%d", &value);
        arr[pos] = value;
    }
    else
    {
        printf("Vi tri khong hop le!\n");
    }
}

// Xóa phần tử theo vị trí
void deleteElement(int arr[], int *n)
{
    int pos;
    printf("Nhap vi tri can xoa (0-%d): ", *n - 1);
    scanf("%d", &pos);

    if (pos >= 0 && pos < *n)
    {
        for (int i = pos; i < *n - 1; i++)
        {
            arr[i] = arr[i + 1];
        }
        (*n)--;
    }
    else
    {
        printf("Vi tri khong hop le!\n");
    }
}

// Tìm kiếm tuyến tính
void linearSearch(int arr[], int n)
{
    int key, found = 0;
    printf("Nhap gia tri can tim: ");
    scanf("%d", &key);

    for (int i = 0; i < n; i++)
    {
        if (arr[i] == key)
        {
            printf("Tim thay %d tai vi tri %d\n", key, i);
            found = 1;
        }
    }
    if (!found)
    {
        printf("Khong tim thay %d trong mang!\n", key);
    }
}

// Sắp xếp giảm dần
void sortDescending(int arr[], int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if (arr[i] < arr[j])
            {
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
    printf("Mang da duoc sap xep giam dan!\n");
}

// Chèn phần tử vào mảng có sắp xếp
void insertSorted(int arr[], int *n)
{
    if (*n >= SIZE)
    {
        printf("Mang da day!\n");
        return;
    }
    int value, i;
    printf("Nhap gia tri can chen: ");
    scanf("%d", &value);

    // Tìm vị trí thích hợp để chèn
    for (i = *n; i > 0 && arr[i - 1] < value; i--)
    {
        arr[i] = arr[i - 1];
    }
    arr[i] = value;
    (*n)++;
    printf("Da chen phan tu thanh cong!\n");
}

// Tìm kiếm nhị phân (yêu cầu mảng đã sắp xếp)
void secondSearch(int arr[], int n)
{
    int key, left = 0, right = n - 1, mid, found = 0;
    printf("Nhap gia tri can tim (nhi phan): ");
    scanf("%d", &key);

    while (left <= right)
    {
        mid = (left + right) / 2;
        if (arr[mid] == key)
        {
            printf("Tim thay %d tai vi tri %d\n", key, mid);
            found = 1;
            break;
        }
        else if (arr[mid] < key)
        {
            right = mid - 1;
        }
        else
        {
            left = mid + 1;
        }
    }
    if (!found)
    {
        printf("Khong tim thay %d trong mang!\n", key);
    }
}

// Hiển thị mảng
void displayArray(int arr[], int n)
{
    printf("Mang hien tai: ");
    for (int i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}