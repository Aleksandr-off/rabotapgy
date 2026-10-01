#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <string.h>

void generateRandom(int* arr, int n, int maxValue) {
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % maxValue;
    }
}

void generateAscending(int* arr, int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = i;
    }
}

void generateDescending(int* arr, int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = n - i;
    }
}

void generateHalfAscHalfDesc(int* arr, int n) {
    int mid = n / 2;
    for (int i = 0; i < mid; i++) {
        arr[i] = i;
    }
    for (int i = mid; i < n; i++) {
        arr[i] = n - (i - mid) - 1;
    }
}

int* copyArray(int* original, int n) {
    int* copy = (int*)malloc(n * sizeof(int));
    if (copy == NULL) {
        printf("Ошибка выделения памяти!\n");
        exit(1);
    }
    memcpy(copy, original, n * sizeof(int));
    return copy;
}

int isSorted(int* arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            return 0;
        }
    }
    return 1;
}

int compare(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

// --- Методы сортировки ---

void quickSortIterative(int* arr, int low, int high) {
    int* stack = (int*)malloc((high - low + 1) * sizeof(int));
    if (stack == NULL) return;

    int top = -1;
    stack[++top] = low;
    stack[++top] = high;

    while (top >= 0) {
        high = stack[top--];
        low = stack[top--];

        int pivot = arr[high];
        int i = low - 1;

        for (int j = low; j < high; j++) {
            if (arr[j] <= pivot) {
                i++;
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }

        int temp = arr[i + 1];
        arr[i + 1] = arr[high];
        arr[high] = temp;

        int pi = i + 1;

        if (pi - 1 > low) {
            stack[++top] = low;
            stack[++top] = pi - 1;
        }

        if (pi + 1 < high) {
            stack[++top] = pi + 1;
            stack[++top] = high;
        }
    }

    free(stack);
}

void quickSortWrapper(int* arr, int n) {
    quickSortIterative(arr, 0, n - 1);
}

void shellSort(int* arr, int n) {
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            int temp = arr[i];
            int j;
            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap) {
                arr[j] = arr[j - gap];
            }
            arr[j] = temp;
        }
    }
}

void qsortWrapper(int* arr, int n) {
    qsort(arr, n, sizeof(int), compare);
}

// Функция для измерения времени сортировки
double measureTime(void (*sortFunc)(int*, int), int* arr, int n) {
    int* copy = copyArray(arr, n);
    clock_t start = clock();
    sortFunc(copy, n);
    clock_t end = clock();
    free(copy);
    return ((double)(end - start)) / CLOCKS_PER_SEC;
}

// Вывод заголовка таблицы
void printTableHeader() {
    printf("┌────────┬─────────────┬──────────────┬─────────────┬─────────────┐\n");
    printf("│   N    │  Случайный  │ Возрастающий │  Убывающий  │  Вз и Уб    │\n");
    printf("├────────┼─────────────┼──────────────┼──────────────────────────┤\n");
}

// Вывод строки таблицы для одного алгоритма
void printTableRow(const char* algoName, double random, double ascending,
    double descending, double halfAscHalfDesc) {
    printf("│%-8s│%11.6f│%12.6f│%11.6f│%11.6f│\n",
        algoName, random, ascending, descending, halfAscHalfDesc);
}

// Вывод разделителя
void printSeparator() {
    printf("├─────────────────────┼──────────────┼─────────────┼─────────────┤\n");
}

// Вывод конца таблицы
void printTableFooter() {
    printf("────────┴─────────────┴──────────────┴─────────────┴─────────────┘\n");
}

void runTestsForSize(int n) {
    int* arr = (int*)malloc(n * sizeof(int));
    if (arr == NULL) {
        printf("Ошибка выделения памяти!\n");
        return;
    }

    printf("\nРазмер массива: %d\n", n);
    printTableHeader();

    // Тест для случайного массива
    generateRandom(arr, n, n);
    double quickRandom = measureTime(quickSortWrapper, arr, n);
    double shellRandom = measureTime(shellSort, arr, n);
    double qsortRandom = measureTime(qsortWrapper, arr, n);

    // Тест для возрастающего массива
    generateAscending(arr, n);
    double quickAsc = measureTime(quickSortWrapper, arr, n);
    double shellAsc = measureTime(shellSort, arr, n);
    double qsortAsc = measureTime(qsortWrapper, arr, n);

    // Тест для убывающего массива
    generateDescending(arr, n);
    double quickDesc = measureTime(quickSortWrapper, arr, n);
    double shellDesc = measureTime(shellSort, arr, n);
    double qsortDesc = measureTime(qsortWrapper, arr, n);

    // Тест для половины возрастающего/убывающего
    generateHalfAscHalfDesc(arr, n);
    double quickHalf = measureTime(quickSortWrapper, arr, n);
    double shellHalf = measureTime(shellSort, arr, n);
    double qsortHalf = measureTime(qsortWrapper, arr, n);

    // Вывод результатов
    printTableRow("QuickSort", quickRandom, quickAsc, quickDesc, quickHalf);
    printSeparator();
    printTableRow("ShellSort", shellRandom, shellAsc, shellDesc, shellHalf);
    printSeparator();
    printTableRow("QSort", qsortRandom, qsortAsc, qsortDesc, qsortHalf);

    printTableFooter();

    free(arr);
}

int main() {
    srand(time(NULL));
    setlocale(LC_ALL, "Russian");

    printf("=== Тестирование методов сортировки ===\n");

    // Сначала тест для 5000 элементов
    runTestsForSize(5000);

    // Потом тест для 50000 элементов
    runTestsForSize(50000);

    return 0;
}