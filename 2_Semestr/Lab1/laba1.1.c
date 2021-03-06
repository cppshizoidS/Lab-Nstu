#include <stdio.h>
/*
А - разделить по модулю В, прибавить сумму С, увеличенного на 1, и D.  D увеличить на 1.
В - увеличить на 5.
C - сложить с А, увеличенным на 1, и В. В уменьшить на 1. Результат разделить на 5.
D - выполнить поразрядное «и» 2 или 6 разряда В и 4-8 разрядов С, сдвинутых влево на 2 разряда.
*/

static int A = 12, B = 8, C = 32, D = 123;
int main() {
    printf("\n\t\tИсходные данные:\n\tA=%d, B=%d, C=0%o, D=0%o", A, B, C, D);
    A = A % B + ++C + D++;
    B += 5;
    C = (C + ++A + B--) / 5;
    D = (B & 0b100) & ((C & 0b111110000) << 2);
    printf("\n\t\tРезультаты расчёта:\n\tA=%d, B=%d, C=0%o, D=0%o", A, B, C, D);
    return 0;
}
