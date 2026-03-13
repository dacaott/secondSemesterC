#include "bst.h"
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct BSTNode* insert(struct BSTNode* root, int val)
{
    if (root == NULL) {
        struct BSTNode* res = (struct BSTNode*)malloc(sizeof(struct BSTNode));
        res->val = val;
        res->left = NULL;
        res->right = NULL;
        return res;
    }
    if (root->val < val)
        root->right = insert(root->right, val);
    if (root->val > val)
        root->left = insert(root->left, val);
    return root;
}

bool contains(struct BSTNode* root, int value)
{
    if (root == NULL) {
        return false;
    }
    if (root->val == value) {
        return true;
    }

    if (root->val < value)
        return contains(root->right, value);
    if (root->val > value)
        return contains(root->left, value);
}

void freeNode(struct BSTNode* root)
{
    if (root == NULL)
        return;
    freeNode(root->left);
    freeNode(root->right);
    free(root);
}

struct BSTNode* findMinNode(struct BSTNode* root)
{
    while (root->left != NULL)
        root = root->left;
    return root;
}

struct BSTNode* deleteNode(struct BSTNode* root, int val)
{
    if (root == NULL)
        return NULL;
    if (val < root->val)
        root->left = deleteNode(root->left, val);
    else if (val > root->val)
        root->right = deleteNode(root->right, val);
    else {
        if (root->left == NULL) {
            struct BSTNode* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            struct BSTNode* temp = root->left;
            free(root);
            return temp;
        }
        struct BSTNode* temp = findMinNode(root->right);
        root->val = temp->val;
        root->right = deleteNode(root->right, temp->val);
    }
    return root;
}

int height(struct BSTNode* root)
{
    if (root == NULL) {
        return 0;
    }
    int leftHeight = height(root->left);
    int rightHeight = height(root->right);

    if (leftHeight < rightHeight) {
        return rightHeight + 1;
    } else {
        return leftHeight + 1;
    }
}

int size(struct BSTNode* root)
{
    if (root == NULL) {
        return 0;
    }
    return 1 + size(root->left) + size(root->right);
}

void inorder(struct BSTNode* root)
{
    if (root == NULL)
        return;
    inorder(root->left);
    printf("%d\n", root->val);
    inorder(root->right);
}
void preorder(struct BSTNode* root)
{
    if (root == NULL)
        return;
    printf("%d\n", root->val);
    preorder(root->left);
    preorder(root->right);
}
void postorder(struct BSTNode* root)
{
    if (root == NULL)
        return;
    postorder(root->left);
    postorder(root->right);
    printf("%d\n", root->val);
}

void bstInsert(struct BST* tree, int val)
{
    if (tree != NULL)
        tree->head = insert(tree->head, val);
}

bool bstContains(struct BST* tree, int val)
{
    if (tree == NULL)
        return false;
    return contains(tree->head, val);
}

void bstFree(struct BST* tree)
{
    if (tree != NULL) {
        freeNode(tree->head);
        free(tree);
    }
}

int bstHeight(struct BST* tree)
{
    return height(tree->head);
}

int bstSize(struct BST* tree)
{
    return size(tree->head);
}

// Если дерево пустое, функция возвращает INT_MIN
int bstMax(struct BST* tree)
{
    if (tree == NULL || tree->head == NULL)
        return INT_MIN;
    struct BSTNode* root = tree->head;
    while (root->right != NULL) {
        root = root->right;
    }
    return root->val;
}

// Если дерево пустое, функция возвращает INT_MAX
int bstMin(struct BST* tree)
{
    if (tree == NULL || tree->head == NULL)
        return INT_MAX;
    struct BSTNode* root = tree->head;
    while (root->left != NULL) {
        root = root->left;
    }
    return root->val;
}

void bstDelete(struct BST* tree, int value)
{
    if (tree != NULL)
        tree->head = deleteNode(tree->head, value);
}

void bstInorder(struct BST* tree)
{
    if (tree == NULL)
        return;
    inorder(tree->head);
    printf("\n");
}

void bstPreorder(struct BST* tree)
{
    if (tree == NULL)
        return;
    preorder(tree->head);
    printf("\n");
}

void bstPostorder(struct BST* tree)
{
    if (tree == NULL)
        return;
    postorder(tree->head);
    printf("\n");
}
