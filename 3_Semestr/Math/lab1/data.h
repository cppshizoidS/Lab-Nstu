#ifndef DATA_H_
#define DATA_H_

double func(double x);
double iter_func(double x, double L);

double first_derivate(double x);
double second_derivate(double x);

int newton(double x, double a, double b, double epsilon);
void iteration(double x, double a, double b, double epsilon);
void half_div(double x, double a, double b, double epsilon);

void output(double x, double a, double b, int counter);

#endif  // DATA_H_