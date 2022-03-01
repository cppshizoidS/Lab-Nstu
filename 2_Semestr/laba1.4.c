#include <stdio.h>

int main() {
    auto int A, B, C, D;
    printf("\n\t\tДо присваивания:\n\tA=%d, B=%d, C=0%o, D=0%o", A, B, C, D);
    printf("\nВведите A, B, C, D: ");
    scanf("%d %d %d %d", &A, &B, &C, &D);
    printf("\n\t\tИсходные данные:\n\tA=%d, B=%d, C=0%o, D=0%o", A, B, C, D);
    A = A % B + ++C + D++;
    B += 5;
    C = (C + ++A + B--) / 5;
    D = (B & 0b100) & ((C & 0b111110000) << 2);
    printf("\n\t\tРезультаты расчёта:\n\tA=%d, B=%d, C=0%o, D=0%o", A, B, C, D);
    return 0;
}
