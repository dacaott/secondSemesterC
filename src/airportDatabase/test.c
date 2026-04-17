#include "tree.h"
#include <stdio.h>
#include <string.h>

int main()
{
    int errors = 0;
    struct Node* root = NULL;
    struct Node* result = NULL;

    printf("Запуск тестов...\n");

    // 1. Добавление и поиск
    printf("Тест 1 (Добавление и поиск): ");
    root = insert(root, "JFK", "John F. Kennedy");
    root = insert(root, "LAX", "Los Angeles");

    result = find(root, "LAX");
    if (result != NULL && strcmp(result->value, "Los Angeles") == 0) {
        printf("OK\n");
    } else {
        printf("FAIL\n");
        errors++;
    }

    // 2. Поиск несуществующего узла
    printf("Тест 2 (Поиск несуществующего): ");
    result = find(root, "DME");
    if (result == NULL) {
        printf("OK\n");
    } else {
        printf("FAIL\n");
        errors++;
    }

    // 3. Удаление простого узла (листа)
    printf("Тест 3 (Удаление листа LAX): ");
    root = deleteNode(root, "LAX");
    result = find(root, "LAX");
    if (result == NULL) {
        printf("OK\n");
    } else {
        printf("FAIL\n");
        errors++;
    }

    // 4. Удаление узла с потомками
    printf("Тест 4 (Удаление сложного узла): ");
    root = insert(root, "MIA", "Miami");
    root = insert(root, "ORD", "Chicago");
    root = deleteNode(root, "JFK");

    result = find(root, "JFK");
    struct Node* checkMia = find(root, "MIA");

    if (result == NULL && checkMia != NULL && strcmp(checkMia->value, "Miami") == 0) {
        printf("OK\n");
    } else {
        printf("FAIL\n");
        errors++;
    }

    printf("\nОшибок найдено: %d\n", errors);

    deleteTree(root);

    if (errors == 0) {
        return 0;
    } else {
        return 1;
    }
}