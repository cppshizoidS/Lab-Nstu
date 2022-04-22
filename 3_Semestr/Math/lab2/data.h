#ifndef DATA_H_
#define DATA_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void input(int size, float matrix[size][size]);
void array_output(int size, float array[size]);
void output(int size, float matrix[size][size]);

void holeckovo(int size, float matrix[size][size]);

int validate_epsilon(int size, float x[size], float nextX[size]);
int diag_dominance(int size, float matrix[size][size]);
void iteration(int size, float matrix[size][size]);

void gauss(int size, float matrix[size][size]);

#endif  // DATA_H_
