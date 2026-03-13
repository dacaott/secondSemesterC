#ifndef BST_H
#define BST_H

#include <stdbool.h>

struct BSTNode {
    struct BSTNode* left;
    struct BSTNode* right;
    int val;
};

struct BST {
    struct BSTNode* head;
};

// A
void bstInsert(struct BST* tree, int val);
bool bstContains(struct BST* tree, int val);
void bstFree(struct BST* tree);

// B
void bstInorder(struct BST* tree);
void bstPreorder(struct BST* tree);
void bstPostorder(struct BST* tree);

// C
int bstHeight(struct BST* tree);
int bstSize(struct BST* tree);
int bstMax(struct BST* tree);
int bstMin(struct BST* tree);

// D
void bstDelete(struct BST* tree, int value);

#endif // BST_H