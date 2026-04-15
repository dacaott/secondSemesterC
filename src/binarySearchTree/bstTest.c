#include "bst.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    struct BST* tree = (struct BST*)malloc(sizeof(struct BST));
    tree->head = NULL;

    // Тест 1 (пустое дерево)
    assert(bstSize(tree) == 0);
    assert(bstHeight(tree) == 0);
    assert(bstContains(tree, 10) == false);
    printf("Empty tree tests -- passed.\n");

    // Тест 2 (вставка и наличие элементов)
    int arr[] = { 6, 7, 9, 4, 2, 5, 10, 8 };
    for (int i = 0; i < 8; i++) {
        bstInsert(tree, arr[i]);
    }
    assert(bstSize(tree) == 8);
    assert(bstContains(tree, 9) == true);
    assert(bstContains(tree, 2) == true);
    assert(bstContains(tree, 100) == false);
    printf("Insert & Contains tests -- passed.\n");

    // Тест 3 (статистика дерева)
    assert(bstMin(tree) == 2);
    assert(bstMax(tree) == 10);
    assert(bstHeight(tree) == 4);
    printf("Statistics tests -- passed.\n");

    // Тест 4 (удаление листьев если нет потомков)
    bstDelete(tree, 2);
    assert(bstContains(tree, 2) == false);
    assert(bstSize(tree) == 7);
    printf("Leaf deletion -- passed.\n");

    // Тест 5 (удаление узла с двумя потомками, если корень = 6)
    bstDelete(tree, 6);
    assert(bstContains(tree, 6) == false);
    assert(bstSize(tree) == 6);
    // корень должен был замениться на 7 (минимум из правого поддерева)
    assert(tree->head->val == 7);
    printf("Two-children deletion -- passed.\n");

    bstFree(tree);
    printf("\nAll tests passed successfully!\n");

    return 0;
}