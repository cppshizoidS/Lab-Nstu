#include <stdio.h>


static int A = 12, B = 8, C = 32, D = 123;
int main() {

    printf("\n\t\tInitial data:\n\tA=%d, B=%d, C=0%o, D=0%o", A, B, C, D);
    A = A % B + ++C + D++;
    B += 5;
    C = (C + ++A + B--) / 5;
    D = (B & 0b100) & ((C & 0b111110000) << 2);
    printf("\n\t\tResults:\n\tA=%d, B=%d, C=0%o, D=0%o", A, B, C, D);
    return 0;
}