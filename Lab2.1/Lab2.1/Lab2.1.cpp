#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

void multiply(int n) {
    // Выделение памяти под матрицы
    double** A = (double**)malloc(n * sizeof(double*));
    double** B = (double**)malloc(n * sizeof(double*));
    double** C = (double**)malloc(n * sizeof(double*));
    for (int i = 0; i < n; i++) {
        A[i] = (double*)malloc(n * sizeof(double));
        B[i] = (double*)malloc(n * sizeof(double));
        C[i] = (double*)malloc(n * sizeof(double));
    }

    // Заполнение случайными числами
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            A[i][j] = rand() % 100;
            B[i][j] = rand() % 100;
            C[i][j] = 0;
        }

    // Засекаем время
    clock_t start = clock();

    // Классическое перемножение матриц
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            for (int k = 0; k < n; k++)
                C[i][j] += A[i][k] * B[k][j];

    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Размер матрицы: %d x %d | Время выполнения: %.3f секунд\n", n, n, time_spent);

    // Освобождение памяти
    for (int i = 0; i < n; i++) {
        free(A[i]); free(B[i]); free(C[i]);
    }
    free(A); free(B); free(C);
}

int main() {
    setlocale(LC_ALL, "Russian");
    srand(time(NULL));

    int sizes[] = { 100, 200, 400, 1000, 2000, 4000, 10000 };
    int count = sizeof(sizes) / sizeof(sizes[0]);

    printf("Замер времени перемножения квадратных матриц\n\n");

    for (int i = 0; i < count; i++) {
        multiply(sizes[i]);
    }

    printf("\nТеоретическая сложность алгоритма: O(n^3)\n");

    return 0;
}
