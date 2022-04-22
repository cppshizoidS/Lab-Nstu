#include <stdio.h>
#include <math.h>
#include "data.h"

int main() {
    double a, b, x = 0;
    double epsilon = 0.000001;

    if (scanf("%lf %lf", &a, &b) != 2) {
        printf("n/a");
    } else {
        printf("\n===HALF DIV===\n");
        half_div(x, a, b, epsilon);

        printf("\n===NEWTON===\n");
        newton(x, a, b, epsilon);

        printf("\n===SIMPLE ITER===\n");
        iteration(x, a, b, epsilon);

        
    }
    return 0;
}

double func(double x) {
    return pow(x, 3) - 3 * pow(x, 2) + 2 * x - 1.5;
}

double iter_func(double x, double L) {
    return x + L * func(x);
}

double first_derivate(double x) {
    return 3 * pow(x, 2) - 6 * x + 2;
}

double second_derivate(double x) {
    return 6 * x - 6;
}

void iteration(double x, double a, double b, double epsilon) {
    x = (b + a) / 2;
    double L = - 1.0/4;
    int counter = 0;
    double checker = 0;

    while (fabs(checker - x) > epsilon) {
        x = iter_func(x, L);
        checker = iter_func(x, L);
        counter++;
    }

    output(x, a, b, counter);
}

void half_div(double x, double a, double b, double epsilon) {
    int counter = 0;
    double temp_a = a;
    double temp_b = b;

    if (func(a) * func(b) < 0) {
        while (fabs(temp_b - temp_a) / 2 > epsilon) {
            double temp = (temp_a + temp_b) / 2;
            if (func(temp_a)*func(temp) > 0) {
                temp_a = temp;
            } else {
                temp_b = temp;
            }
            counter++;
            x = temp;
        }
        output(x, a, b, counter);
    } else {
        printf("n/a");
    }
}

int newton(double x, double a, double b, double epsilon) {
    int counter = 0;
    double checker = 0;
    if (func(a) * func(b) > 0) {
        printf("n/a");
        return 1;
    }
    if (func(a) * second_derivate(a) > 0) {
        x = a;
    } else {
        x = b;
    }

    while (fabs(checker - x) > epsilon) {
        x -= func(x) / first_derivate(x);
        checker = x - func(x) / first_derivate(x);
        counter++;
    }
    output(x, a, b, counter);
    return 0;
}

void output(double x, double a, double b, int counter) {
    if (x >= a && x <= b) {
        printf("RESULT: %0.6lf\n", x);
        printf("ITERATIONS MADE: %d\n", counter);
    } else {
        printf("n/a");
    }
}