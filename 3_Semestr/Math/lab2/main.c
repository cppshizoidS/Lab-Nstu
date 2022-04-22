#include "data.h"

int main() {
  int size = 4;
  float matrix[size][size];
  clock_t start, end;

  input(size, matrix);
  start = clock();
  holeckovo(size, matrix);
  end = clock();
  printf("\nExecution time: %.1lfms\n", difftime(end, start));
  if (diag_dominance(size, matrix)) {
    start = clock();
    iteration(size, matrix);
    end = clock();
    printf("\nExecution time: %.1lfms\n", difftime(end, start));
  } else {
    printf(
        "\n\nMatrix does not fulfill the condition of diagonal dominance. Can "
        "not proceed iteration method.\nStopped.\n");
  }

  start = clock();
  gauss(size, matrix);
  end = clock();
  printf("Execution time: %.1lfms\n", difftime(end, start));

  return 0;
}

void input(int size, float matrix[size][size]) {
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      scanf("%f", &matrix[i][j]);
    }
  }
}

void array_output(int size, float array[size]) {
  for (int i = 0; i < size; i++) {
    if (i != 0) printf(" ");
    printf("%.2f", array[i]);
  }
}

void output(int size, float matrix[size][size]) {
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      if (j != 0) printf(" ");
      printf("%.3f", matrix[i][j]);
    }
    printf("\n");
  }
}

void holeckovo(int size, float matrix[size][size]) {
  float lMatrix[size][size];
  float uMatrix[size][size];
  float b[4] = {
      19, 0, 34,
      -49};  // Four for size of matrix. Need to be number because of init error
  float x[4] = {0, 0, 0, 0};  // Original system init solution

  // Fill LU-Matrix elements with 0 value
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      lMatrix[i][j] = 0;
      uMatrix[i][j] = 0;
    }
  }
  // Find true values of LU-Matrix
  for (int i = 0; i < size; i++) {
    for (int k = 0; k < size; k++) {
      // L-Matrix elements
      if (k <= i) {
        float temp = 0;
        for (int j = 0; j < k; j++) {
          temp += lMatrix[i][j] * uMatrix[j][k];
        }
        lMatrix[i][k] = matrix[i][k] - temp;
      }
      // U-Matrix elements
      if (i < k) {
        float temp = 0;
        for (int j = 0; j < i; j++) {
          temp += lMatrix[i][j] * uMatrix[j][k];
        }
        uMatrix[i][k] = (matrix[i][k] - temp) / lMatrix[i][i];
      }
      if (i == k) {
        uMatrix[k][k] = 1;
      }
    }
  }
  // Original system solution
  for (int i = 0; i < size; i++) {
    float temp = 0;
    for (int j = 0; j < i; j++) {
      temp += lMatrix[i][j] * b[j];
    }
    b[i] = (b[i] - temp) / lMatrix[i][i];
  }

  for (int i = size - 1; i >= 0; i--) {
    float temp = 0;
    for (int j = size - 1; j > i; j--) {
      temp += uMatrix[i][j] * x[j];
    }
    x[i] = b[i] - temp;
  }

  printf("\n===CHOLETSY===\n");
  array_output(size, x);
}

int validate_epsilon(int size, float x[size], float nextX[size]) {
  float epsilon = 0.00001;
  int flag = 0;
  for (int i = 0; i < size; i++) {
    if (fabs(x[i] - nextX[i]) > epsilon) {
      return 1;
    }
    flag += 1;
  }
  return flag == size ? 0 : 1;
}

int diag_dominance(int size, float matrix[size][size]) {
  for (int i = 0; i < size; i++) {
    int temp = 0;
    for (int j = 0; j < size; j++) {
      if (i != j) {
        temp += fabs(matrix[i][j]);
      }
      if (fabs(matrix[i][i]) < temp) {
        return 0;
      }
    }
  }
  return 1;
}

void iteration(int size, float matrix[size][size]) {
  float b[4] = {19, 0, 34, -49};
  float x[4] = {0, 0, 0, 0};
  float nextX[4] = {0, 0, 0, 0};
  do {
    for (int i = 0; i < size; i++) {
      x[i] = nextX[i];
    }

    for (int i = 0; i < size; i++) {
      float beta = b[i] / matrix[i][i];
      float temp = 0;

      for (int j = 0; j < size; j++) {
        float newA = 0;
        if (i != j) {
          newA = -(matrix[i][j] / matrix[i][i]);
        }
        temp += x[j] * newA;
      }
      nextX[i] = beta + temp;
    }
  } while (validate_epsilon(size, x, nextX));

  printf("\n===ITERATIONS===\n");
  array_output(size, x);
}

void gauss(int size, float matrix[size][size]) {
  float b[4] = {19, 0, 34, -49};
  float equation[4][5];
  float result[4] = {0, 0, 0, 0};
  // Fill equation matrix
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size + 1; j++) {
      if (j != size) {
        equation[i][j] = matrix[i][j];
      } else {
        equation[i][j] = b[i];
      }
    }
  }
  // Forward stroke
  for (int i = 0; i < size; i++) {
    float firstElement = equation[i][i];
    for (int j = 0; j < size + 1; j++) {
      equation[i][j] /= firstElement;
    }

    for (int k = i + 1; k < size; k++) {
      float coefficient = equation[k][i] / equation[i][i];
      for (int j = 0; j < size + 1; j++) {
        equation[k][j] -= (equation[i][j] * coefficient);
      }
    }
  }
  // Back stroke
  for (int i = size - 1; i >= 0; i--) {
    float val = equation[i][size];
    float sum = 0;
    for (int j = 0; j < size; j++) {
      sum += result[j] == 0 ? 0 : equation[i][j] * result[j];
    }
    result[i] = (val - sum) / equation[i][i];
  }

  printf("\n===GAUSS===\n");

  for (int i = 0; i < size; i++) {
    printf("%0.2f ", result[i]);
  }
  printf("\n");
}
