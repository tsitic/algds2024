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

struct treaps* rhtgRotation(struct treaps* y) {
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
            root = rhtgRotation(root);
    }
    else {
        root->right = insert(root->right, key);

        if (root->right && root->right->val > root->val)
            root = lftRotation(root);
    }
    return root;
}

struct treaps* join(struct treaps* left, struct treaps* right) {
    if (!left) return right;
    if (!right) return left;

    if (left->val > right->val) {
        left->right = join(left->right, right);
        return left;
    }
    else {
        right->left = join(left, right->left);
        return right;
    }
}

void split(struct treaps* root, int key, struct treaps** left, struct treaps** right) {
    if (!root) {
        *left = *right = NULL;
    }
    else if (key <= root->key) {
        split(root->left, key, left, &root->left);
        *right = root;
    }
    else {
        split(root->right, key, &root->right, right);
        *left = root;
    }
}

struct treaps* delNode(struct treaps* root, int key) {
    if (!root)
        return root;

    if (key < root->key)
        root->left = delNode(root->left, key);
    else if (key > root->key)
        root->right = delNode(root->right, key);
    else {
        struct treaps* temp = join(root->left, root->right);
        free(root);
        return temp;
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

    printf("Дерево после вставок:\n\n");
    printTreap(root, 0);

    printf("\n\nУдаляем элемент с ключом 20\n\n");
    root = delNode(root, 20);

    printf("\nДерево после удаления:\n\n");
    printTreap(root, 0);

    struct treaps* left = NULL;
    struct treaps* right = NULL;

    printf("\n\nРасцепляем дерево по ключу 40\n\n");
    split(root, 40, &left, &right);

    printf("\n\n\nЛевое дерево:\n");
    printTreap(left, 0);

    printf("\n\n\nПравое дерево:\n");
    printTreap(right, 0);

    printf("\n\nСцепляем левое и правое деревья\n\n");
    root = join(left, right);

    printf("\n\nДерево после сцепления:\\n");
    printTreap(root, 0);

    return 0;
}
