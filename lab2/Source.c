#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>

struct treaps {
    int key;
    int val;
    struct treaps* left;
    struct treaps* right;
};

struct treaps* rghtRotation(struct treaps* y) {
    struct treaps* x = y->left;
    struct treaps* T2 = x->right;
    x->right = y;
    y->left = T2;
    return x;
}

struct treaps* lftRotation(struct treaps* x) {
    struct treaps* y = x->right;
    struct treaps* T2 = y->left;
    y->left = x;
    x->right = T2;
    return y;
}

struct treaps* newNode(int key) {
    struct treaps* temp = (struct treaps*)malloc(sizeof(struct treaps));
    temp->key = key;
    temp->val = rand() % 100;
    temp->left = temp->right = NULL;
    return temp;
}

struct treaps* insert(struct treaps* root, int key) {
    if (!root)
        return newNode(key);

    if (key <= root->key) {
        root->left = insert(root->left, key);

        if (root->left && root->left->val > root->val)
            root = rghtRotation(root);
    }
    else {
        root->right = insert(root->right, key);

        if (root->right && root->right->val > root->val)
            root = lftRotation(root);
    }
    return root;
}


struct treaps* delNode(struct treaps* root, int key) {
    if (!root)
        return root;

    if (key < root->key)
        root->left = delNode(root->left, key);
    else if (key > root->key)
        root->right = delNode(root->right, key);
    else {
        if (!root->left) {
            struct treaps* temp = root->right;
            free(root);
            root = temp;
        }
        else if (!root->right) {
            struct treaps* temp = root->left;
            free(root);
            root = temp;
        }
        else {
            if (root->left->val < root->right->val) {
                root = lftRotation(root);
                root->left = delNode(root->left, key);
            }
            else {
                root = rghtRotation(root);
                root->right = delNode(root->right, key);
            }
        }
    }
    return root;
}

void printTreap(struct treaps* root, int k) {
    if (root == NULL) {
        return;
    }

    printTreap(root->right, k + 1);

    for (int i = 0; i < k; i++) {
        printf("\t");
    }
    printf("%d(%d)\n", root->key, root->val);

    printTreap(root->left, k + 1);
}

int main() {
    setlocale(LC_CTYPE, "Russian");
    srand(time(NULL));
    
    struct treaps* root = NULL;
    root = insert(root, 50);
    root = insert(root, 30);
    root = insert(root, 20);
    root = insert(root, 40);
    root = insert(root, 70);
    root = insert(root, 60);
    root = insert(root, 80);

    printf("Дерево:\n\n");
    printTreap(root, 0);

    printf("\n\nУдаляем элемент с ключом 20\n");
    root = delNode(root, 20);

    printf("\nДерево после удаления:\n");
    printTreap(root, 0);

    return 0;
}
