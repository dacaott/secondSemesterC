#ifndef TREE_H
#define TREE_H

#include <stdio.h>

// AVL Tree node
struct Node {
    char key[4];
    char* value;
    struct Node* left;
    struct Node* right;
    int height;
};

struct Node* insert(struct Node* node, char key[4], char* value);
struct Node* buildTree(FILE* file);
struct Node* find(struct Node* node, char key[4]);
void saveTree(struct Node* root, FILE* file);
void deleteTree(struct Node* root);
struct Node* deleteNode(struct Node* root, char key[4]);

#endif // TREE_H