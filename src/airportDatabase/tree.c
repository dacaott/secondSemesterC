#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void freeNode(struct Node* node)
{
    if (node != NULL) {
        free(node->value);
        free(node);
    }
}

static int getHeight(struct Node* n)
{
    if (n == NULL)
        return 0;
    return n->height;
}

static struct Node* createNode(char key[4], char* value)
{
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    sprintf(node->key, "%s", key);
    node->value = (char*)malloc(strlen(value) + 1);
    sprintf(node->value, "%s", value);
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

static int max_int(int a, int b) { return (a > b) ? a : b; }

static int getBalanceFactor(struct Node* n)
{
    if (n == NULL)
        return 0;
    return getHeight(n->left) - getHeight(n->right);
}

static struct Node* rightRotate(struct Node* y)
{
    struct Node* x = y->left;
    struct Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max_int(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = max_int(getHeight(x->left), getHeight(x->right)) + 1;

    return x;
}

static struct Node* leftRotate(struct Node* x)
{
    struct Node* y = x->right;
    struct Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max_int(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max_int(getHeight(y->left), getHeight(y->right)) + 1;

    return y;
}

struct Node* insert(struct Node* node, char key[4], char* value)
{
    if (node == NULL)
        return createNode(key, value);

    if (strcmp(key, node->key) < 0)
        node->left = insert(node->left, key, value);
    else if (strcmp(key, node->key) > 0)
        node->right = insert(node->right, key, value);
    else
        return node;

    node->height = 1 + max_int(getHeight(node->left), getHeight(node->right));
    int balance = getBalanceFactor(node);

    if (balance > 1 && strcmp(key, node->left->key) < 0)
        return rightRotate(node);
    if (balance < -1 && strcmp(key, node->right->key) > 0)
        return leftRotate(node);
    if (balance > 1 && strcmp(key, node->left->key) > 0) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    if (balance < -1 && strcmp(key, node->right->key) < 0) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}

struct Node* find(struct Node* node, char key[4])
{
    if (node == NULL)
        return NULL;
    if (strcmp(key, node->key) == 0)
        return node;
    else if (strcmp(key, node->key) < 0)
        return find(node->left, key);
    else
        return find(node->right, key);
}

static struct Node* minValueNode(struct Node* node)
{
    struct Node* current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}

struct Node* deleteNode(struct Node* root, char key[4])
{
    if (root == NULL)
        return root;

    if (strcmp(key, root->key) < 0)
        root->left = deleteNode(root->left, key);
    else if (strcmp(key, root->key) > 0)
        root->right = deleteNode(root->right, key);
    else {
        if ((root->left == NULL) || (root->right == NULL)) {
            struct Node* temp = root->left ? root->left : root->right;
            if (temp == NULL) {
                temp = root;
                root = NULL;
                freeNode(temp);
            } else {
                freeNode(root);
                return temp;
            }
        } else {
            struct Node* temp = minValueNode(root->right);
            strcpy(root->key, temp->key);

            free(root->value);
            root->value = (char*)malloc(strlen(temp->value) + 1);
            strcpy(root->value, temp->value);

            root->right = deleteNode(root->right, temp->key);
        }
    }

    if (root == NULL)
        return root;

    root->height = 1 + max_int(getHeight(root->left), getHeight(root->right));
    int balance = getBalanceFactor(root);

    if (balance > 1 && getBalanceFactor(root->left) >= 0)
        return rightRotate(root);
    if (balance > 1 && getBalanceFactor(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    if (balance < -1 && getBalanceFactor(root->right) <= 0)
        return leftRotate(root);
    if (balance < -1 && getBalanceFactor(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
    return root;
}

void saveTree(struct Node* root, FILE* file)
{
    if (root == NULL)
        return;
    saveTree(root->left, file);
    fprintf(file, "%s:%s\n", root->key, root->value);
    saveTree(root->right, file);
}

void deleteTree(struct Node* root)
{
    if (root == NULL)
        return;
    deleteTree(root->left);
    deleteTree(root->right);
    freeNode(root);
}

struct Node* buildTree(FILE* file)
{
    struct Node* root = NULL;
    char line[1000];
    while (fgets(line, sizeof(line), file)) {
        char key[4];
        char value[1000];
        if (sscanf(line, "%[^:]:%[^\n]", key, value) == 2) {
            root = insert(root, key, value);
        }
    }
    return root;
}