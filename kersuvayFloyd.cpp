#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

void printResult(int** dist, int** next, int n, FILE* file) {
    printf("\nКратчайшие расстояния между вершинами:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (dist[i][j] == INT_MAX)
                printf("%7s", "INF");
            else
                printf("%7d", dist[i][j]);
        }
        printf("\n");
    }

    fprintf(file, "\nКратчайшие расстояния между вершинами:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (dist[i][j] == INT_MAX)
                fprintf(file, "%7s", "INF");
            else
                fprintf(file, "%7d", dist[i][j]);
        }
        fprintf(file, "\n");
    }

    printf("\nКратчайшие пути:\n");
    fprintf(file, "\nКратчайшие пути:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j && dist[i][j] != INT_MAX) {
                printf("Путь из %d в %d: ", i, j);
                fprintf(file, "Путь из %d в %d: ", i, j);
                int u = i;
                while (u != j) {
                    printf("%d -> ", u);
                    fprintf(file, "%d -> ", u);
                    u = next[u][j];
                }
                printf("%d\n", j);
                fprintf(file, "%d\n", j);
            }
        }
    }
}

void floyd(int** graph, int n) {
    int** dist = (int**)malloc(n * sizeof(int*));
    int** next = (int**)malloc(n * sizeof(int*));

    for (int i = 0; i < n; i++) {
        dist[i] = (int*)malloc(n * sizeof(int));
        next[i] = (int*)malloc(n * sizeof(int));
        for (int j = 0; j < n; j++) {
            dist[i][j] = graph[i][j];
            next[i][j] = (graph[i][j] != INT_MAX && i != j) ? j : -1;
        }
    }

    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX
                    && dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    next[i][j] = next[i][k];
                }
            }
        }
    }

    FILE* file = fopen("result.txt", "w");
    if (file == NULL) {
        printf("Ошибка открытия файла!\n");
        return;
    }

    printResult(dist, next, n, file);

    fclose(file);
    printf("Результаты сохранены в файл result.txt\n");

    for (int i = 0; i < n; i++) {
        free(dist[i]);
        free(next[i]);
    }
    free(dist);
    free(next);
}

void readGraph(int*** graph, int* n, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Ошибка: не удалось открыть файл %s\n", filename);
        exit(EXIT_FAILURE);
    }

    fscanf(file, "%d", n);

    *graph = (int**)malloc(*n * sizeof(int*));
    for (int i = 0; i < *n; i++) {
        (*graph)[i] = (int*)malloc(*n * sizeof(int));
        for (int j = 0; j < *n; j++) {
            int value;
            fscanf(file, "%d", &value);
            if (value == -1) {
                (*graph)[i][j] = INT_MAX;
            }
            else {
                (*graph)[i][j] = value;
            }
        }
    }

    fclose(file);
}

int main() {
    int n;
    int** graph;
    int option;

    printf("Выберите способ задания графа:\n");
    printf("1. Сгенерировать случайный граф\n");
    printf("2. Загрузить граф из файла\n");
    scanf("%d", &option);

    if (option == 1) {
        printf("Введите количество вершин: ");
        scanf("%d", &n);

        graph = (int**)malloc(n * sizeof(int*));
        for (int i = 0; i < n; i++) {
            graph[i] = (int*)malloc(n * sizeof(int));
        }

        srand(time(NULL));
        printf("Случайная матрица смежности:\n");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i == j) {
                    graph[i][j] = 0;
                }
                else {
                    graph[i][j] = rand() % 20 + 1;
                    if (rand() % 2 == 0) {
                        graph[i][j] = INT_MAX;
                    }
                }
                if (graph[i][j] == INT_MAX)
                    printf("%7s", "INF");
                else
                    printf("%7d", graph[i][j]);
            }
            printf("\n");
        }
    }
    else if (option == 2) {
        char filename[100];
        printf("Введите имя файла: ");
        scanf("%s", filename);

        readGraph(&graph, &n, filename);

        printf("Матрица смежности из файла:\n");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (graph[i][j] == INT_MAX)
                    printf("%7s", "INF");
                else
                    printf("%7d", graph[i][j]);
            }
            printf("\n");
        }
    }
    else {
        printf("Неверный выбор!\n");
        return 1;
    }

    floyd(graph, n);

    for (int i = 0; i < n; i++) {
        free(graph[i]);
    }
    free(graph);

    return 0;
}