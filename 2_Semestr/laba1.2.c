#include <stdio.h>

int main() {
    static int A = 12, B = 8, C = 32, D = 123;
    printf("\n\t\tИсходные данные:\n\tA=%d, B=%d, C=0%o, D=0%o", A, B, C, D);
    A = A % B + ++C + D++;
    B += 5;
    C = (C + ++A + B--) / 5;
    D = (B & 0b100) & ((C & 0b111110000) << 2);
    printf("\n\t\tРезультаты расчёта:\n\tA=%d, B=%d, C=0%o, D=0%o", A, B, C, D);
    return 0;
}
