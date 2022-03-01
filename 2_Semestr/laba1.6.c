##include <stdio.h>
static int vars[] = {12, 8, 32, 123};
int main() {
    printf("\n\t\tИсходные данные:\n\tvars[0]=%d, vars[1]=%d, vars[2]=0%o, vars[3]=0%o", vars[0], vars[1], vars[2], vars[3]);
    vars[0] = vars[0] % vars[1] + ++vars[2] + vars[3]++;
    vars[1] += 5;
    vars[2] = (vars[2] + ++vars[0] + vars[1]--) / 5;
    vars[3] = (vars[1] & 0b100) & ((vars[2] & 0b111110000) << 2);
    printf("\n\t\tРезультаты расчёта:\n\tvars[0]=%d, vars[1]=%d, vars[2]=0%o, vars[3]=0%o", vars[0], vars[1], vars[2], vars[3]);
    return 0;
}
